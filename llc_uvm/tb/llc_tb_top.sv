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