module x_front_to_front#(
   parameter integer p_clk_hz = 1000000, 
   parameter integer p_baud   = 115200
)(
   input    logic       i_clk,
   input    logic       i_rst,
   input    logic       i_rx,
   output   logic       o_tx
);
   logic       rx_valid;
   logic [7:0] rx_data_d; 
   logic [7:0] rx_data_q;

   logic       valid_en;
   logic       valid_d;
   logic       valid_q;

   logic       tx_accept;   

   x_uart_rx #(
      .p_clk_hz   (p_clk_hz   ),
      .p_baud     (p_baud     )
   )u_rx(
      .i_clk      (i_clk      ),
      .i_rst      (i_rst      ),
      .i_rx       (i_rx       ),
      .o_valid    (rx_valid   ),
      .o_data     (rx_data_d  )
   );
   
   always_ff@(posedge i_clk or posedge i_rst) begin
      if(i_rst)         rx_data_q <= 'd0;
      else if(rx_valid) rx_data_q <= rx_data_d;
   end
   
   assign valid_en = rx_valid | tx_accept;
   
   assign valid_d = rx_valid;
   
   always_ff@(posedge i_clk or posedge i_rst) begin
      if(i_rst)         valid_q <= 'd0;
      else if(valid_en) valid_q <= valid_d;
   end
   
   x_uart_tx #(
      .p_clk_hz   (p_clk_hz   ),
      .p_baud     (p_baud     )
   ) u_tx (
      .i_clk      (i_clk      ),
      .i_rst      (i_rst      ),
      .i_data     (rx_data_q  ),
      .o_tx       (o_tx       ),
      .i_valid    (valid_q    ),
      .o_accept   (tx_accept  )
   );

endmodule
