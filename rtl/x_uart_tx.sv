module x_uart_tx#(
   parameter integer p_clk_hz = 12000000, 
   parameter integer p_baud   = 115200
)(
   input    logic       i_clk,
   input    logic       i_rst_n,
   input    logic [7:0] i_data,
   output   logic       o_tx,
   input    logic       i_valid,
   output   logic       o_accept
);
 
   localparam p_timer_top   = p_clk_hz / p_baud;
   localparam p_timer_width = $clog2(p_timer_top);
   
   localparam logic [p_timer_width-1:0] p_timer_top_w  = p_timer_top[p_timer_width-1:0];

   localparam logic [4:0] IDLE_0 = 5'h00; 
   localparam logic [4:0] IDLE_1 = 5'h01;
   localparam logic [4:0] IDLE_2 = 5'h02;
   localparam logic [4:0] IDLE_3 = 5'h03;
   localparam logic [4:0] IDLE_4 = 5'h04;
   localparam logic [4:0] IDLE_5 = 5'h05;
   localparam logic [4:0] IDLE_6 = 5'h06;
   localparam logic [4:0] IDLE_7 = 5'h07;
   localparam logic [4:0] START  = 5'h08;
   localparam logic [4:0] A0     = 5'h09;
   localparam logic [4:0] A1     = 5'h0A;
   localparam logic [4:0] A2     = 5'h0B;
   localparam logic [4:0] A3     = 5'h0C;
   localparam logic [4:0] A4     = 5'h0D;
   localparam logic [4:0] A5     = 5'h0E;
   localparam logic [4:0] A6     = 5'h0F;
   localparam logic [4:0] A7     = 5'h10;
   localparam logic [4:0] STOP   = 5'h11;
  
   logic [4:0]                sm_uart_q;
   logic [4:0]                sm_uart_d;  
   logic [4:0]                sm_uart_inc;  
   logic                      sm_uart_en;

   logic                      timer_top;
   logic [p_timer_width-1:0]  timer_inc;
   logic [p_timer_width-1:0]  timer_d;
   logic [p_timer_width-1:0]  timer_q;
   logic                      timer_en;

   ///////////////////////////////////////////////////////////////////
   // Timer

   assign timer_top  = (timer_q == p_timer_top_w);
   assign timer_inc  = timer_q + 'd1;
   assign timer_d    = (timer_top) ? 'd0 : timer_inc; 
   assign timer_en   = (sm_uart_q != IDLE_0);

   always@(posedge i_clk or negedge i_rst_n) begin
      if(!i_rst_n)         timer_q <= 'd0;
      else if(timer_en)    timer_q <= timer_d;
   end
   
   ///////////////////////////////////////////////////////////////////
   // State machine updates
  
   assign sm_uart_inc = sm_uart_q + 'd1; 
   assign sm_uart_d   = (sm_uart_q == STOP    ) ? IDLE_0  : sm_uart_inc;
   assign sm_uart_en  = (sm_uart_q == IDLE_0) ? i_valid : timer_top; 
 
   always@(posedge i_clk or negedge i_rst_n) begin
      if(!i_rst_n)            sm_uart_q <= IDLE_0;
      else if(sm_uart_en)     sm_uart_q <= sm_uart_d;
   end
  
   ///////////////////////////////////////////////////////////////////
   // Ending

   assign o_accept = (sm_uart_q == STOP) & timer_top; 

   ///////////////////////////////////////////////////////////////////
   // Drive TX
   
   always@(*) begin
      case(sm_uart_q) 
         START:   o_tx = 1'b0;      
         A0:      o_tx = i_data[0];
         A1:      o_tx = i_data[1];
         A2:      o_tx = i_data[2];
         A3:      o_tx = i_data[3];
         A4:      o_tx = i_data[4];
         A5:      o_tx = i_data[5];
         A6:      o_tx = i_data[6];
         A7:      o_tx = i_data[7];
         default: o_tx = 1'b1;
      endcase
   end
 
   
endmodule
