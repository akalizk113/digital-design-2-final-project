module dct_v2(
   input clk,	
	input reset,
   input start,

	input [7:0]data_in,

   output finish,
   output exportProduct,
	output signed [31:0] data_out
);
   wire [1:0] control;
   wire load_control;

   wire load_finished;
   wire one_dct_finished;
   wire transpose_finished;

   dct_controller ctrl (
      .clk (clk),
      .reset (reset),
      .start (start),

      .transpose_finished (transpose_finished),
      .one_dct_finished (one_dct_finished),
      .load_finished (load_finished),

      .control (control),
      .load_control (load_control),
      
      .exportProduct (exportProduct),
      .finish (finish)
   );

   datapath dtp(
      .clk (clk),
      .reset (reset),

      .data_in (data_in),

      .control (control),
      .load_control (load_control),

      .transpose_finished (transpose_finished),
      .one_dct_finished (one_dct_finished),
      .load_finished (load_finished),

      .product (data_out)

      
   );

endmodule

module dct_controller(
   input clk,
   input reset,
   input start,

   input transpose_finished,
   input one_dct_finished,
   input load_finished,

   output reg [1:0] control,
   output reg load_control,

   output reg exportProduct,
   output reg finish
);


   localparam  IDLE  = 2'b00,
               LOAD  = 2'b01,
               ONE_DCT   = 2'b10,
               TRANSPOSE = 2'b11;


   reg [1:0] state, next_state;

   integer transpose_cnt = 0;
   integer one_dct_cnt = 0;
    
   always @(posedge clk or negedge reset) begin : proc_state
   if(~reset) begin
      state <= 0;
   end else begin
      state <= next_state;
   end
   end

   always @(*) begin
      next_state  = IDLE;
      finish      = 0;
      control = 0;
      load_control = 0;
      exportProduct = 0;
      one_dct_cnt = one_dct_cnt;
      transpose_cnt = transpose_cnt;
      case (state)
         IDLE: begin
            if(start) begin
               next_state = LOAD;
            end
            else begin
               next_state = IDLE;
            end
         end
         LOAD: begin
            control = 2'b01;
            if(transpose_cnt >= 2) begin
               load_control = 1;
               // @(posedge clk);
               exportProduct = 1;
            end

            if(load_finished) begin
               if(transpose_cnt < 2) begin
                  next_state = ONE_DCT;
               end
               else begin
                  finish = 1;
                  next_state = IDLE;
               end
            end
            else begin
               next_state = LOAD;
            end
         end
         ONE_DCT: begin
            control = 2'b10;
            if(one_dct_finished) begin 
               next_state = TRANSPOSE;
            end
            else begin
               next_state = ONE_DCT;
            end
            
         end

         TRANSPOSE: begin
            control = 2'b11;
           if(transpose_finished) begin
               transpose_cnt = transpose_cnt + 1;
               if(transpose_cnt < 2) begin
                  next_state = ONE_DCT;
               end
               else begin
                  next_state = LOAD;
                  // transpose_cnt = 0;
               end
            end
            else begin
               next_state = TRANSPOSE;
            end

         end
      endcase
   end

endmodule


module datapath (
   input clk,
   input reset,
   input [7:0] data_in,
   
   input [1:0] control,
   input load_control,

   output reg load_finished,
   output reg transpose_finished,
   output reg one_dct_finished,

   output reg signed [31:0] product

);

localparam  IDLE  = 2'b00,
            LOAD  = 2'b01,
            ONE_DCT   = 2'b10,
            TRANSPOSE = 2'b11;
reg signed [31:0] data [63:0];


integer i;
integer j;
reg signed [31:0] a;
reg signed [31:0] x0,x1,x2,x3,x4,x5,x6,x7;
wire signed [31:0] y0,y1,y2,y3,y4,y5,y6,y7;
wire oft;
one_dct o1(
   .clk (clk),
   .oft (oft),
   .reset (reset),
   .x0 (x0),
   .x1 (x1),
   .x2 (x2),
   .x3 (x3),
   .x4 (x4),
   .x5 (x5),
   .x6 (x6),
   .x7 (x7),
   .y0 (y0),
   .y1 (y1),
   .y2 (y2),
   .y3 (y3),
   .y4 (y4),
   .y5 (y5),
   .y6 (y6),
   .y7 (y7)

);

always @(control, load_control) begin
   load_finished = 0;
   transpose_finished = 0;
   one_dct_finished = 0;
   i = 0;
   j = 0;
end

always @(posedge clk or negedge reset) begin 
   if(~reset) begin
      for(j = 0; j < 64; j = j + 1) begin
         data[j] <= 0;
         a <= 0;
      end
      j = 0;
   end
   else begin 
      
      if(control == LOAD ) begin
         if(!load_control) begin 
            a [27:20] = data_in;
            data[i] = a;
            i = i + 1;
            if(i == 64) begin
               load_finished <= 1;
               // i <= 0;
            end
         end else begin
            product <= data[i];
            i = i + 1; 
            if(i == 64) begin
               load_finished <= 1;
            end
         end
      end

      if(control == ONE_DCT) begin
         if(i <= 8) begin 
            x0 <= data[i * 8 ];
            x1 <= data[i * 8 + 1];
            x2 <= data[i * 8 + 2];
            x3 <= data[i * 8 + 3];
            x4 <= data[i * 8 + 4];
            x5 <= data[i * 8 + 5];
            x6 <= data[i * 8 + 6];
            x7 <= data[i * 8 + 7];
         end

         if(oft) begin            
            data[(i - 1) * 8] = y0;
            data[(i - 1) * 8 + 1] = y1;
            data[(i - 1) * 8 + 2] = y2;
            data[(i - 1) * 8 + 3] = y3;
            data[(i - 1) * 8 + 4] = y4;
            data[(i - 1) * 8 + 5] = y5;
            data[(i - 1) * 8 + 6] = y6;
            data[(i - 1) * 8 + 7] = y7;
            i = i + 1;
            
         end
         if(i > 8 ) begin
            one_dct_finished <= 1;
            x0 <= 0;      
            x1 <= 0;
            x2 <= 0;
            x3 <= 0;
            x4 <= 0;
            x5 <= 0;
            x6 <= 0;
            x7 <= 0;
         end

      end
      else begin
         x0 <= 0;
         x1 <= 0;
         x2 <= 0;
         x3 <= 0;
         x4 <= 0;
         x5 <= 0;
         x6 <= 0;
         x7 <= 0;
      end
      if(control == TRANSPOSE) begin 
         data[i * 8 + j] <= data[j * 8 + i];
         data[j * 8 + i] <= data[i * 8 + j];
         j = j + 1;
         if(j == 8) begin 
            i = i + 1;
            j = i;
            if (i == 8 ) begin
               transpose_finished <= 1;
               
            end
         end
      end
   end
end

endmodule


module one_dct (
   input clk,

   input reset,

   input signed [31:0]x0,x1,x2,x3,x4,x5,x6,x7,

   output reg oft,

	output reg signed [31:0]y0,y1,y2,y3,y4,y5,y6,y7 
);

               
reg signed [31:0] btsum1, btsum2, btsum3, btsum4,
                  btsub1, btsub2, btsub3, btsub4,
                  btsum5, btsum6, btsub5, btsub6,
                  btsum7, btsum8, btsum9, btsum10,
                  btsub7, btsub8, btsub9, btsub10, b1,b2,b3,b4;

reg signed [63:0] c_x1, c_x2, c_x3, c_x4, c_x5, c_x6,
                  c_y1, c_y2, c_y3, c_y4, c_y5, c_y6,
                  a0, a1, a2, a3, a4, a5, a6, a7;
reg signed [127:0] b3_ovf;
wire signed [31:0] half = 32'b00000000000010000000000000000000;
wire signed [63:0] half_ovf = 64'b0000000000000000000000001000000000000000000000000000000000000000;
  
wire signed [31:0] 
   cos_1 = 32'b00000000000010110101000001001111,
   sin_1 = 32'b00000000000010110101000001001111,
   cos_2 = 32'b00000000000001100001111101111000,
   sin_2 = 32'b00000000000011101100100000110101,
   cos_3 = 32'b00000000000000110001111100010111,
   sin_3 = 32'b00000000000011111011000101001011,
   cos_4 = 32'b00000000000011010100110110110011,
   sin_4 = 32'b00000000000010001110001110011101;
  

always @(posedge clk or negedge reset) begin
   if(~reset) begin
      y0 <= 0;
      y1 <= 0;
      y2 <= 0;
      y3 <= 0;
      y4 <= 0;
      y5 <= 0;
      y6 <= 0;
      y7 <= 0;
      oft <= 0;
   end
end

always @(*) begin
   oft = 0;
   // stage 1

   btsum1 = x0 + x7;
   btsub1 = x0 - x7;

   btsum2 = x1 + x6;
   btsub2 = x1 - x6;

   btsum3 = x2 + x5;
   btsub3 = x2 - x5;

   btsum4 = x3 + x4;
   btsub4 = x3 - x4;


   // Stage 2

   btsum5 = btsum1 + btsum4;
   btsub5 = btsum1 - btsum4;

   btsum6 = btsum2 + btsum3;
   btsub6 = btsum2 - btsum3; 

   c_x3 = (btsub1 * cos_3 - btsub4 * sin_3);
   c_y3 = (btsub1 * sin_3 + btsub4 * cos_3);

   c_x4 = (btsub3 * cos_4 - btsub2 * sin_4);
   c_y4 = (btsub3 * sin_4 + btsub2 * cos_4);

   c_x5 = (btsub1 * cos_4 - btsub4 * sin_4);
   c_y5 = (btsub1 * sin_4 + btsub4 * cos_4);

   c_x6 = (btsub3 * cos_3 - btsub2 * sin_3);
   c_y6 = (btsub3 * sin_3 + btsub2 * cos_3);


   // stage 3

   btsum7 = c_y3[51:20] + c_y4[51:20];
   btsum8 = c_x3[51:20] + c_x4[51:20];

   btsub9 = c_x5[51:20] - c_y6[51:20];
   btsum10 = c_y5[51:20] + c_x6[51:20];
   // a1 = btsum7 * half;
   // a2 = btsum8 * half;

   c_x1 = (btsum5 * cos_1 - btsum6 * sin_1);
   c_y1 = (btsum5 * sin_1 + btsum6 * cos_1);

   c_x2 = (btsub5 * cos_2 - btsub6 * sin_2);
   c_y2 = (btsub5 * sin_2 + btsub6 * cos_2);


   // stage 4

   b3_ovf = c_y1 * half_ovf;


   b1 = c_x1[51:20];
   b2 = c_y2[51:20];
   b3 = c_y1[51:20];
   b4 = c_x2[51:20];
   
   // a0 = b3 * half;
   a0 = b3_ovf[103:40];
   a4 = b1 * half;
   

   a6 = b4  * half;
   a2 = b2 * half;

   a1 = btsum7 * half;
   a7 = btsum8 * half;

   a3 = btsub9 * half;
   a5 = btsum10 * half;

   
   


   y0 = a0[51:20];
   y4 = a4[51:20];

   y6 = a6[51:20];
   y2 = a2[51:20];

   y1 = a1[51:20];
   y7 = a7[51:20];

   y3 = a3[51:20];
   y5 = a5[51:20];

   oft = 1;
end
endmodule