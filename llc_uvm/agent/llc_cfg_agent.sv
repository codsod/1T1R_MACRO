// Mode driver: 在驱动 opa/opb 前提前 1 拍写入 mode_reg
// 由于 mode_reg 存储在 DUT 中，需要与数据同步
class llc_cfg_driver extends uvm_driver #(llc_in_tr);
    `uvm_component_utils(llc_cfg_driver)
    function new(string name, uvm_component parent);
        super.new(name, parent);
    endfunction

    virtual llc_if vif;

    function void build_phase(uvm_phase phase);
        super.build_phase(phase);
        if (!uvm_config_db #(virtual llc_if)::get(this, "", "llc_vif", vif))
            `uvm_fatal("NOVIF", "Virtual interface not found")
    endfunction

    task run_phase(uvm_phase phase);
        forever begin
            seq_item_port.get_next_item(req);
            // mode 需要在 vldi 前 1 拍稳定
            @(vif.drv_cb);
            vif.drv_cb.mode <= req.mode;
            seq_item_port.item_done();
            // mdoe 持续有效，直到下一个 req 覆盖
            // 注意：cfg_agent 和 opa_agent 需要通过 virtual_sequencer 同步
        end
    endtask
endclass

class llc_cfg_monitor extends uvm_monitor; 
    `uvm_component_utils(llc_cfg_monitor)
    function new(string name, uvm_component parent);
        super.new(name, parent);
    endfunction

    
    virtual llc_if vif;
    uvm_analysis_port #(llc_in_tr) item_collected_port;

    function void build_phase(uvm_phase phase);
        super.build_phase(phase);
        item_collected_port = new("item_collected_port", this);
        if (!uvm_config_db #(virtual llc_if)::get(this, "", "llc_vif", vif))
            `uvm_fatal("NOVIF", "Virtual interface not found")
    endfunction

    task run_phase(uvm_phase phase);
        forever begin
            @(vif.mon_cb);
            if (vif.mon_cb.vldi) begin
                llc_in_tr tr = llc_in_tr::type_id::create("tr");
                tr.mode = vif.mon_cb.mode;
                item_collected_port.write(tr);
            end
        end
    endtask
endclass

class llc_cfg_agent extends uvm_agent;
    `uvm_component_utils(llc_cfg_agent)
    function new(string name, uvm_component parent);
        super.new(name, parent);
    endfunction

    llc_cfg_driver driver;
    llc_cfg_monitor monitor;
    uvm_sequencer #(llc_in_tr) sequencer;

    function void build_phase(uvm_phase phase);
        super.build_phase(phase);
        driver    = llc_cfg_driver::type_id::create("driver",this);
        monitor   = llc_cfg_monitor::type_id::create("monitor",this);
        sequencer = uvm_sequencer#(llc_in_tr)::type_id::create("sequencer",this);
    endfunction

    function void connect_phase(uvm_phase phase);
        super.connect_phase(phase);
        driver.seq_item_port.connect(sequencer.seq_item_export);
    endfunction
endclass
