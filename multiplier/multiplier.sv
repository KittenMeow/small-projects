`include "4adder.sv"

module multiplier(a, b, q);
    input [3:0] a;
    input [3:0] b;
    output [7:0] q;

    wire [3:0] a_stage1;
    wire [3:0] a_stage2;
    wire [3:0] a_stage3;
    wire [3:0] a_stage4;
    wire [5:0] q_stage1;
    wire [5:0] q_stage2;
    wire [5:0] q_stage3;

    assign a_stage1 = a & {8{b[0]}};
    assign a_stage2 = a & {8{b[1]}};
    assign a_stage3 = a & {8{b[2]}};
    assign a_stage4 = a & {8{b[3]}};

    assign q[0] = a_stage1[0];

    four_adder stage1(a_stage1 >> 1, a_stage2, 1'b0, q_stage1[4], q_stage1[3:0]);

    assign q[1] = q_stage1[0];

    four_adder stage2(a_stage3, q_stage1[3:0] >> 1, 1'b0, q_stage2[4], q_stage2[3:0]);

    assign q[2] = q_stage2[0];

    four_adder stage3(a_stage4, q_stage2[3:0] >> 1, 1'b0, q_stage3[4], q_stage3[3:0]);

    assign q[7:3] = q_stage3;

endmodule