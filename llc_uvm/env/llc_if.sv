`ifndef LLC_IF_SV
`define LLC_IF_SV

interface llc_if (input bit clk);
    logic               rstn;
    logic               vldi;
    logic [2:0]         mode;
    logic [1023:0]      opa;
    logic [1023:0]      opb;
    logic [1023:0]      mask;
    logic               vldo;
    logic [41:0]        out;

    // driver使用的clocking block (避免竞争)
    clocking drv_cb @(posedge clk);
        default input #1ns output #1ns;
        output rstn, vldi, mode, opa, opb, mask;
    endclocking

    // monitor使用的clocking block (确保采样窗口)
    clocking mon_cb @(posedge clk);
        default input #1ns;
        input rstn, vldi, mode, opa, opb, mask, vldo, out;
    endclocking

    // modport for driver
    modport drv_mp (clocking drv_cb);
    // modport for monitor
    modport mon_mp (clocking mon_cb);
endinterface

`endif
