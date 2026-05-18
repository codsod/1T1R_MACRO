class llc_int16_seq extends llc_base_seq; 
    `uvm_object_utils(llc_int16_seq)

    task body(); 
        llc_in_tr tr;
        for(int i = 0; i < 100; i++) begin
            tr = llc_in_tr::type_id::create("tr");
            // 约束随机：mode=0(INT16),mask全1
            if (!tr.randomize() with { mode == 0; mask == {1024{1'b1}};})
                `uvm_error("SEQ", $sformatf("Randomization failed"))
            start_item(tr);
            finish_item(tr); 
        end
    endtask
endclass