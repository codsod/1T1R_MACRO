# 从 1t1r_macro 项目中学到什么

> 目标项目：芯片中小模型推理和通信模块的共享算子设计
> 参考项目：1t1r_macro（RRAM CIM macro，导师提供）

---

## 一、你的目标 vs 这个项目的相关度

| 你的项目需求 | 1t1r_macro 相关度 | 说明 |
|-------------|-------------------|------|
| **共享算子设计** | ⭐⭐⭐⭐⭐ 直接相关 | 同一硬件支持 5 种精度（INT16/INT8/INT4/BF16/Ternary），就是共享算子的范本 |
| **模型推理（MAC）** | ⭐⭐⭐⭐⭐ 核心 | 乘法器 + 加法树 = 推理的最核心算子 |
| **通信模块** | ⭐ 未涉及 | 该项目不包含总线接口或跨时钟域通信 |

---

## 二、需要提取的 3 个核心设计模式

### 2.1 多精度 MAC = 共享算子的最佳范本

**文件**：`src/mult_int16.sv`

同一个乘法器硬件通过 `mode` 信号动态切换不同精度：

```verilog
if(mode == 'd0)  // INT16: 4个8bit乘法 + 移位组合（Booth分解）
else if(mode == 'd1) // INT8: 2路8bit直接乘
else if(mode == 'd2) // INT4: 4路4bit直接乘
else if(mode == 'd4) // Ternary: 8路{-1,0,+1}低精度乘
```

**对你后续设计的启发**：你的共享算子也需要这种 mode 切换架构——比如一个运算核同时支持 Conv / FIR滤波 / FFT蝶形，运行时通过配置切换。

### 2.2 加法树 = 聚合运算的标准骨架

**文件**：`src/rram_1t1r_macro_llc.sv:104-136`

三级流水线加法树：

```
64路乘法结果 → 16组4输入加法 → 4组4输入加法 → 1个最终累加
    (第1级)         (第2级)         (第3级)
```

每级插入寄存器，实现高吞吐流水线。

**对你后续设计的启发**：你的共享算子需要将这种加法树泛化为**可配置的归约网络**——不仅是加法，还可能是 max pooling、累加通信统计量等。结构相同，运算单元可替换。

### 2.3 generate + parameter = 可扩展架构

**文件**：`src/rram_1t1r_macro_llc.sv:82-100`

```verilog
generate
    for (i = 0; i < 64; i = i+1) begin
        mult_int16 #(.DATA_W(DATA_W)) mult_int16 ( ... );
    end
endgenerate
```

**对你后续设计的启发**：共享算子的基本方法论——设计一个基本运算单元，通过参数化 + generate 例化 N 个，配不同的连接拓扑（树形/链形/网状）。

---

## 三、这个项目没教但你要自学的关键技能

### 3.1 通信相关

| 技能 | 为什么需要 | 学习方向 |
|------|-----------|---------|
| **AXI-Stream / AXI-Lite 总线接口** | 算子和 CPU/DMA 之间传输数据 | 开源 AXI slave 模板 |
| **异步 FIFO / 跨时钟域同步** | 推理核和通信模块通常在不同时钟域 | syncff、格雷码指针 |
| **valid/ready 握手协议** | 模块间数据流控制 | 该项目已有 vldi/vldo 简化版，需升级 |

### 3.2 共享算子的泛化设计

1T1R macro 只做了"不同精度的 MAC"。你的共享算子要扩展：

```
共享算子 = 同一硬件单元，运行时切换到不同功能
         ├── 推理模式：Conv / Pooling / FC
         └── 通信模式：FIR滤波 / FFT蝶形 / CRC校验
```

设计要点：

| 组件 | 作用 |
|------|------|
| **输入 MUX** | 不同功能的数据来自不同源（推理从 SRAM，通信从接口） |
| **输出 DEMUX** | 结果送往不同目的地 |
| **控制寄存器** | 配置当前 mode、数据路径、精度（学 scan-chain 思想） |

---

## 四、建议的学习路径

### Step 1：跑通仿真
```bash
cd /home/kzhang/workplace/1t1r_macro/verif/macro_llc
bash run_vcs.sh
```
理解点：vldi/vldo 握手时序、流水线延迟对齐（testbench 中的 3 拍黄金参考值对齐）

### Step 2：画架构时序图
把 `mult_int16` + 加法树的**精确流水线级数**画出来，标注每级 latency：
- mult_int16: 组合逻辑（乘法器）→ 1 级寄存器
- rram_1t1r_macro_llc: 64x mult_int16 → 3 级加法树 → 输出
- 总 latency: 4 个时钟周期（从 vldi 到 vldo）

### Step 3：在 1T1R 基础上扩展功能（练习）
给 `mult_int16` 加一个 FIR mode（通信滤波），练习共享算子的 mode 扩展流程：
1. 在 mode 枚举中新增 FIR 模式
2. 添加 FIR 系数寄存器
3. 修改数据通路 MUX

### Step 4：增加总线接口（自学部分）
- 调研 AXI-Stream 或 AXI-Lite 规范
- 给 `rram_macro_llc_top` 增加寄存器配置接口（替代 scan-chain）
- 实现 valid/ready 握手替代简单的 vldi/vldo

---

## 五、可以直接复用的部分

| 资产 | 路径 | 用途 |
|------|------|------|
| 综合脚本 | `syn/*/*.tcl` | 改 PDK 路径后直接复用 |
| 仿真脚本 | `verif/*/run_vcs.sh` | 不需要改 |
| testbench 模板 | `verif/*/testbench.sv` | golden model 对比方法 |
| 多精度 MAC 架构 | `src/mult_int16.sv` | 共享算子核心模板 |
| 扫描链配置方法 | `src/rram_macro_llc_top.sv` | CIM/模拟芯片通用结构 |
| 加法树流水线 | `src/rram_1t1r_macro_llc.sv` | 聚合运算的标准骨架 |
