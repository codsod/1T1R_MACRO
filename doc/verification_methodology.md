# RRAM CIM Macro 验证方法论

## 1. 现有 Testbench 分析

### 1.1 目录结构

```
verif/
├── mult_int16/
│   ├── testbench.sv          # mult_int16 单模块定向测试
│   ├── simv                   # VCS 编译产物
│   └── sim.log
├── macro_llc/
│   ├── testbench.sv          # rram_1t1r_macro_llc 定向测试
│   └── simv
├── rram_macro_llc_top/
│   ├── testbench.sv          # LLC + subarray 顶层测试
│   └── simv
└── rram_macro_top/
    ├── testbench.sv          # 全芯片顶层测试
    └── simv
```

### 1.2 现有 Testbench 特征

| 特征 | 说明 |
|------|------|
| 架构 | 单一 `module`，所有激励、检查逻辑混在 `initial` 块中 |
| 激励方式 | 定向（directed）：固定 `for` 循环 + `$random` 函数 |
| 结果检查 | 手写 `if/else` + `$display` 打印 pass/fail |
| 参考模型 | 内联在 testbench 中，与激励混合 |
| 波形输出 | `$fsdbDumpvars` 导出 FSDB 波形 |
| 流水对齐 | 手写 4 级延迟寄存器链（`_reg` / `_d` / `_2d` / `_3d`） |
| DUT 访问 | 直接探内部信号：`macro_llc.vldi_d[0]` |

### 1.3 现有 Testbench 的问题

#### 流水延迟链爆炸

LLC 内部有 3 拍流水延迟（vldi → vldo），现有 testbench 用显式寄存器链对齐参考值和实际输出：

```verilog
// 5 个 mode，每个需要多组 4 级延迟链
opa_int16_reg[j]    <= opa_int16[j];        // +0 拍
opa_int16_reg_d[j]  <= opa_int16_reg[j];     // +1 拍
opa_int16_reg_2d[j] <= opa_int16_reg_d[j];   // +2 拍
opa_int16_reg_3d[j] <= opa_int16_reg_2d[j];  // +3 拍 → 对齐 vldo

gold_out_int16_reg    <= gold_out_int16;
gold_out_int16_reg_d  <= gold_out_int16_reg;
gold_out_int16_reg_2d <= gold_out_int16_reg_d;
gold_out_int16_reg_3d <= gold_out_int16_reg_2d;
// ... 同样模式重复用于 INT8 / INT4 / Ternary
```

问题：代码膨胀，对齐逻辑占据大量行数；DUT 流水深度变化时需要重写所有延迟链。

#### 强绑定 DUT 内部信号

```verilog
if (macro_llc.vldi_d[0]) begin
    if ($signed(gold_int16_reg[j]) == $signed(macro_llc.out_mult_int16[j]))
```

问题：
- 测试与 DUT 内部实现耦合
- DUT 内部信号重命名/重构 → testbench 全坏
- 综合后网表中这些信号可能被优化掉 → 后仿无法运行

#### 测试用例不可组合

4 个 mode 的测试是 4 段 copy-paste 代码，结构相同只是变量名不同。要增加新的测试组合（如 mode=0 + 随机 mask），需要再写一段新代码。

#### 无覆盖率

没有 functional coverage、cross coverage 收集，无法量化验证完备性。

---

## 2. UVM 验证架构

### 2.1 整体结构

```
uvm_test_top (base_test)
├── uvm_env (llc_env)
│   ├── opa_agent (in)        ──── 驱动 opa/opb/mask
│   │   ├── sequencer
│   │   ├── driver
│   │   └── monitor
│   ├── cfg_agent (in)        ──── 驱动 mode
│   │   ├── sequencer
│   │   ├── driver
│   │   └── monitor
│   ├── out_agent (in)        ──── 监控 vldo/out
│   │   ├── monitor
│   │   └── coverage_collector
│   └── scoreboard
│       ├── reference_model   ──── C 参考模型
│       └── comparator        ──── 期望值 vs 实际值
└── virtual_sequencer
    └── sequences
        ├── llc_int16_seq
        ├── llc_int8_seq
        ├── llc_int4_seq
        ├── llc_ternary_seq
        ├── llc_mask_seq
        └── llc_stress_seq
```

### 2.2 DUT 接口定义

```verilog
// llc_if.sv - 顶层 interface
interface llc_if(input bit clk);
    logic                 rstn;
    logic                 vldi;
    logic [2:0]           mode;
    logic [1023:0]        opa;
    logic [1023:0]        opb;
    logic [1023:0]        mask;
    logic                 vldo;
    logic [41:0]          out;

    clocking drv_cb @(posedge clk);
        default input #1ns output #1ns;
        output rstn, vldi, mode, opa, opb, mask;
    endclocking

    clocking mon_cb @(posedge clk);
        default input #1ns;
        input rstn, vldi, mode, opa, opb, mask, vldo, out;
    endclocking
endinterface
```

### 2.3 Scoreboard 设计（关键）

Scoreboard 是验证的核心，需要精确复现 LLC 的数学行为：

```verilog
class llc_scoreboard extends uvm_scoreboard;

    uvm_analysis_imp_in_op  #(llc_in_tr,  llc_scoreboard) in_op_imp;
    uvm_analysis_imp_in_out #(llc_out_tr, llc_scoreboard) in_out_imp;

    // 期望值 FIFO，自动对齐 3 拍流水延迟
    bit [41:0] exp_q[$];
    int unsigned pipeline_depth = 3;

    // 参考模型
    function bit [41:0] ref_model(bit [1023:0] opa, opb, mask, bit [2:0] mode);
        bit signed [41:0] sum = 0;
        for (int i = 0; i < 64; i++) begin
            bit [15:0] oa = opa[i*16+:16];
            bit [15:0] ob = opb[i*16+:16];
            if (!mask[i]) continue;
            // 调用 mult_int16 参考函数
            sum += mult_int16_ref(oa[15:8], oa[7:0], ob[15:8], ob[7:0], mode);
        end
        return sum;
    endfunction

    function void write_in_op(llc_in_tr tr);
        bit [41:0] expected = ref_model(tr.opa, tr.opb, tr.mask, tr.mode);
        exp_q.push_back(expected);
    endfunction

    function void write_in_out(llc_out_tr tr);
        if (exp_q.size() == 0) begin
            `uvm_error("SCBD", "Unexpected output - no matching input")
            return;
        end
        bit [41:0] expected = exp_q.pop_front();
        if (tr.out !== expected) begin
            `uvm_error("SCBD", $sformatf(
                "Mismatch: vldi_cycle=%0d got=0x%0h expected=0x%0h",
                tr.vldi_cycle, tr.out, expected))
        end
    endfunction
endclass
```

### 2.4 参考模型关键注意点

1. **INT16 −1 → 0 映射**：`mult_int16` 中高 8 bit 为 `−1` 时被强制映射为 0。参考模型必须复现此硬件 workaround，否则比对失败。
2. **符号扩展差异**：INT16 模式下 opa_2/opb_2 是零扩展（`{1'b0, val}`），INT8 模式下 opa_2/opb_2 是符号扩展（`{val[7], val}`）。参考模型需要按 mode 区分。
3. **INT8 交叉项**：mode=1 时 mult_int16 内部 mult1/mult2 计算了交叉项但不参与最终求和，参考模型可以直接用 `opa_1×opb_1 + opa_2×opb_2`。
4. **vldi 门控**：`vldi=0` 时 opa/opb 被清零（`& {IN_WIDTH{vldi}}`），scoreboard 收到 vldi=0 的 transaction 时参考模型应输出 0。

---

## 3. 验证 Task 列表

| # | Task | 模式 | 激励策略 | 检查点 |
|---|------|------|---------|--------|
| 1 | `test_int16_random` | 0 | 64 lane 随机 16-bit 有符号数 | lane 级 + MAC 总结果 |
| 2 | `test_int16_corner` | 0 | 边界值：0x0000, 0x7FFF, 0x8000, 0xFFFF, 0xFFxx | -1→0 映射正确性 |
| 3 | `test_int8_random` | 1 | 每 slot 两组独立随机 INT8 | lane 级 2 乘加 + MAC 总结果 |
| 4 | `test_int8_sign_boundary` | 1 | 边界值：0x7F, 0x80, 0x00, 0xFF | 符号扩展正确性 |
| 5 | `test_int4_random` | 2 | 每 slot 四组独立随机 INT4 | lane 级 4 乘加 |
| 6 | `test_ternary_random` | 4 | 8 组 2-bit 随机值 | 两两求和处理 |
| 7 | `test_mask_random` | all | 随机 mask 位 + 随机操作数 | 被 mask lane 输出为 0 |
| 8 | `test_pipeline_cont` | 0 | 连续 100 笔 vldi（背靠背） | vldo 3 拍延迟，不乱序 |
| 9 | `test_pipeline_gap` | 0 | 间隔随机拍数的 vldi | 空闲拍 vldo=0 |
| 10 | `test_reset` | 0 | 随机时刻拉低 rstn | 状态清零，不留残余 |
| 11 | `test_mode_switch` | 0→1→2 | 连续切换 mode | 模式切换不串扰 |
| 12 | `test_stress_rand` | rand | 随机 mode + 随机 mask + 随机间隔 | 多维度交叉覆盖 |

---

## 4. 现有 Testbench vs UVM 对比

| | 现有 testbench | UVM |
|---|---|---|
| **封装** | 单一 module | 类分层：agent/driver/monitor/scoreboard |
| **激励** | 定向 for 循环 + $random | 约束随机：rand + constraint |
| **结果检查** | 手写 if/else + $display | scoreboard 自动比对 + uvm_error 宏 |
| **流水对齐** | 手写 4 级延迟寄存器链 | scoreboard 内 FIFO 自动对齐 |
| **复用性** | 4 个 mode = 4 段 copy-paste | base sequence + 参数化派生 |
| **DUT 访问** | 直接探内部信号 | 仅通过 interface clocking block |
| **覆盖率** | 无 | functional + cross coverage |
| **报告** | $display 打印 | uvm_report 统计 + 分类 + 日志 |
| **可扩展性** | 加新测试 = 写新 initial 块 | 加新 sequence 即可 |
| **学习成本** | 低 | 较高 |
| **适用阶段** | 早期 RTL 调试 | 回归测试 / 流片 sign-off |

---

## 5. 搭建步骤

### 5.1 目录规划

```
verif/llc_uvm/
├── env/
│   ├── llc_if.sv              # DUT 接口
│   ├── llc_pkg.sv             # UVM 包（含 transaction、sequence、agent 等）
│   ├── llc_env.sv             # UVM env（组装 agent + scoreboard）
│   ├── llc_scoreboard.sv      # 参考模型 + 比对
│   └── llc_coverage.sv        # 功能覆盖率
├── agent/
│   ├── llc_opa_agent.sv       # OPA/OPB/mask driver + monitor
│   ├── llc_cfg_agent.sv       # mode driver + monitor
│   └── llc_out_agent.sv       # vldo/out monitor
├── seq/
│   ├── llc_base_seq.sv        # 基类 sequence
│   ├── llc_int16_seq.sv       # INT16 定向/随机
│   ├── llc_int8_seq.sv        # INT8
│   ├── llc_int4_seq.sv        # INT4
│   ├── llc_ternary_seq.sv     # Ternary
│   ├── llc_mask_seq.sv        # Mask 测试
│   └── llc_stress_seq.sv      # 随机压力
├── test/
│   ├── llc_base_test.sv       # 基类 test
│   ├── test_int16_random.sv   # 各 test 入口
│   ├── test_int8_random.sv
│   ├── test_int4_random.sv
│   ├── test_ternary_random.sv
│   ├── test_mask.sv
│   ├── test_pipeline.sv
│   ├── test_reset.sv
│   └── test_regression.sv     # 回归合集
├── tb/
│   └── llc_tb_top.sv          # 顶层 module（例化 DUT + interface + 启动 UVM）
├── ref/
│   └── ref_mult_int16.cpp     # C 参考模型（可选，DPI-C 调用）
├── scripts/
│   ├── run_vcs.sh             # VCS 编译 + 仿真脚本
│   ├── run_regression.sh      # 批量回归脚本
│   └── cov_report.sh          # 覆盖率收集/合并脚本
└── Makefile
```

### 5.2 文件创建顺序（由上到下，每步完成后可独立编译检查）

```
Step 1  →  llc_if.sv            接口定义，无依赖
Step 2  →  llc_pkg.sv           定义 transaction、typedef，依赖 step1
Step 3  →  llc_opa_agent.sv     driver + monitor，依赖 step2
Step 4  →  llc_cfg_agent.sv     driver + monitor，依赖 step2
Step 5  →  llc_out_agent.sv     monitor，依赖 step2
Step 6  →  llc_scoreboard.sv    参考模型 + 比对逻辑，依赖 step2
Step 7  →  llc_coverage.sv      覆盖率定义，依赖 step2
Step 8  →  llc_env.sv           组装 agent + scoreboard + coverage，依赖 step3-7
Step 9  →  llc_base_seq.sv      sequence 基类，依赖 step2
Step 10 →  llc_int16_seq.sv     测试 sequence，依赖 step9
Step 11 →  llc_base_test.sv     test 基类，依赖 step8
Step 12 →  test_xxx.sv          各 test 入口，依赖 step8-10
Step 13 →  llc_tb_top.sv        顶层 module + run_test()，依赖 step1,12
Step 14 →  run_vcs.sh           编译仿真脚本
```

### 5.3 Step 1 — Interface（`env/llc_if.sv`）

```bash
cat > env/llc_if.sv << 'EOF'
`ifndef LLC_IF_SV
`define LLC_IF_SV

interface llc_if (input bit clk);
    logic                 rstn;
    logic                 vldi;
    logic [2:0]           mode;
    logic [1023:0]        opa;
    logic [1023:0]        opb;
    logic [1023:0]        mask;
    logic                 vldo;
    logic [41:0]          out;

    // driver 使用的 clocking block（#1ns 避免竞争）
    clocking drv_cb @(posedge clk);
        default input #1ns output #1ns;
        output rstn, vldi, mode, opa, opb, mask;
    endclocking

    // monitor 使用的 clocking block（#1ns 确保采样窗口）
    clocking mon_cb @(posedge clk);
        default input #1ns;
        input rstn, vldi, mode, opa, opb, mask, vldo, out;
    endclocking

    // driver 的 modport
    modport drv_mp (clocking drv_cb);
    // monitor 的 modport
    modport mon_mp (clocking mon_cb);
endinterface

`endif
EOF
```

### 5.4 Step 2 — Package（`env/llc_pkg.sv`）

```bash
cat > env/llc_pkg.sv << 'EOF'
package llc_pkg;
    import uvm_pkg::*;
    `include "uvm_macros.svh"

    // ---- 参数 ----
    parameter IN_WIDTH = 1024;
    parameter DATA_W   = 8;
    parameter OUT_WIDTH = 42;  // DATA_W*4+10

    // ---- Transaction：从 opa/cfg agent 传入 scoreboard ----
    class llc_in_tr extends uvm_sequence_item;
        rand bit [1023:0]  opa;
        rand bit [1023:0]  opb;
        rand bit [1023:0]  mask;
        rand bit [2:0]     mode;
        bit                 vldi;

        `uvm_object_utils_begin(llc_in_tr)
            `uvm_field_int(opa,  UVM_ALL_ON)
            `uvm_field_int(opb,  UVM_ALL_ON)
            `uvm_field_int(mask, UVM_ALL_ON)
            `uvm_field_int(mode, UVM_ALL_ON)
        `uvm_object_utils_end

        function new(string name = "llc_in_tr");
            super.new(name);
        endfunction

        // 约束：mode 在 0~4 内
        constraint c_mode { mode inside {0, 1, 2, 3, 4}; }
        // 约束：mask 密度 50%~100%（可调整）
        constraint c_mask { $countones(mask) > 512; }
    endclass

    // ---- Transaction：从 out agent 传入 scoreboard ----
    class llc_out_tr extends uvm_sequence_item;
        bit                 vldo;
        bit [41:0]          out;
        int                 cycle;       // 供 scoreboard 追踪

        `uvm_object_utils_begin(llc_out_tr)
            `uvm_field_int(vldo, UVM_ALL_ON)
            `uvm_field_int(out,  UVM_ALL_ON)
        `uvm_object_utils_end

        function new(string name = "llc_out_tr");
            super.new(name);
        endfunction
    endclass

    // ---- mult_int16 SV 参考模型（纯 SV 实现，无需 DPI-C） ----
    function automatic bit signed [16:0] mult_int16_ref_lane(
        input bit [7:0] opa_1, opa_2, opb_1, opb_2,
        input bit [2:0] mode
    );
        bit signed [7:0]  s_opa_1, s_opb_1;
        bit signed [8:0]  s_opa_2, s_opb_2;
        bit signed [16:0] p0, p1, p2, p3;
        bit signed [16:0] tp[4], tp2[4];            // Ternary
        bit signed [1:0]  t0_a, t0_b, t1_a, t1_b;
        bit signed [1:0]  t2_a, t2_b, t3_a, t3_b;
        bit signed [1:0]  t4_a, t4_b, t5_a, t5_b;
        bit signed [1:0]  t6_a, t6_b, t7_a, t7_b;

        case (mode)
        0, 3: begin   // INT16 / BF16
            s_opa_1 = ($signed(opa_1) == -1) ? 8'd0 : opa_1;
            s_opb_1 = ($signed(opb_1) == -1) ? 8'd0 : opb_1;
            s_opa_2 = {1'b0, opa_2};                     // 零扩展
            s_opb_2 = {1'b0, opb_2};
            p0 = s_opa_1 * s_opb_1;
            p1 = s_opa_1 * s_opb_2;
            p2 = s_opa_2 * s_opb_1;
            p3 = s_opa_2 * s_opb_2;
            return (p0 << 16) + (p1 << 8) + (p2 << 8) + p3;
        end
        1: begin     // INT8
            return $signed(opa_2) * $signed(opb_2)
                 + $signed(opa_1) * $signed(opb_1);
        end
        2: begin     // INT4
            return $signed(opa_1[3:0]) * $signed(opb_1[3:0])
                 + $signed(opa_1[7:4]) * $signed(opb_1[7:4])
                 + $signed(opa_2[3:0]) * $signed(opb_2[3:0])
                 + $signed(opa_2[7:4]) * $signed(opb_2[7:4]);
        end
        4: begin     // Ternary
            t0_a = $signed(opa_1[1:0]); t0_b = $signed(opb_1[1:0]);
            t1_a = $signed(opa_1[3:2]); t1_b = $signed(opb_1[3:2]);
            t2_a = $signed(opa_1[5:4]); t2_b = $signed(opb_1[5:4]);
            t3_a = $signed(opa_1[7:6]); t3_b = $signed(opb_1[7:6]);
            t4_a = $signed(opa_2[1:0]); t4_b = $signed(opb_2[1:0]);
            t5_a = $signed(opa_2[3:2]); t5_b = $signed(opb_2[3:2]);
            t6_a = $signed(opa_2[5:4]); t6_b = $signed(opb_2[5:4]);
            t7_a = $signed(opa_2[7:6]); t7_b = $signed(opb_2[7:6]);
            tp[0] = t0_a*t0_b; tp[1] = t1_a*t1_b;
            tp[2] = t2_a*t2_b; tp[3] = t3_a*t3_b;
            tp2[0] = t4_a*t4_b; tp2[1] = t5_a*t5_b;
            tp2[2] = t6_a*t6_b; tp2[3] = t7_a*t7_b;
            return (tp[0]+tp2[0]) + (tp[1]+tp2[1])
                 + (tp[2]+tp2[2]) + (tp[3]+tp2[3]);
        end
        default: return 0;
        endcase
    endfunction

    // ---- LLC 顶层参考模型：64 lane * mult_int16 → MAC 累加 ----
    function automatic bit signed [41:0] llc_ref_model(
        input bit [1023:0] opa, opb, mask,
        input bit [2:0]    mode
    );
        bit signed [41:0] sum = 0;
        for (int i = 0; i < 64; i++) begin
            bit [15:0] oa = opa[i*16+:16];
            bit [15:0] ob = opb[i*16+:16];
            if (!mask[i]) continue;
            sum += mult_int16_ref_lane(oa[15:8], oa[7:0], ob[15:8], ob[7:0], mode);
        end
        return sum;
    endfunction

endpackage
EOF
```

### 5.5 Step 3~5 — Agent 实现

#### OPA Agent（`agent/llc_opa_agent.sv`）

```bash
cat > agent/llc_opa_agent.sv << 'EOF'
class llc_opa_driver extends uvm_driver #(llc_in_tr);
    `uvm_component_utils(llc_opa_driver)

    virtual llc_if vif;

    function void build_phase(uvm_phase phase);
        super.build_phase(phase);
        if (!uvm_config_db #(virtual llc_if)::get(this, "", "llc_vif", vif))
            `uvm_fatal("NOVIF", "Virtual interface not found")
    endfunction

    task run_phase(uvm_phase phase);
        vif.drv_cb.rstn <= 1'b1;
        vif.drv_cb.vldi <= 1'b0;
        vif.drv_cb.opa  <= '0;
        vif.drv_cb.opb  <= '0;
        vif.drv_cb.mask <= '0;

        // 复位
        repeat (2) @(vif.drv_cb);
        vif.drv_cb.rstn <= 1'b0;
        repeat (2) @(vif.drv_cb);
        vif.drv_cb.rstn <= 1'b1;
        repeat (2) @(vif.drv_cb);

        forever begin
            seq_item_port.get_next_item(req);
            // 驱动数据
            vif.drv_cb.opa  <= req.opa;
            vif.drv_cb.opb  <= req.opb;
            vif.drv_cb.mask <= req.mask;
            vif.drv_cb.vldi <= 1'b1;
            @(vif.drv_cb);
            vif.drv_cb.vldi <= 1'b0;
            seq_item_port.item_done();
        end
    endtask
endclass

class llc_opa_monitor extends uvm_monitor;
    `uvm_component_utils(llc_opa_monitor)

    virtual llc_if vif;
    uvm_analysis_port #(llc_in_tr) item_collected_port;

    function void build_phase(uvm_phase phase);
        super.build_phase(phase);
        item_collected_port = new("item_collected_port", this);
        if (!uvm_config_db #(virtual llc_if)::get(this, "", "llc_vif", vif))
            `uvm_fatal("NOVIF", "Virtual interface not found")
    endfunction

    task run_phase(uvm_phase phase);
        forever begin
            @(vif.mon_cb);
            if (vif.mon_cb.vldi) begin
                llc_in_tr tr = llc_in_tr::type_id::create("tr");
                tr.opa  = vif.mon_cb.opa;
                tr.opb  = vif.mon_cb.opb;
                tr.mask = vif.mon_cb.mask;
                tr.mode = vif.mon_cb.mode;
                tr.vldi = 1'b1;
                item_collected_port.write(tr);
            end
        end
    endtask
endclass

class llc_opa_agent extends uvm_agent;
    `uvm_component_utils(llc_opa_agent)

    llc_opa_driver    driver;
    llc_opa_monitor   monitor;
    uvm_sequencer #(llc_in_tr) sequencer;

    function void build_phase(uvm_phase phase);
        super.build_phase(phase);
        monitor   = llc_opa_monitor::type_id::create("monitor", this);
        driver    = llc_opa_driver::type_id::create("driver", this);
        sequencer = uvm_sequencer #(llc_in_tr)::type_id::create("sequencer", this);
    endfunction

    function void connect_phase(uvm_phase phase);
        super.connect_phase(phase);
        driver.seq_item_port.connect(sequencer.seq_item_export);
    endfunction
endclass
EOF

class llc_cfg_agent extends uvm_agent;
    `uvm_component_utils(llc_cfg_agent)
    llc_cfg_driver    driver;
    llc_cfg_monitor   monitor;
    uvm_sequencer #(llc_in_tr) sequencer;

    function void build_phase(uvm_phase phase);
        super.build_phase(phase);
        monitor   = llc_cfg_monitor::type_id::create("monitor", this);
        driver    = llc_cfg_driver::type_id::create("driver", this);
        sequencer = uvm_sequencer #(llc_in_tr)::type_id::create("sequencer", this);
    endfunction

    function void connect_phase(uvm_phase phase);
        super.connect_phase(phase);
        driver.seq_item_port.connect(sequencer.seq_item_export);
    endfunction
endclass
```

#### Mode Agent（`agent/llc_cfg_agent.sv`）

```bash
cat > agent/llc_cfg_agent.sv << 'EOF'
// Mode driver：在驱动 opa/opb 前提前 1 拍写入 mode_reg
// 由于 mode_reg 存储在 DUT 中，需要与数据同步
class llc_cfg_driver extends uvm_driver #(llc_in_tr);
    `uvm_component_utils(llc_cfg_driver)
    virtual llc_if vif;

    function void build_phase(uvm_phase phase);
        super.build_phase(phase);
        if (!uvm_config_db #(virtual llc_if)::get(this, "", "llc_vif", vif))
            `uvm_fatal("NOVIF", "Virtual interface not found")
    endfunction

    task run_phase(uvm_phase phase);
        forever begin
            seq_item_port.get_next_item(req);
            // mode 需要在 vldi 前 1 拍稳定
            @(vif.drv_cb);
            vif.drv_cb.mode <= req.mode;
            seq_item_port.item_done();
            // mode 持续有效，直到下一个 req 覆盖
            // 注意：cfg_agent 和 opa_agent 需要通过 virtual_sequencer 同步
        end
    endtask
endclass

class llc_cfg_monitor extends uvm_monitor;
    `uvm_component_utils(llc_cfg_monitor)
    virtual llc_if vif;
    uvm_analysis_port #(llc_in_tr) item_collected_port;

    function void build_phase(uvm_phase phase);
        super.build_phase(phase);
        item_collected_port = new("item_collected_port", this);
        if (!uvm_config_db #(virtual llc_if)::get(this, "", "llc_vif", vif))
            `uvm_fatal("NOVIF", "Virtual interface not found")
    endfunction

    task run_phase(uvm_phase phase);
        // mode 变化时采样（简化处理：与 opa monitor 同步采样）
        forever begin
            @(vif.mon_cb);
            if (vif.mon_cb.vldi) begin
                llc_in_tr tr = llc_in_tr::type_id::create("tr");
                tr.mode = vif.mon_cb.mode;
                item_collected_port.write(tr);
            end
        end
    endtask
endclass
EOF

class llc_cfg_agent extends uvm_agent;
    `uvm_component_utils(llc_cfg_agent)
    llc_cfg_driver    driver;
    llc_cfg_monitor   monitor;
    uvm_sequencer #(llc_in_tr) sequencer;

    function void build_phase(uvm_phase phase);
        super.build_phase(phase);
        monitor   = llc_cfg_monitor::type_id::create("monitor", this);
        driver    = llc_cfg_driver::type_id::create("driver", this);
        sequencer = uvm_sequencer #(llc_in_tr)::type_id::create("sequencer", this);
    endfunction

    function void connect_phase(uvm_phase phase);
        super.connect_phase(phase);
        driver.seq_item_port.connect(sequencer.seq_item_export);
    endfunction
endclass
```

#### Out Agent（`agent/llc_out_agent.sv`）

```bash
cat > agent/llc_out_agent.sv << 'EOF'
class llc_out_monitor extends uvm_monitor;
    `uvm_component_utils(llc_out_monitor)
    virtual llc_if vif;
    uvm_analysis_port #(llc_out_tr) item_collected_port;

    function void build_phase(uvm_phase phase);
        super.build_phase(phase);
        item_collected_port = new("item_collected_port", this);
        if (!uvm_config_db #(virtual llc_if)::get(this, "", "llc_vif", vif))
            `uvm_fatal("NOVIF", "Virtual interface not found")
    endfunction

    task run_phase(uvm_phase phase);
        forever begin
            @(vif.mon_cb);
            if (vif.mon_cb.vldo) begin
                llc_out_tr tr = llc_out_tr::type_id::create("tr");
                tr.vldo  = vif.mon_cb.vldo;
                tr.out   = vif.mon_cb.out;
                tr.cycle = $time;
                item_collected_port.write(tr);
            end
        end
    endtask
endclass

class llc_out_agent extends uvm_agent;
    `uvm_component_utils(llc_out_agent)
    llc_out_monitor monitor;

    function void build_phase(uvm_phase phase);
        super.build_phase(phase);
        monitor = llc_out_monitor::type_id::create("monitor", this);
    endfunction
endclass
EOF

class llc_cfg_agent extends uvm_agent;
    `uvm_component_utils(llc_cfg_agent)
    llc_cfg_driver    driver;
    llc_cfg_monitor   monitor;
    uvm_sequencer #(llc_in_tr) sequencer;

    function void build_phase(uvm_phase phase);
        super.build_phase(phase);
        monitor   = llc_cfg_monitor::type_id::create("monitor", this);
        driver    = llc_cfg_driver::type_id::create("driver", this);
        sequencer = uvm_sequencer #(llc_in_tr)::type_id::create("sequencer", this);
    endfunction

    function void connect_phase(uvm_phase phase);
        super.connect_phase(phase);
        driver.seq_item_port.connect(sequencer.seq_item_export);
    endfunction
endclass
```

### 5.6 Step 6 — Scoreboard（`env/llc_scoreboard.sv`）

```bash
cat > env/llc_scoreboard.sv << 'EOF'
class llc_scoreboard extends uvm_scoreboard;
    `uvm_component_utils(llc_scoreboard)

    uvm_analysis_imp_in_op  #(llc_in_tr,  llc_scoreboard) in_op_imp;
    uvm_analysis_imp_in_out #(llc_out_tr, llc_scoreboard) in_out_imp;

    // FIFO 对齐 3 拍流水延迟
    bit signed [41:0] exp_q[$];
    int unsigned pipeline_depth = 3;

    int total_checked;
    int total_passed;
    int total_failed;

    function new(string name, uvm_component parent);
        super.new(name, parent);
    endfunction

    function void build_phase(uvm_phase phase);
        super.build_phase(phase);
        in_op_imp  = new("in_op_imp", this);
        in_out_imp = new("in_out_imp", this);
    endfunction

    function void write_in_op(llc_in_tr tr);
        bit signed [41:0] expected;
        expected = llc_ref_model(tr.opa, tr.opb, tr.mask, tr.mode);
        exp_q.push_back(expected);
        `uvm_info("SCBD", $sformatf(
            "IN:  opa=0x%0h opb=0x%0h mask=0x%0h mode=%0d exp=0x%0h",
            tr.opa, tr.opb, tr.mask, tr.mode, expected), UVM_HIGH)
    endfunction

    function void write_in_out(llc_out_tr tr);
        bit signed [41:0] expected;
        total_checked++;

        if (exp_q.size() == 0) begin
            `uvm_error("SCBD", $sformatf(
                "OUT without matching IN: out=0x%0h cycle=%0d",
                tr.out, tr.cycle))
            total_failed++;
            return;
        end

        expected = exp_q.pop_front();

        if (tr.out !== expected) begin
            `uvm_error("SCBD", $sformatf(
                "MISMATCH: got=0x%0h (%-0d) expected=0x%0h (%-0d) cycle=%0d",
                tr.out, $signed(tr.out),
                expected, $signed(expected), tr.cycle))
            total_failed++;
        end else begin
            `uvm_info("SCBD", $sformatf(
                "PASS: out=0x%0h (%-0d) cycle=%0d",
                tr.out, $signed(tr.out), tr.cycle), UVM_HIGH)
            total_passed++;
        end
    endfunction

    function void report_phase(uvm_phase phase);
        `uvm_info("SCBD", $sformatf(
            "\n==================================================\n\
             SCOREBOARD REPORT:\n\
             Total checked : %0d\n\
             Total passed  : %0d\n\
             Total failed  : %0d\n\
             FIFO remaining : %0d\n\
             ==================================================",
            total_checked, total_passed, total_failed, exp_q.size()), UVM_NONE)
    endfunction
endclass
EOF

class llc_cfg_agent extends uvm_agent;
    `uvm_component_utils(llc_cfg_agent)
    llc_cfg_driver    driver;
    llc_cfg_monitor   monitor;
    uvm_sequencer #(llc_in_tr) sequencer;

    function void build_phase(uvm_phase phase);
        super.build_phase(phase);
        monitor   = llc_cfg_monitor::type_id::create("monitor", this);
        driver    = llc_cfg_driver::type_id::create("driver", this);
        sequencer = uvm_sequencer #(llc_in_tr)::type_id::create("sequencer", this);
    endfunction

    function void connect_phase(uvm_phase phase);
        super.connect_phase(phase);
        driver.seq_item_port.connect(sequencer.seq_item_export);
    endfunction
endclass
```

### 5.7 Step 8 — Env（`env/llc_env.sv`）

```bash
cat > env/llc_env.sv << 'EOF'
class llc_env extends uvm_env;
    `uvm_component_utils(llc_env)

    llc_opa_agent    opa_agent;
    llc_cfg_agent    cfg_agent;
    llc_out_agent    out_agent;
    llc_scoreboard   scoreboard;

    function new(string name, uvm_component parent);
        super.new(name, parent);
    endfunction

    function void build_phase(uvm_phase phase);
        super.build_phase(phase);
        opa_agent  = llc_opa_agent::type_id::create("opa_agent", this);
        cfg_agent  = llc_cfg_agent::type_id::create("cfg_agent", this);
        out_agent  = llc_out_agent::type_id::create("out_agent", this);
        scoreboard = llc_scoreboard::type_id::create("scoreboard", this);
    endfunction

    function void connect_phase(uvm_phase phase);
        super.connect_phase(phase);
        // opa monitor → scoreboard
        opa_agent.monitor.item_collected_port.connect(
            scoreboard.in_op_imp);
        // out monitor → scoreboard
        out_agent.monitor.item_collected_port.connect(
            scoreboard.in_out_imp);
    endfunction
endclass
EOF

class llc_cfg_agent extends uvm_agent;
    `uvm_component_utils(llc_cfg_agent)
    llc_cfg_driver    driver;
    llc_cfg_monitor   monitor;
    uvm_sequencer #(llc_in_tr) sequencer;

    function void build_phase(uvm_phase phase);
        super.build_phase(phase);
        monitor   = llc_cfg_monitor::type_id::create("monitor", this);
        driver    = llc_cfg_driver::type_id::create("driver", this);
        sequencer = uvm_sequencer #(llc_in_tr)::type_id::create("sequencer", this);
    endfunction

    function void connect_phase(uvm_phase phase);
        super.connect_phase(phase);
        driver.seq_item_port.connect(sequencer.seq_item_export);
    endfunction
endclass
```

### 5.8 Step 9~12 — Sequence 和 Test

#### Base Sequence（`seq/llc_base_seq.sv`）

```bash
cat > seq/llc_base_seq.sv << 'EOF'
class llc_base_seq extends uvm_sequence #(llc_in_tr);
    `uvm_object_utils(llc_base_seq)
    int num_tests = 100;

    function new(string name = "llc_base_seq");
        super.new(name);
    endfunction
endclass
EOF

class llc_cfg_agent extends uvm_agent;
    `uvm_component_utils(llc_cfg_agent)
    llc_cfg_driver    driver;
    llc_cfg_monitor   monitor;
    uvm_sequencer #(llc_in_tr) sequencer;

    function void build_phase(uvm_phase phase);
        super.build_phase(phase);
        monitor   = llc_cfg_monitor::type_id::create("monitor", this);
        driver    = llc_cfg_driver::type_id::create("driver", this);
        sequencer = uvm_sequencer #(llc_in_tr)::type_id::create("sequencer", this);
    endfunction

    function void connect_phase(uvm_phase phase);
        super.connect_phase(phase);
        driver.seq_item_port.connect(sequencer.seq_item_export);
    endfunction
endclass
```

#### INT16 Sequence（`seq/llc_int16_seq.sv`）

```bash
cat > seq/llc_int16_seq.sv << 'EOF'
class llc_int16_seq extends llc_base_seq;
    `uvm_object_utils(llc_int16_seq)

    task body();
        llc_in_tr tr;
        for (int i = 0; i < num_tests; i++) begin
            tr = llc_in_tr::type_id::create("tr");
            // 约束随机化：mode=0（INT16），mask 全 1
            if (!tr.randomize() with { mode == 0; mask == {1024{1'b1}}; })
                `uvm_error("SEQ", "Randomization failed")
            start_item(tr);
            finish_item(tr); 
        end
    endtask
endclass
EOF

class llc_cfg_agent extends uvm_agent;
    `uvm_component_utils(llc_cfg_agent)
    llc_cfg_driver    driver;
    llc_cfg_monitor   monitor;
    uvm_sequencer #(llc_in_tr) sequencer;

    function void build_phase(uvm_phase phase);
        super.build_phase(phase);
        monitor   = llc_cfg_monitor::type_id::create("monitor", this);
        driver    = llc_cfg_driver::type_id::create("driver", this);
        sequencer = uvm_sequencer #(llc_in_tr)::type_id::create("sequencer", this);
    endfunction

    function void connect_phase(uvm_phase phase);
        super.connect_phase(phase);
        driver.seq_item_port.connect(sequencer.seq_item_export);
    endfunction
endclass
```

#### Base Test（`test/llc_base_test.sv`）

```bash
cat > test/llc_base_test.sv << 'EOF'
class llc_base_test extends uvm_test;
    `uvm_component_utils(llc_base_test)
    llc_env env;

    function new(string name = "llc_base_test", uvm_component parent = null);
        super.new(name, parent);
    endfunction

    function void build_phase(uvm_phase phase);
        super.build_phase(phase);
        env = llc_env::type_id::create("env", this);
    endfunction
endclass
EOF

class llc_cfg_agent extends uvm_agent;
    `uvm_component_utils(llc_cfg_agent)
    llc_cfg_driver    driver;
    llc_cfg_monitor   monitor;
    uvm_sequencer #(llc_in_tr) sequencer;

    function void build_phase(uvm_phase phase);
        super.build_phase(phase);
        monitor   = llc_cfg_monitor::type_id::create("monitor", this);
        driver    = llc_cfg_driver::type_id::create("driver", this);
        sequencer = uvm_sequencer #(llc_in_tr)::type_id::create("sequencer", this);
    endfunction

    function void connect_phase(uvm_phase phase);
        super.connect_phase(phase);
        driver.seq_item_port.connect(sequencer.seq_item_export);
    endfunction
endclass
```

#### INT16 Test（`test/test_int16_random.sv`）

```bash
cat > test/test_int16_random.sv << 'EOF'
class test_int16_random extends llc_base_test;
    `uvm_component_utils(test_int16_random)

    task run_phase(uvm_phase phase);
        llc_int16_seq seq;

        phase.raise_objection(this);

        seq = llc_int16_seq::type_id::create("seq");
        seq.num_tests = 200;
        `uvm_info("TEST", "Starting INT16 random test (200 iterations)", UVM_NONE)
        seq.start(env.opa_agent.sequencer);

        phase.drop_objection(this);
    endtask
endclass
EOF

class llc_cfg_agent extends uvm_agent;
    `uvm_component_utils(llc_cfg_agent)
    llc_cfg_driver    driver;
    llc_cfg_monitor   monitor;
    uvm_sequencer #(llc_in_tr) sequencer;

    function void build_phase(uvm_phase phase);
        super.build_phase(phase);
        monitor   = llc_cfg_monitor::type_id::create("monitor", this);
        driver    = llc_cfg_driver::type_id::create("driver", this);
        sequencer = uvm_sequencer #(llc_in_tr)::type_id::create("sequencer", this);
    endfunction

    function void connect_phase(uvm_phase phase);
        super.connect_phase(phase);
        driver.seq_item_port.connect(sequencer.seq_item_export);
    endfunction
endclass
```

### 5.9 Step 13 — Testbench Top（`tb/llc_tb_top.sv`）

```bash
cat > tb/llc_tb_top.sv << 'EOF'
`timescale 1ns/10ps

module llc_tb_top;
    import uvm_pkg::*;
    import llc_pkg::*;
    `include "uvm_macros.svh"

    // ---- 时钟 ----
    bit clk;
    always #1.5 clk = ~clk;    // 3ns 周期

    // ---- Interface + DUT 连接 ----
    llc_if llc_vif (.clk(clk));

    rram_1t1r_macro_llc #(
        .IN_WIDTH (IN_WIDTH),
        .DATA_W   (DATA_W)
    ) dut (
        .clk  (clk),
        .rstn (llc_vif.rstn),
        .vldi (llc_vif.vldi),
        .opa  (llc_vif.opa),
        .opb  (llc_vif.opb),
        .mode (llc_vif.mode),
        .mask (llc_vif.mask),
        .vldo (llc_vif.vldo),
        .out  (llc_vif.out)
    );

    // ---- UVM 启动 ----
    initial begin
        // 将 virtual interface 放入 config_db
        uvm_config_db #(virtual llc_if)::set(null, "*", "llc_vif", llc_vif);
        // 波形输出
        $fsdbDumpfile("llc_uvm.fsdb");
        $fsdbDumpvars(0, dut);
        // 启动 UVM，test 名称从命令行 +UVM_TESTNAME 获取
        run_test();
    end
endmodule
EOF
```

### 5.10 Step 14 — 编译和运行脚本

#### VCS 编译脚本（`scripts/run_vcs.sh`）

```bash
cat > scripts/run_vcs.sh << 'EOF'
#!/bin/bash
set -e

# ---- 文件列表 ----
RTL_FILES="\
    ../../src/mult_int16.sv \
    ../../src/rram_1t1r_macro_llc.sv"

UVM_FILES="\
    ../env/llc_if.sv \
    ../env/llc_pkg.sv \
    ../agent/llc_opa_agent.sv \
    ../agent/llc_cfg_agent.sv \
    ../agent/llc_out_agent.sv \
    ../env/llc_scoreboard.sv \
    ../env/llc_env.sv \
    ../seq/llc_base_seq.sv \
    ../seq/llc_int16_seq.sv \
    ../test/llc_base_test.sv \
    ../test/test_int16_random.sv \
    ../tb/llc_tb_top.sv"

# ---- 清理 ----
rm -rf simv csrc ucli.key DVEfiles *.fsdb *.log *.vpd

# ---- 编译 ----
vcs -sverilog -full64 \
    -ntb_opts uvm-1.2 \
    +v2k \
    -timescale=1ns/10ps \
    -debug_access+all \
    -lca \
    -kdb \
    +define+UVM_NO_DPI \
    -top llc_tb_top \
    -o simv \
    $RTL_FILES $UVM_FILES

# ---- 仿真 ----
./simv +UVM_TESTNAME=test_int16_random \
    +UVM_VERBOSITY=UVM_MEDIUM \
    -l sim.log

# ---- 检查结果 ----
grep -E "UVM_ERROR|UVM_FATAL|PASS|FAIL|SCOREBOARD REPORT" sim.log
EOF

chmod +x scripts/run_vcs.sh
```

#### 回归脚本（`scripts/run_regression.sh`）

```bash
cat > scripts/run_regression.sh << 'EOF'
#!/bin/bash
set -e

TESTS=(
    "test_int16_random"
    "test_int8_random"
    "test_int4_random"
    "test_ternary_random"
    "test_mask"
    "test_pipeline"
    "test_reset"
)

PASS_COUNT=0
FAIL_COUNT=0

for t in "${TESTS[@]}"; do
    echo "========== Running: $t =========="
    if ./simv +UVM_TESTNAME=$t \
        +UVM_VERBOSITY=UVM_LOW \
        -l logs/${t}.log; then
        echo "PASS: $t"
        ((PASS_COUNT++))
    else
        echo "FAIL: $t"
        ((FAIL_COUNT++))
    fi
done

echo ""
echo "============================================"
echo "Regression Summary: $PASS_COUNT PASS, $FAIL_COUNT FAIL"
echo "============================================"
EOF

chmod +x scripts/run_regression.sh
```

### 5.11 运行验证

```bash
# 1. 创建目录结构
cd /data/kzhang/workplace/1t1r_macro/verif/llc_uvm
mkdir -p env agent seq test tb scripts logs

# 2. 按 5.3~5.10 顺序创建所有文件（或批量 cat 创建）

# 3. 编译
cd scripts
./run_vcs.sh

# 4. 运行单个 test
./simv +UVM_TESTNAME=test_int16_random +UVM_VERBOSITY=UVM_MEDIUM -l sim.log

# 5. 运行回归
./run_regression.sh

# 6. 查看波形
# verdi -ssf llc_uvm.fsdb &
```

---

## 6. 建议的验证策略

### 阶段 1：保持现有 Testbench（当前阶段）

- 用于 RTL 快速迭代和单点调试
- 波形直接可看，不需要搭建 UVM 环境

### 阶段 2：UVM 迁移（LLC 功能冻结后）

- 从工具生成 UVM 框架（VCS `-uvm` 或 Questa `uvm_gen`）
- 手写 interface + transaction + scoreboard + sequence
- 参考模型用 DPI-C 调用 C 参考模型（可选，也可纯 SV 实现）

### 阶段 3：全链路验证（LLC + Subarray + 扫描链）

- 复用 LLC 的 UVM 环境，增加 subarray agent 和 scan_chain agent
- 需要处理扫描链串行配置的时序（最长 1160 拍 × 多组）
- Subarray 行为模型提供 nFINISH 握手，UVM 需要等 nFINISH 后再检查

### 阶段 4：覆盖率驱动的回归

- 定义 functional coverage：mode × mask_density × opa_range × opb_range
- 定义 cross coverage：保证所有 mode 组合被覆盖
- 定义 toggle coverage（可选，通常综合工具做）
