
`timescale 1ns / 1ps


module dct_v2_tb ();
   reg clk;
	reg reset;
   reg start;
	reg [7:0]data_in;
   wire exportProduct;
   wire finish;
	wire signed [31:0] data_out;
   dct_v2 dct_v2 (
      .clk (clk),
      .reset (reset),
      .start (start),
      .data_in (data_in),
      .data_out (data_out),
      .exportProduct (exportProduct),
      .finish (finish)
   );
   integer i = 0;
   integer               input_data_file, output_data_file;
   integer               scan_file    ; 
   reg [7:0]captured_data;
   `define NULL 0  

   initial begin
      reset = 1;
      #5 reset = 0;
      #10 reset = 1;
  end
  initial begin
     clk = 0;
     forever clk = #5 ~clk;
  end

     

   initial begin
   #10 start = 1;
   input_data_file = $fopen("E:/TL/2021-1/TKHTS-2/project/Final/code/text/input/input20_encoded.txt", "r");
   output_data_file = $fopen("E:/TL/2021-1/TKHTS-2/project/Final/code/text/output_verilog/output20.txt", "w");
   if (input_data_file == `NULL) begin
      $display("data_file handle was NULL");
      $stop;
   end
   
   end

   always @(posedge clk) begin
      start = 1;
      scan_file = $fscanf(input_data_file, "%b\n", captured_data); 
      if (!$feof(input_data_file)) begin
         //use captured_data as you would any other wire or reg value;
         data_in = captured_data;
      end
      data_in = captured_data;

      if(exportProduct) begin
         // @(posedge clk);
         // @(posedge clk);
         if(data_out) begin
            $fwriteb(output_data_file, data_out, "\n");
         end
      end
      
      

   
      if(finish) begin
         start = 0;
         $stop;

      end

   end
   
endmodule