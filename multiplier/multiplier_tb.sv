module multiplier_tb;
    reg [3:0] a;
    reg [3:0] b;
    wire [7:0] q;

    multiplier mul(.a (a), .b (b), .q (q));

    initial begin
        $monitor ("[%0t] a=%0h b=%0h | q=%0h", $time, a, b, q);
        a <= 4'b0000;
        b <= 4'b0000;
        #2 a <= 4'b0011;
        #2 b <= 4'b0001;
        #2 b <= 4'b0011;
        #2 a <= 4'b0101;
        #2 b <= 4'b1001;
        #2 a <= 4'b1001;
    end

    initial
    begin
        $dumpfile("multipler.vcd");
        $dumpvars(0, multiplier_tb);
    end
endmodule