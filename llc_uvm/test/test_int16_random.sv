class test_int16_random extends llc_base_test;
    `uvm_component_utils(test_int16_random)

    task run_phase(uvm_phase phase);
        llc_int16_seq seq;

        phase.raise_objection(this);

        seq = llc_int16_seq::type_id::create("seq");
        seq.num_tests = 200;
        `uvm_info("TEST", "Starting INT16 random test (200 iterations)", UVM_NONE)
        seq.start(env.opa_agent.sequencer);

        phase.drop_objection(this);
    endtask
endclass