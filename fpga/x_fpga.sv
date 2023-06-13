module x_fpga#(
   parameter integer p_clk_hz = 12000000, 
   parameter integer p_baud   = 115200
)(
   input    logic       i_clk,
   input    logic       i_rst,
   input    logic       i_rx,
   output   logic       o_tx
); 
   parameter p_divr = 4'd0;
   parameter p_divf = 7'd63;
   parameter p_divq = 
      (p_clk_hz == 192000000) ? 3'd2:
      (p_clk_hz == 96000000)  ? 3'd3:
      (p_clk_hz == 48000000)  ? 3'd4:
                                3'd5;
   logic pll_clk;
   logic clk;

   x_front_to_front #(
      .p_clk_hz   (p_clk_hz   ),
      .p_baud     (p_baud     )
   )u_front_to_front(
      .i_clk      (clk        ),
      .i_rst      (i_rst      ),
      .i_rx       (i_rx       ),
      .o_tx       (o_tx       )
   );

   // Skip PLL
   assign clk = (p_clk_hz == 12000000) ? i_clk : pll_clk;


   SB_PLL40_CORE #(
      .FEEDBACK_PATH ("SIMPLE"   ),
      .PLLOUT_SELECT ("GENCLK"   ),
      .DIVR          (p_divr     ),
      .DIVF          (p_divf     ),
      .DIVQ          (p_divq     ),
      .FILTER_RANGE  (3'b001     )  // Always 1 https://www.reddit.com/r/yosys/comments/3yrq6d/are_plls_supported_on_the_icestick_hw/
   ) u_sb_pll40_core (
      .LOCK          (           ),
      .RESETB        (~i_rst     ),
      .BYPASS        (1'b0       ),
      .REFERENCECLK  (i_clk      ),
      .PLLOUTCORE    (pll_clk    )
   );

endmodule
