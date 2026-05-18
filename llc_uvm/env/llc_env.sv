class llc_env extends uvm_env;
    `uvm_component_utils(llc_env)

    llc_opa_agent     opa_agent;
    llc_cfg_agent     cfg_agent;
    llc_out_agent     out_agent;
    llc_scoreboard    scoreboard;

    function new(string name, uvm_component parent);
        super.new(name, parent);
    endfunction

    function void build_phase(uvm_phase phase);
        super.build_phase(phase);
        opa_agent = llc_opa_agent::type_id::create("opa_agent", this);
        cfg_agent = llc_cfg_agent::type_id::create("cfg_agent", this);
        out_agent = llc_out_agent::type_id::create("out_agent", this);
        scoreboard = llc_scoreboard::type_id::create("scoreboard", this);
    endfunction

    function void connect_phase(uvm_phase phase);
        super.connect_phase(phase);
        opa_agent.monitor.item_collected_port.connect(scoreboard.in_op_imp);
        out_agent.monitor.item_collected_port.connect(scoreboard.out_op_imp);
    endfunction
endclass