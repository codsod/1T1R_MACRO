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

    
endclass