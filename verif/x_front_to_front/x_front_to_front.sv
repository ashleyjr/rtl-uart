module x_front_to_front#(
   parameter integer p_clk_hz = 12000000, 
   parameter integer p_baud   = 115200
)(
   input    logic       i_clk,
   input    logic       i_rst,
   input    logic       i_rx,
   output   logic       o_tx
);
   logic          rx_valid;
   logic [7:0]    rx_data;

   logic [7:0]   data_en;
   logic [7:0]    data_q   [7:0];

   logic          rx_ptr_en;
   logic [2:0]    rx_ptr_d;
   logic [2:0]    rx_ptr_q;
   
   logic          tx_ptr_en;
   logic [2:0]    tx_ptr_d;
   logic [2:0]    tx_ptr_q;
     
   logic          tx_valid;
   logic [7:0]    tx_data;
   logic          tx_accept;    

   logic          fill_en;
   logic [4:0]    fill_d;
   logic [4:0]    fill_q;

   logic          top;
   logic          bot;
   logic          drain_en;
   logic          drain_q;
   logic          drain_d;

   x_uart_rx #(
      .p_clk_hz   (p_clk_hz   ),
      .p_baud     (p_baud     )
   )u_rx(
      .i_clk      (i_clk      ),
      .i_rst      (i_rst      ),
      .i_rx       (i_rx       ),
      .o_valid    (rx_valid   ),
      .o_data     (rx_data    )
   );
 
   // Write to the FIFO
   assign data_en = {{7{1'b0}},rx_valid} << rx_ptr_q;

   generate
      for(genvar i=0;i<8;i++) begin
         always_ff@(posedge i_clk or posedge i_rst) begin
            if(i_rst)            data_q[i] <= 'd0;
            else if(data_en[i])  data_q[i] <= rx_data;
         end
      end
   endgenerate
 
   // Write FIFO ptr
   assign rx_ptr_en = rx_valid;

   assign rx_ptr_d = (rx_ptr_q + 'd1);

   always_ff@(posedge i_clk or posedge i_rst) begin
      if(i_rst)            rx_ptr_q <= 'd0;
      else if(rx_ptr_en)   rx_ptr_q <= rx_ptr_d;
   end
  
   // Read FIFO ptr
   assign tx_ptr_en = tx_accept;

   assign tx_ptr_d = (tx_ptr_q + 'd1);

   always_ff@(posedge i_clk or posedge i_rst) begin
      if(i_rst)            tx_ptr_q <= 'd0;
      else if(tx_ptr_en)   tx_ptr_q <= tx_ptr_d;
   end
   
   // Full/Empty 
   assign fill_d = (rx_valid) ? (fill_q + 'd1):
                                (fill_q - 'd1);
                                 
   assign fill_en = rx_valid ^ tx_accept;

   always_ff@(posedge i_clk or posedge i_rst) begin
      if(i_rst)         fill_q <= 'd0;
      else if(fill_en)  fill_q <= fill_d;
   end
   
   // Drain
   assign top = (fill_q == 7);
   assign bot = (fill_q == 0);

   assign drain_en = top | bot;

   assign drain_d = top;

   always_ff@(posedge i_clk or posedge i_rst) begin
      if(i_rst)          drain_q <= 'd0;
      else if(drain_en)  drain_q <= drain_d;
   end
 
   // Tx
   assign tx_valid = drain_q & (tx_ptr_q != rx_ptr_q);

   assign tx_data = data_q[tx_ptr_q];
   
   x_uart_tx #(
      .p_clk_hz   (p_clk_hz   ),
      .p_baud     (p_baud     )
   ) u_tx (
      .i_clk      (i_clk      ),
      .i_rst      (i_rst      ),
      .i_data     (tx_data    ),
      .o_tx       (o_tx       ),
      .i_valid    (tx_valid   ),
      .o_accept   (tx_accept  )
   );

endmodule
