module x_back_to_back#(
   parameter integer p_clk_hz = 1000000, 
   parameter integer p_baud   = 115200
)(
   input    logic       i_clk,
   input    logic       i_rst,
   input    logic [7:0] i_data, 
   input    logic       i_valid,
   output   logic       o_accept,
   output   logic       o_valid,    
   output   logic [7:0] o_data
);
   logic tx_rx;

   x_uart_tx #(
      .p_clk_hz   (p_clk_hz   ),
      .p_baud     (p_baud     )
   ) u_tx (
      .i_clk      (i_clk      ),
      .i_rst      (i_rst      ),
      .i_data     (i_data     ),
      .o_tx       (tx_rx      ),
      .i_valid    (i_valid    ),
      .o_accept   (o_accept   )
   );

   x_uart_rx #(
      .p_clk_hz   (p_clk_hz   ),
      .p_baud     (p_baud     )
   )u_rx(
      .i_clk      (i_clk      ),
      .i_rst      (i_rst      ),
      .i_rx       (tx_rx      ),
      .o_valid    (o_valid    ),
      .o_data     (o_data     )
   );

endmodule
