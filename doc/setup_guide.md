# 1T1R Macro 开发环境打通指南

## 环境概况

| 项目 | 详情 |
|------|------|
| 工具链 | Synopsys VCS（仿真）+ Verdi（波形查看）+ Design Compiler（综合） |
| 工艺库 | SMIC 28nm（IC28HKN） |
| 设计语言 | SystemVerilog |

### EDA 工具版本及路径

| 工具 | 版本 | 路径 |
|------|------|------|
| VCS | O-2018.09-SP2 | `/data/eda/synopsys/vcs/O-2018.09-SP2/bin/vcs` |
| Verdi | O-2018.09-SP2 | `/data/eda/synopsys/verdi/Verdi_O-2018.09-SP2/bin/verdi` |
| DC | O-2018.06-SP5-5 | `/data/eda/synopsys/syn/O-2018.06-SP5-5/bin/dc_shell-xg-t` |

---

## 一、路径修正（必须先做）

原项目硬编码了 `/users/taoyaoyu/workspace/1t1r_macro/`，需要批量替换为当前项目路径。

### 1.1 修正 syn.tcl 中的项目路径（共 5 个文件）

```bash
cd /data/kzhang/workplace/1t1r_macro

# 批量替换所有 syn.tcl 中的 PRJ_DIR 和 REF_DIR
for f in syn/*/syn.tcl; do
    sed -i 's|/users/taoyaoyu/workspace/1t1r_macro|/data/kzhang/workplace/1t1r_macro|g' "$f"
done
```

### 1.2 修正 dut.f 中的源文件路径（共 6 个文件）

```bash
for f in syn/*/dut.f; do
    sed -i 's|/users/taoyaoyu/workspace/1t1r_macro|/data/kzhang/workplace/1t1r_macro|g' "$f"
done
```

### 1.3 确认修改结果

```bash
grep -r "taoyaoyu" syn/ --include="*.tcl" --include="dut.f"
# 应无任何输出
```

> **注意**：`load_libs.tcl` 中的 PDK 路径 `/share/ICEM/...` 是公共库路径，不需要修改。

---

## 二、验证：仿真环境

仿真脚本使用相对路径，路径修正后可直接运行。

### 2.1 模块验证层次

推荐按从底向上的顺序验证，每次验证通过后再跑上一层：

| 序号 | 模块 | 验证目录 | 说明 |
|------|------|---------|------|
| 1 | `mult_int16` | `verif/mult_int16/` | 乘法器单元，多精度 MAC 核心 |
| 2 | `macro_llc` | `verif/macro_llc/` | 64 路乘法器 + 3 级加法树 |
| 3 | `rram_macro_llc_top` | `verif/rram_macro_llc_top/` | 含子阵列模拟模块的顶层 |
| 4 | `rram_macro_top` | `verif/rram_macro_top/` | 完整 RRAM macro 顶层 |

### 2.2 运行仿真

```bash
# 以 mult_int16 为例
cd /data/kzhang/workplace/1t1r_macro/verif/mult_int16
bash run_vcs.sh
```

### 2.3 成功标志

- 控制台输出中每个 Test 都显示 `Pass`（不应出现 `Fail`）
- 最后打印 `Testbench Ended`
- 生成 `TESTBENCH.fsdb` 波形文件

### 2.4 查看波形（可选）

`verif/` 目录下没有 `dut.f` 文件（那个只在 `syn/` 下面），所以不能直接用 `verdi -f dut.f`。

**Verdi 需要两样东西**：RTL 源文件（用于在源码窗口追踪信号）+ FSDB 波形文件（用于显示波形）。

**方法一：直接传源文件（推荐，最简单）**

```bash
# 在 verif 目录下执行，源文件路径对照 run_vcs.sh 中的内容来写

# mult_int16 模块
cd /data/kzhang/workplace/1t1r_macro/verif/mult_int16
verdi -sv ../../src/mult_int16.sv -ssf TESTBENCH.fsdb &

# macro_llc 模块
cd /data/kzhang/workplace/1t1r_macro/verif/macro_llc
verdi -sv ../../src/mult_int16.sv ../../src/rram_1t1r_macro_llc.sv -ssf TESTBENCH.fsdb &

# rram_macro_llc_top 模块
cd /data/kzhang/workplace/1t1r_macro/verif/rram_macro_llc_top
verdi -sv ../../src/mult_int16.sv ../../src/rram_1t1r_macro_llc.sv ../../src/rram_macro_llc_top_with_subarray.sv ../../rram_lib/sub_array_analog_total.sv -ssf TESTBENCH.fsdb &

# rram_macro_top 模块
cd /data/kzhang/workplace/1t1r_macro/verif/rram_macro_top
verdi -sv ../../src/mult_int16.sv ../../src/rram_1t1r_macro_llc.sv ../../src/rram_macro_llc_top_with_subarray.sv ../../rram_lib/sub_array_analog_total.sv -ssf TESTBENCH.fsdb &
```

**方法二：临时生成 dut.f 再传**

```bash
cd /data/kzhang/workplace/1t1r_macro/verif/macro_llc
# 从 run_vcs.sh 中提取源文件路径，写入临时 dut.f
grep -oP '\.\./\.\./[^ ]+\.sv' run_vcs.sh > dut.f
verdi -sv -f dut.f -ssf TESTBENCH.fsdb &
```

**参数说明**：
| 参数 | 含义 |
|------|------|
| `-sv` | 启用 SystemVerilog 支持 |
| `-ssf TESTBENCH.fsdb` | 加载 FSDB 波形文件 |
| `-f dut.f` | 从文件列表读取源文件路径（每行一个文件） |
| `&` | 后台运行，不阻塞终端 |

> 若提示 license 相关问题，检查 `LM_LICENSE_FILE` 环境变量。

---

## 三、验证：综合环境

### 3.1 综合模块及层次

综合流程除验证模块外，所有 5 个综合目标均需要跑通：

| 序号 | 模块 | 综合目录 | 说明 |
|------|------|---------|------|
| 1 | `mult_int16` | `syn/mult_int16/` | 乘法器（1 个文件） |
| 2 | `macro_llc` | `syn/macro_llc/` | LLC 级（mult_int16 + 加法树） |
| 3 | `rram_macro_llc_top` | `syn/rram_macro_llc_top/` | 含扫描链的 LLC 顶层 |
| 4 | `rram_macro_llc_top_with_subarray` | `syn/rram_macro_llc_top_with_subarray/` | 含子阵列模拟模块 |
| 5 | `rram_macro_top` | `syn/rram_macro_top/` | 完整顶层（使用上一步综合网表） |

### 3.2 运行综合

```bash
# 以 mult_int16 为例
cd /data/kzhang/workplace/1t1r_macro/syn/mult_int16
bash run_syn
```

综合流程会依次执行：
1. 读入 RTL → `elaborate` 建网表
2. `check_design` 检查设计
3. `compile_ultra` 逻辑综合与优化
4. 输出门级网表、SDC、SDF、report

### 3.3 成功标志

1. 无 Error（Warning 通常可接受）
2. `result/` 目录下生成以下产物：

| 产物 | 文件名格式 | 用途 |
|------|-----------|------|
| 门级网表 | `*_svsim.sv` / `*.syn.v` | 后仿真 / 版图 |
| 时序约束 | `*.syn.sdc` | P&R 输入 |
| 延迟反标 | `*.syn.sdf` | 门级仿真 |
| 面积报告 | `report/*.area.rpt` | 面积评估 |
| 时序报告 | `report/*.timing.max.rpt` | setup 时序检查 |
| 功耗报告 | `report/*.power.rpt` | 功耗评估 |

3. 时序报告中的 slack 应为正值（无违例）

---

## 四、常见问题

### 4.1 VCS license 报错

```bash
# 检查 license 状态
lmstat -c $LM_LICENSE_FILE | grep -i vcs
```

若 license 不可用，检查是否需切换 license server 或排队等待。

### 4.2 DC 找不到 PDK 库

`load_libs.tcl` 引用的路径：
```
/share/ICEM/ICEM_28nm_v6.0/CL028HKN_PDK_v1d2e/IP/STD/7T/IC28HKN_HD_RVT_L30_V0p1c/nldm
```

确认该路径是否存在且可读：
```bash
ls /share/ICEM/ICEM_28nm_v6.0/CL028HKN_PDK_v1d2e/IP/STD/7T/IC28HKN_HD_RVT_L30_V0p1c/nldm/
```

### 4.3 仿真编译报 `gcc` 相关错误

VCS 需要 GCC 编译器。确认 gcc 可用：
```bash
which gcc && gcc --version
```

### 4.4 已有仿真产物冲突

部分 verif 目录下已有 `simv`、`simv.daidir` 等旧产物。重新跑仿真时 VCS 会自动覆盖，无需手动清理。若出现问题，删除后重试：
```bash
rm -rf simv simv.daidir csrc TESTBENCH.fsdb
```

### 4.5 Verdi 报 `Can't open display`（X11 图形显示问题）

Verdi 是 GUI 工具，需要 X11 图形环境。这个报错说明当前 SSH 会话没有转发 X11。

**解决方法**：

断开当前 SSH，用 `-X` 或 `-Y` 重新连接：
```bash
ssh -Y kzhang@yaoyu01
```

- `-Y`：信任远程 X11 客户端，性能更好，EDA 工具推荐
- `-X`：更安全但可能较慢

**各平台前置条件**：

| 平台 | 需要安装 |
|------|---------|
| macOS | XQuartz：`brew install xquartz`，重启终端后再 ssh |
| Windows | VcXsrv 或 MobaXterm，启动 X server 后再 ssh |
| Linux | 无需额外安装（本地桌面环境自带 X server） |

重新连接后验证：
```bash
echo $DISPLAY
# 应输出类似 localhost:10.0 或 :0，而不是空
```

---

## 五、一次性验证脚本

将以下脚本保存为 `check_env.sh`，一键检查环境是否就绪：

```bash
#!/bin/bash
echo "=== 检查 EDA 工具 ==="
which vcs       && echo "VCS: OK"      || echo "VCS: MISSING"
which verdi     && echo "Verdi: OK"    || echo "Verdi: MISSING"
which dc_shell-xg-t && echo "DC: OK"   || echo "DC: MISSING"

echo ""
echo "=== 检查 PDK ==="
PDK_PATH="/share/ICEM/ICEM_28nm_v6.0/CL028HKN_PDK_v1d2e/IP/STD/7T/IC28HKN_HD_RVT_L30_V0p1c/nldm"
[ -d "$PDK_PATH" ] && echo "PDK: OK ($PDK_PATH)" || echo "PDK: MISSING ($PDK_PATH)"

echo ""
echo "=== 检查路径替换 ==="
count=$(grep -r "taoyaoyu" syn/ --include="*.tcl" --include="dut.f" 2>/dev/null | wc -l)
[ "$count" -eq 0 ] && echo "路径替换: OK" || echo "路径替换: 还有 $count 处未替换"

echo ""
echo "=== 项目文件结构 ==="
echo "Verif 模块: $(ls -d verif/*/ | wc -l) 个"
echo "Syn 模块:  $(ls -d syn/*/ | wc -l) 个"
echo "RTL 文件:  $(ls src/*.sv | wc -l) 个"
```

---

## 六、打通检查清单

- [ ] **路径修正**：`grep -r "taoyaoyu" syn/` 无输出
- [ ] **仿真 mult_int16**：`cd verif/mult_int16 && bash run_vcs.sh` → 全部 Pass
- [ ] **仿真 macro_llc**：`cd verif/macro_llc && bash run_vcs.sh` → 全部 Pass
- [ ] **仿真 rram_macro_llc_top**：`cd verif/rram_macro_llc_top && bash run_vcs.sh` → 全部 Pass
- [ ] **仿真 rram_macro_top**：`cd verif/rram_macro_top && bash run_vcs.sh` → 全部 Pass
- [ ] **综合 mult_int16**：`cd syn/mult_int16 && bash run_syn` → 无 Error，slack 为正
- [ ] **综合 macro_llc**：`cd syn/macro_llc && bash run_syn` → 无 Error，slack 为正
- [ ] **综合 rram_macro_llc_top**：`cd syn/rram_macro_llc_top && bash run_syn` → 无 Error，slack 为正
- [ ] **综合 rram_macro_llc_top_with_subarray**：`cd syn/rram_macro_llc_top_with_subarray && bash run_syn` → 无 Error，slack 为正
- [ ] **综合 rram_macro_top**：`cd syn/rram_macro_top && bash run_syn` → 无 Error，slack 为正
- [ ] **Verdi 波形**：`cd verif/macro_llc && verdi -sv ../../src/mult_int16.sv ../../src/rram_1t1r_macro_llc.sv -ssf TESTBENCH.fsdb &` → 能打开并查看波形
