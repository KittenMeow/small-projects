module adder (input a, input b, input ci, output co, output q);
    wire stage1a; // XORs of stage one
    wire stage1c; // Carry of stage on

    // Half-adder
    assign stage1a = a ^ b;
    assign stage1c = a & b;

    // Full adder
    assign q = ci ^ stage1a;
    assign co = stage1c | (ci & stage1a);

endmodule
