`include "adder.v"

module four_adder(a, b, ci, co, q);
    input[3:0] a;
    input[3:0] b;
    input ci;
    output co;
    output[3:0] q;

    wire[2:0] c;

    adder a0(a[0], b[0], ci, c[0], q[0]);
    adder a1(a[1], b[1], c[0], c[1], q[1]);
    adder a2(a[2], b[2], c[1], c[2], q[2]);
    adder a3(a[3], b[3], c[2], co, q[3]);
    

    

endmodule