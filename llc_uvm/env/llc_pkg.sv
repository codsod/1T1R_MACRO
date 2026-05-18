package llc_pkg;
    import uvm_pkg::*;
    `include "uvm_macros.svh"

    // ---- parameter ----
    parameter IN_WIDTH  = 1024;
    parameter DATA_W    = 8;
    parameter OUT_WIDTH = 42;

    // ---- Transaction: 从 opa/cfg agent 传入 scoreboard ----
    class llc_in_tr extends uvm_sequence_item; 
        rand bit [1023:0]  opa;
        rand bit [1023:0]  opb;
        rand bit [1023:0]  mask;
        rand bit [2:0]     mode;
        bit                vldi;

        `uvm_object_utils_begin(llc_in_tr)
            `uvm_field_int(opa,   UVM_ALL_ON);
            `uvm_field_int(opb,   UVM_ALL_ON);
            `uvm_field_int(mask,  UVM_ALL_ON);
            `uvm_field_int(mode,  UVM_ALL_ON);
        `uvm_object_utils_end

        function new(string name = "llc_in_tr");
            super.new(name);
        endfunction
        
        // ---- constraint ----
        constraint c_mode { mode inside {0, 1, 2, 3, 4};}
        constraint c_mask { $countones(mask) > 512;}

    endclass

    // ---- Transaction：从 out agent 传入 scoreboard ----
    class llc_out_tr extends uvm_sequence_item;
        bit             vldo;
        bit [41:0]      out;
        int             cycle;     // 供scoreboard追踪

        `uvm_object_utils_begin(llc_out_tr)
            `uvm_field_int(vldo,  UVM_ALL_ON)
            `uvm_field_int(out,   UVM_ALL_ON)
        `uvm_object_utils_end

        function new(string name = "llc_out_tr");
            super.new(name);
        endfunction
    endclass

    // ---- mult_int16 SV 参考模型（纯 SV 实现，无需 DPI-C） ----
    function automatic bit signed [16:0] mult_int16_ref_lane(
        input bit [7:0] opa_1, opa_2, opb_1, opb_2,
        input bit [2:0] mode
    );
        bit signed [7:0] s_opa_1, s_opb_1;
        bit signed [8:0] s_opa_2, s_opb_2;
        bit signed [16:0] p0, p1, p2, p3;
        bit signed [16:0] tp[4], tp2[4];
        bit signed [1:0] t0_a, t0_b, t1_a, t1_b;
        bit signed [1:0] t2_a, t2_b, t3_a, t3_b;
        bit signed [1:0] t4_a, t4_b, t5_a, t5_b;
        bit signed [1:0] t6_a, t6_b, t7_a, t7_b;

        case (mode)
        0, 3: begin   // INT16 / BF16
            s_opa_1 = ($signed(opa_1)) == -1) ? 8'd0 : opa_1;
            s_opb_1 = ($signed(opb_1)) == -1) ? 8'd0 : opb_1;
            s_opa_2 = {1'b0, opa_2};
            s_opb_2 = {1'b0, opb_2};
            p0 = s_opa_1 * s_opb_1;
            p1 = s_opa_1 * s_opb_2;
            p2 = s_opa_2 * s_opb_1;
            p3 = s_opa_2 * s_opb_2;
            return (p0 << 16) + (p1 << 8) + (p2 << 8) + p3;
        end
        1: begin      // INT8
            return $signed(opa_2) * $signed(opb_2)
                 + $signed(opa_1) * $signed(opb_1);
        end
        2: begin      // INT4
            return $signed(opa_1[3:0]) * $signed(opb_1[3:0])
                 + $signed(opa_1[7:4]) * $signed(opb_1[7:4])
                 + $signed(opa_2[3:0]) * $signed(opb_2[3:0])
                 + $signed(opa_2[7:4]) * $signed(opb_2[7:4]);
        end
        4: begin      // Ternary
            t0_a = $signed(opa_1[1:0]); t0_b = $signed(opb_1[1:0]);
            t1_a = $signed(opa_1[3:2]); t1_b = $signed(opb_1[3:2]);
            t2_a = $signed(opa_1[5:4]); t2_b = $signed(opb_1[5:4]);
            t3_a = $signed(opa_1[7:6]); t3_b = $signed(opb_1[7:6]);
            t4_a = $signed(opa_2[1:0]); t4_b = $signed(opb_2[1:0]);
            t5_a = $signed(opa_2[3:2]); t5_b = $signed(opb_2[3:2]);
            t6_a = $signed(opa_2[5:4]); t6_b = $signed(opb_2[5:4]);
            t7_a = $signed(opa_2[7:6]); t7_b = $signed(opb_2[7:6]);
            tp[0] = t0_a * t0_b; tp2[0] = t4_a * t4_b;
            tp[1] = t1_a * t1_b; tp2[1] = t5_a * t5_b;
            tp[2] = t2_a * t2_b; tp2[2] = t6_a * t6_b;
            tp[3] = t3_a * t3_b; tp2[3] = t7_a * t7_b;
            return (tp[0] + tp2[0]) + (tp[1] + tp2[1])
                 + (tp[2] + tp2[2]) + (tp[3] + tp2[3]);
        end
        default: return 0;
        endcase
    endfunction

    function automatic bit signed [41:0] llc_ref_model(
        input bit [1023:0] opa, opb, mask,
        input bit [2:0] mode
    );
        bit signed [41:0] sum = 0;
        for (int i = 0; i< 64; i++) begin
            bit [15:0] oa = opa[i*16 +: 16];
            bit [15:0] ob = opb[i*16 +: 16];
            if (!mask[i]) continue;
            sum += mult_int16_ref_lane(oa[15:8], oa[7:0], ob[15:8], ob[7:0], mode);
        end
    endfunction

endpackage
            