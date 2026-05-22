import uvm_pkg::*;
`include "uvm_macros.svh"
import llc_pkg::*;
`uvm_analysis_imp_decl(_in_op)
`uvm_analysis_imp_decl(_out_op)

class llc_scoreboard extends uvm_scoreboard; 
    `uvm_component_utils(llc_scoreboard)

    uvm_analysis_imp_in_op #(llc_in_tr, llc_scoreboard) in_op_imp;
    uvm_analysis_imp_out_op #(llc_out_tr, llc_scoreboard) out_op_imp;

    // FIFO 对齐 3 拍流水延迟
    bit signed [41:0] exp_q[$]; // $ 动态队列，可变长度的由符号数据序列
    int unsigned pipeline_depth = 3;

    int total_checked;
    int total_passed;
    int total_failed;

    function new(string name, uvm_component parent);
        super.new(name,parent);
    endfunction

    function void build_phase(uvm_phase phase);
        super.build_phase(phase);
        in_op_imp = new("in_op_imp", this);
        out_op_imp = new("out_op_imp", this);
    endfunction

    function void write_in_op(llc_in_tr tr);
        bit signed [41:0] expected;
        static int dbg_cnt = 0;
        expected = llc_ref_model(tr.opa, tr.opb, tr.mask, tr.mode);
        exp_q.push_back(expected);
        `uvm_info("SCBD", $sformatf(
            "IN:  opa=0x%0h opb=0x%0h mask=0x%0h mode=%0d exp=0x%0h",
            tr.opa, tr.opb, tr.mask, tr.mode, expected), UVM_HIGH)
        if (dbg_cnt < 2) begin
            $display("DEBUG_IN[%0d]: opa[15:0]=0x%0h opb[15:0]=0x%0h mode=%0d exp=0x%0h(%0d)",
                dbg_cnt, tr.opa[15:0], tr.opb[15:0], tr.mode, expected, $signed(expected));
            dbg_cnt++;
        end
    endfunction

    function void write_out_op(llc_out_tr tr); 
        bit signed [41:0] expected;
        total_checked++;

        if(exp_q.size() == 0) begin
            `uvm_error("SCBD", $sformatf(
                "OUT without matching IN: out=0x%0h cycle=%0d",
                tr.out, tr.cycle))
            total_failed++;
            return;
        end

        expected = exp_q.pop_front();

        if(tr.out != expected) begin
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
    