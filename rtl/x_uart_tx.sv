module x_uart_tx#(
   p_clk_hz = 12000000, 
   p_baud   = 115200
)(
   input    logic       i_clk,
   input    logic       i_rst,
   input    logic [7:0] i_data,
   output   logic       o_tx,
   input    logic       i_valid,
   output   logic       o_accept
);
 
   localparam p_timer_top   = p_clk_hz / p_baud;
   localparam p_timer_width = $clog2(p_timer_top);
   
   localparam logic [p_timer_width-1:0] p_timer_top_w  = p_timer_top[p_timer_width-1:0];

   typedef enum logic [6:0] {
      IDLE_0, IDLE_1, START, 
      A0, A1, A2, A3, A4, A5, A6, A7, STOP
   } sm_uart_t;
  
   sm_uart_t                  sm_uart_q;
   sm_uart_t                  sm_uart_d;  
   sm_uart_t                  sm_uart_inc;  
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

   always_ff@(posedge i_clk or posedge i_rst) begin
      if(i_rst)         timer_q <= 'd0;
      else if(timer_en) timer_q <= timer_d;
   end
   
   ///////////////////////////////////////////////////////////////////
   // State machine updates
  
   assign sm_uart_inc = sm_uart_q + 'd1; 
   assign sm_uart_d   = (sm_uart_q == STOP    ) ? IDLE_0  : sm_uart_inc;
   assign sm_uart_en  = (sm_uart_q == IDLE_0) ? i_valid : timer_top; 
 
   always_ff@(posedge i_clk or posedge i_rst) begin
      if(i_rst)            sm_uart_q <= IDLE_0;
      else if(sm_uart_en)  sm_uart_q <= sm_uart_d;
   end
  
   ///////////////////////////////////////////////////////////////////
   // Ending

   assign o_accept = (sm_uart_q == STOP) & timer_top; 

   ///////////////////////////////////////////////////////////////////
   // Drive TX
   
   always_comb begin
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
