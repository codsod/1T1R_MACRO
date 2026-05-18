class llc_out_monitor extends uvm_monitor;
    `uvm_component_utils(llc_out_monitor)

    virtual llc_if vif;
    uvm_analysis_port #(llc_out_tr) item_collected_port;

    function void build_phase(uvm_phase phase);
        super.build_phase(phase);
        item_collected_port = new("item_collected_port", this);
        if (!uvm_config_db #(virtual llc_if)::get(this, "", "llc_vif", vif))
            `uvm_fatal("NOVIF", "Virtual interface not found")
    endfunction

    task run_phase(uvm_phase phase);
        forever begin
            @(vif.mon_cb);
            if (vif.mon_cb.vldo) begin
                llc_out_tr tr = llc_out_tr::type_id::create("tr");
                tr.vldo  = vif.mon_cb.vldo;
                tr.out   = vif.mon_cb.out;
                tr.cycle = $time;
                item_collected_port.write(tr);
            end
        end
    endtask
endclass

class llc_out_agent extends uvm_agent;
    `uvm_component_utils(llc_out_agent)
    llc_out_monitor monitor;

    function void build_phase(uvm_phase phase);
        super.build_phase(phase);
        monitor = llc_out_monitor::type_id::create("monitor", this);
    endfunction
endclass


