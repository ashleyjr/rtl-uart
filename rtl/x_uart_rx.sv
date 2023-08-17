module x_uart_rx#(
   parameter integer p_clk_hz = 1200000, 
   parameter integer p_baud   = 115200
)(
   input    logic       i_clk,
   input    logic       i_rst_n,
   input    logic       i_rx,
   output   logic       o_valid,    
   output   logic [7:0] o_data
);
 
   localparam integer p_timer_top   = p_clk_hz / p_baud;
   localparam integer p_timer_half  = p_timer_top / 2;
   localparam integer p_timer_width = $clog2(p_timer_top);
  
   localparam logic [p_timer_width-1:0] p_timer_top_w  = p_timer_top[p_timer_width-1:0];
   localparam logic [p_timer_width-1:0] p_timer_half_w = p_timer_half[p_timer_width-1:0];
    
   localparam logic [3:0] IDLE   = 4'h0; 
   localparam logic [3:0] START  = 4'h1; 
   localparam logic [3:0] A0     = 4'h2; 
   localparam logic [3:0] A1     = 4'h3; 
   localparam logic [3:0] A2     = 4'h4; 
   localparam logic [3:0] A3     = 4'h5; 
   localparam logic [3:0] A4     = 4'h6; 
   localparam logic [3:0] A5     = 4'h7; 
   localparam logic [3:0] A6     = 4'h8; 
   localparam logic [3:0] A7     = 4'h9; 
 
   logic                      p0_rx;
   logic                      p1_rx;
 
   logic                      rx_fall;

   logic [3:0]                sm_uart_q;
   logic [3:0]                sm_uart_d;  
   logic [3:0]                sm_uart_inc;  
   logic                      sm_uart_en;
   logic                      sm_uart_idle;
   logic                      sm_uart_start;

   logic                      timer_top;
   logic                      timer_half;
   logic                      timer_wrap;
   logic [p_timer_width-1:0]  timer_inc;
   logic [p_timer_width-1:0]  timer_d;
   logic [p_timer_width-1:0]  timer_q;
   logic                      timer_en;

   logic [7:0]                data_d;
   logic [7:0]                data_q;
   logic                      data_en;

   logic                      valid_d;
   logic                      valid_q;

   ///////////////////////////////////////////////////////////////////
   // Detect Input
  
   assign p0_rx = i_rx;

   always@(posedge i_clk or negedge i_rst_n) begin
      if(!i_rst_n)   p1_rx <= 'd1;
      else           p1_rx <= p0_rx;
   end 
   
   assign rx_fall = ~p0_rx &  p1_rx;

   ///////////////////////////////////////////////////////////////////
   // Timer

   assign timer_top  = (timer_q == p_timer_top_w);
   assign timer_half = (timer_q == p_timer_half_w) &
                       (sm_uart_q == START);
   assign timer_wrap = timer_top | timer_half;
   assign timer_inc  = timer_q + 'd1;
   assign timer_d    = (timer_wrap) ? 'd0 : timer_inc; 
   assign timer_en   =  ~sm_uart_idle |  
                       ((sm_uart_idle) & rx_fall);

   always@(posedge i_clk or negedge i_rst_n) begin
      if(!i_rst_n)         timer_q <= 'd0;
      else if(timer_en)    timer_q <= timer_d;
   end
   
   ///////////////////////////////////////////////////////////////////
   // State machine updates
  
   assign sm_uart_inc   = sm_uart_q + 'd1; 
   assign sm_uart_d     = (sm_uart_q == A7) ? IDLE  : sm_uart_inc;
   assign sm_uart_idle  = (sm_uart_q == IDLE);
   assign sm_uart_start = (sm_uart_q == START);
   assign sm_uart_en    = (sm_uart_idle ) ? rx_fall: 
                          (sm_uart_start) ? timer_half:
                                            timer_top; 

   always@(posedge i_clk or negedge i_rst_n) begin
      if(!i_rst_n)            sm_uart_q <= IDLE;
      else if(sm_uart_en)     sm_uart_q <= sm_uart_d;
   end
  
   ///////////////////////////////////////////////////////////////////
   // Ending

   assign valid_d = (sm_uart_q == A7) & timer_top; 

   always@(posedge i_clk or negedge i_rst_n) begin
      if(!i_rst_n)   valid_q <= 'd0;
      else           valid_q <= valid_d;
   end

   assign o_valid = valid_q;

   ///////////////////////////////////////////////////////////////////
   // Flop RX
 
   assign data_d  = {p0_rx, data_q[7:1]};
   assign data_en = ~(  (sm_uart_q == IDLE)|
                        (sm_uart_q == START)) & 
                      sm_uart_en;
   
   always@(posedge i_clk or negedge i_rst_n) begin
      if(!i_rst_n)         data_q <= 'd0;
      else if(data_en)     data_q <= data_d;
   end

   assign o_data = data_q;
      
endmodule
