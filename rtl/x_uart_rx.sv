module x_uart_rx#(
   parameter integer p_clk_hz = 1200000, 
   parameter integer p_baud   = 115200
)(
   input    logic       i_clk,
   input    logic       i_rst,
   input    logic       i_rx,
   output   logic       o_valid,    
   output   logic [7:0] o_data
);
 
   localparam integer p_timer_top   = p_clk_hz / p_baud;
   localparam integer p_timer_half  = p_timer_top / 2;
   localparam integer p_timer_width = $clog2(p_timer_top);
  
   localparam logic [p_timer_width-1:0] p_timer_top_w  = p_timer_top[p_timer_width-1:0];
   localparam logic [p_timer_width-1:0] p_timer_half_w = p_timer_half[p_timer_width-1:0];
   
   typedef enum logic [3:0] {
      IDLE, START, 
      A0, A1, A2, A3, A4, A5, A6, A7
   } sm_uart_t;
 
   logic                      p0_rx;
   logic                      p1_rx;
 
   logic                      rx_fall;

   sm_uart_t                  sm_uart_q;
   sm_uart_t                  sm_uart_d;  
   sm_uart_t                  sm_uart_inc;  
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

   always_ff@(posedge i_clk or posedge i_rst) begin
      if(i_rst)   p1_rx <= 'd1;
      else        p1_rx <= p0_rx;
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

   always_ff@(posedge i_clk or posedge i_rst) begin
      if(i_rst)         timer_q <= 'd0;
      else if(timer_en) timer_q <= timer_d;
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

   always_ff@(posedge i_clk or posedge i_rst) begin
      if(i_rst)            sm_uart_q <= IDLE;
      else if(sm_uart_en)  sm_uart_q <= sm_uart_d;
   end
  
   ///////////////////////////////////////////////////////////////////
   // Ending

   assign valid_d = (sm_uart_q == A7) & timer_top; 

   always_ff@(posedge i_clk or posedge i_rst) begin
      if(i_rst)   valid_q <= 'd0;
      else        valid_q <= valid_d;
   end

   assign o_valid = valid_q;

   ///////////////////////////////////////////////////////////////////
   // Flop RX
 
   assign data_d  = {p0_rx, data_q[7:1]};
   assign data_en = ~(  (sm_uart_q == IDLE)|
                        (sm_uart_q == START)) & 
                      sm_uart_en;
   
   always_ff@(posedge i_clk or posedge i_rst) begin
      if(i_rst)         data_q <= 'd0;
      else if(data_en)  data_q <= data_d;
   end

   assign o_data = data_q;
      
endmodule
