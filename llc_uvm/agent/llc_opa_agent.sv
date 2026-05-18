class llc_opa_driver extends uvm_driver #(llc_in_tr); 
    `uvm_component_utils(llc_opa_driver)
    
    virtual llc_if vif

    function void bulid_phase()
endclass