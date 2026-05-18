# 1t1r_macro 本地化部署修改清单

> 项目来源：导师 `taoyaoyu`
> 部署路径：`/home/kzhang/workplace/1t1r_macro/`

---

## 一、必须修改的硬编码路径

### 1.1 syn.tcl 中的 PRJ_DIR / REF_DIR（6个文件）

所有 `syn.tcl` 中都硬编码了导师的家目录路径 `/users/taoyaoyu/`。将 **`PRJ_DIR`** 改为本机部署路径，**`REF_DIR`** 相应地改为各子目录自身的路径。

| # | 文件 | 需修改的行 |
|---|------|-----------|
| 1 | `syn/macro/syn.tcl` | L4: `PRJ_DIR`, L5: `REF_DIR` |
| 2 | `syn/macro_llc/syn.tcl` | L4: `PRJ_DIR`, L5: `REF_DIR` |
| 3 | `syn/rram_macro_top/syn.tcl` | L4: `PRJ_DIR`, L5: `REF_DIR` |
| 4 | `syn/rram_macro_llc_top/syn.tcl` | L4: `PRJ_DIR`, L5: `REF_DIR` |
| 5 | `syn/rram_macro_llc_top_with_subarray/syn.tcl` | L4: `PRJ_DIR`, L5: `REF_DIR` |
| 6 | `syn/mult_int16/syn.tcl` | L4: `PRJ_DIR`, L5: `REF_DIR` |

**示例修改**（以 `syn/macro/syn.tcl` 为例）：

```tcl
# 原值
set PRJ_DIR     "/users/taoyaoyu/workspace/1t1r_macro/"
set REF_DIR     "/users/taoyaoyu/workspace/1t1r_macro/syn/macro_llc"

# 改为
set PRJ_DIR     "/home/kzhang/workplace/1t1r_macro/"
set REF_DIR     "/home/kzhang/workplace/1t1r_macro/syn/macro_llc"
```

---

### 1.2 load_libs.tcl 中的 rram_lib 路径（3个文件）

`LIBRARY_PATH` 中包含了指向导师家目录的 rram_lib，需改为本地路径。

| # | 文件 | 修改内容 |
|---|------|----------|
| 1 | `syn/rram_macro_top/load_libs.tcl` L11 | `/users/taoyaoyu/workspace/1t1r_macro/rram_lib` → `/home/kzhang/workplace/1t1r_macro/rram_lib` |
| 2 | `syn/rram_macro_llc_top/load_libs.tcl` L11 | 同上 |
| 3 | `syn/rram_macro_llc_top_with_subarray/load_libs.tcl` L11 | 同上 |

**不涉及此问题的文件**（`LIBRARY_PATH` 中不含 `/users/taoyaoyu/`）：
- `syn/macro/load_libs.tcl`
- `syn/macro_llc/load_libs.tcl`
- `syn/mult_int16/load_libs.tcl`

---

### 1.3 dut.f 中的绝对路径（6个文件）

`dut.f` 是 DC 综合的 Verilog 文件列表，全部使用了绝对路径，必须改为本机路径或相对路径。

| # | 文件 | 现有内容 |
|---|------|----------|
| 1 | `syn/macro/dut.f` | `/users/taoyaoyu/workspace/1t1r_macro/src/mult_int16.sv` 等 |
| 2 | `syn/macro_llc/dut.f` | 同上 |
| 3 | `syn/rram_macro_top/dut.f` | 同上（含 `rram_macro_top_modified.sv` + 前一次综合输出） |
| 4 | `syn/rram_macro_llc_top/dut.f` | 同上（含 `rram_macro_llc_top.sv`） |
| 5 | `syn/rram_macro_llc_top_with_subarray/dut.f` | 同上（含 `rram_macro_llc_top_with_subarray.sv`） |
| 6 | `syn/mult_int16/dut.f` | `/users/taoyaoyu/workspace/1t1r_macro/src/mult_int16.sv` |

**推荐方案**：改为相对路径，例如：

```
# syn/macro/dut.f 示例
../../src/mult_int16.sv
../../src/rram_1t1r_macro_llc.sv
```

**注意**：`syn/rram_macro_top/dut.f` 还依赖前一次综合的输出文件（`syn/rram_macro_llc_top_with_subarray/result/rram_macro_llc_top.syn.v`），需要先综合 rram_macro_llc_top_with_subarray 模块。

---

## 二、需要确认的外部依赖

### 2.1 PDK 标准单元库

所有 `load_libs.tcl` 都引用了同一个 PDK 库路径：

```
/share/ICEM/ICEM_28nm_v6.0/CL028HKN_PDK_v1d2e/IP/STD/7T/IC28HKN_HD_RVT_L30_V0p1c/nldm
```

目标库文件：`ic28hkn_hd_rvt_l30_tt_0p9_25c_basic.db`

**务必确认**：
- 该路径在你的服务器上是否存在
- 如果 PDK 安装路径不同，需要修改所有 6 个 `load_libs.tcl` 中的 `LIBRARY_PATH`

### 2.2 额外库（部分目标使用）

| 文件 | 额外依赖 |
|------|----------|
| `syn/rram_macro_top/load_libs.tcl` | GPIO 库 `C28HKIOS2P5OD3P3R00_V00_TTG0P9V2P5V25c.db` |
| `syn/rram_macro_llc_top/load_libs.tcl` | 模拟子阵列库 `sub_array_analog_total.db`（位于 `rram_lib/` 下） |
| `syn/rram_macro_llc_top_with_subarray/load_libs.tcl` | 同上 |

### 2.3 EDA 工具

| 工具 | 用途 | 相关脚本/配置 |
|------|------|--------------|
| Synopsys **DC** (`dc_shell-xg-t`) | 逻辑综合 | `syn/*/run_syn` → `syn.tcl` |
| Synopsys **VCS** (`vcs`) | 仿真验证 | `verif/*/run_vcs.sh` |
| Synopsys **Verdi** (`verdi`) | 波形调试 | `verif/novas.rc`, `verif/novas.conf` |

---

## 三、无需修改但建议了解的明细

### 3.1 综合参数差异（syn.tcl CLK_PERIOD）

不同子模块使用了不同的时钟周期设定，部署后可根据实际时序需求调整：

| syn.tcl | CLK_PERIOD | 说明 |
|---------|-----------|------|
| `syn/macro/syn.tcl` | 4.0 ns | rram_1t1r_macro_llc |
| `syn/macro_llc/syn.tcl` | 2.0 ns | rram_1t1r_macro_llc |
| `syn/rram_macro_top/syn.tcl` | 2.0 ns | rram_macro_top |
| `syn/rram_macro_llc_top/syn.tcl` | 5.0 ns | rram_macro_llc_top |
| `syn/rram_macro_llc_top_with_subarray/syn.tcl` | 5.0 ns | 含 subarray |
| `syn/mult_int16/syn.tcl` | 3.0 ns | mult_int16 独立模块 |

### 3.2 综合流程依赖关系

```
mult_int16            ← 可独立综合
rram_1t1r_macro_llc   ← 依赖 mult_int16（实例化）
rram_macro_llc_top    ← 依赖 rram_1t1r_macro_llc
rram_macro_llc_top_with_subarray ← 依赖上者 + rram_lib/sub_array_analog_total
rram_macro_top        ← 依赖 rram_macro_llc_top_with_subarray 的综合结果
```

### 3.3 仿真验证目录

当前 `verif/` 下有 4 个验证目录，但**缺少 `mult_int16` 专用的 run_vcs.sh**（其 `verif/mult_int16/` 目录内无 `run_vcs.sh`，仅有日志/波形文件）。

---

## 四、部署步骤建议

### Step 1：清理生成文件
```bash
# 综合产物
rm -rf syn/*/WORK_autoread syn/*/result syn/*/report syn/*/work
rm -f syn/*/default.svf syn/*/command.log syn/*/dc_run.log*

# 仿真产物
rm -rf verif/*/simv verif/*/simv.daidir verif/*/verdiLog
rm -f verif/*/TESTBENCH.fsdb verif/*/TESTBENCH.vcd verif/*/inter.fsdb
rm -f verif/*/novas_dump.log verif/*/run_vcs.log
```

### Step 2：修改路径
批量替换 `/users/taoyaoyu/workspace/1t1r_macro` → `/data/kzhang/workplace/1t1r_macro`。

### Step 3：确认 PDK 路径
```bash
ls /share/ICEM/ICEM_28nm_v6.0/CL028HKN_PDK_v1d2e/IP/STD/7T/IC28HKN_HD_RVT_L30_V0p1c/nldm/
```
确认 `ic28hkn_hd_rvt_l30_tt_0p9_25c_basic.db` 存在。

### Step 4：验证 EDA 工具
```bash
which dc_shell-xg-t
which vcs
which verdi
```

### Step 5：跑验证确认功能
```bash
# 例：从 simv 目录运行 VCS 仿真
cd verif/macro_llc && bash run_vcs.sh
```
