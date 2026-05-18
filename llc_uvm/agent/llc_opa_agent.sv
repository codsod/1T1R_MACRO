class llc_opa_driver extends uvm_driver #(llc_in_tr); 
    `uvm_component_utils(llc_opa_driver)
    
    virtual llc_if vif;

    function void build_phase(uvm_phase phase);
        super.build_phase(phase);
        if(!uvm_config_db#(virtual llc_if)::get(this, "", "llc_vif", vif))
            `uvm_fatal("NOVIF", "No virtual interface specified for this driver instance")
    endfunction

    task run_phase(uvm_phase phase); 
        vif.drv_cb.rstn <= 1'b1;
        vif.drv_cb.vldi <= 1'b0;
        vif.drv_cb.opa  <= '0;
        vif.drv_cb.opb  <= '0;
        vif.drv_cb.mask <= '0; 

        repeat (2) @(vif.drv_cb);
        vif.drv_cb.rstn <= 1'b0; 
        repeat (2) @(vif.drv_cb);
        vif.drv_cb.rstn <= 1'b1;
        repeat (2) @(vif.drv_cb);

        forever begin
            seq_item_port.get_next_item(req);
            vif.drv_cb.opa  <= req.opa;
            vif.drv_cb.opb  <= req.opb;
            vif.drv_cb.mask <= req.mask;
            vif.drv_cb.vldi <= 1'b1;
            @(vif.drv_cb);
            vif.drv_cb.vldi <= 1'b0;
            seq_item_port.item_done();
        end
    endtask
endclass

class llc_opa_monitor extends uvm_monitor; 
    `uvm_component_utils(llc_opa_monitor)

    virtual llc_if vif;
    uvm_analysis_port #(llc_in_tr) item_collected_port;

    function void build_phase(uvm_phase phase);
        super.build_phase(phase);
        item_collected_port = new("item_collected_port", this);
        if(!uvm_config_db#(virtual llc_if)::get(this, "", "llc_vif", vif))
            `uvm_fatal("NOVIF", "No virtual interface specified for this monitor instance")
    endfunction

    task run_phase(uvm_phase phase); 
        forever begin
            @(vif.mon_cb);
            if (vif.mon_cb.vldi) begin
                llc_in_tr tr = llc_in_tr::type_id::create("tr");
                tr.opa  = vif.mon_cb.opa;
                tr.opb  = vif.mon_cb.opb;
                tr.mask = vif.mon_cb.mask;
                tr.mode = vif.mon_cb.mode;
                tr.vldi = 1'b1;
                item_collected_port.write(tr);
            end
        end
    endtask
endclass

class llc_opa_agent extends uvm_agent; 
    `uvm_component_utils(llc_opa_agent)

    llc_opa_driver driver;
    llc_opa_monitor monitor;
    uvm_sequencer #(llc_in_tr) sequencer;
    
    function void build_phase(uvm_phase phase);
        super.build_phase(phase);
        monitor   = llc_opa_monitor::type_id::create("monitor",this);
        driver    = llc_opa_driver::type_id::create("driver",this);
        sequencer = uvm_sequencer#(llc_in_tr)::type_id::create("sequencer",this);
    endfunction

    function void connect_phase(uvm_phase phase);
        super.connect_phase(phase);
        driver.seq_item_port.connect(sequencer.seq_item_export);
    endfunction
endclass
