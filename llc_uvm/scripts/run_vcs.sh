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
    ../seq/llc_base_test.sv \
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