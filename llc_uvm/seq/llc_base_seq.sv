class llc_base_seq extends uvm_sequence #(llc_in_tr); 
    `uvm_object_utils(llc_base_seq)
    int num_tests = 100;

    function new(string name = "llc_base_seq");
        super.new(name);
    endfunction
endclass