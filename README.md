# rtl-uart
UART module written in SystemVerilog.
Verified with Verilator.

## Module: x_uart_rx

### Parameters
 
 - ``p_clk_hz``: ``i_clk`` frequency in Hz
 - ``p_baud``: Baud rate

### Signals

## Module: x_uart_tx

### Parameters
 
 - ``p_clk_hz``: ``i_clk`` frequency in Hz
 - ``p_baud``: Baud rate

### Signals

 - ``i_clk``: Clock
 - ``i_rst``: Reset active high
 - ``i_data``: Input, 8-bit wide data to be transmitted
 - ``o_tx``: Ouput, 1-bit wide UART transmit wire
 - ``i_valid``: Input, 1-bit wide control signal to be asserted high when ``i_data`` is ready
 - ``o_accept``: Output, 1-bit wide control signal asserted once ``i_data`` captured 

## Module: x_uart

Wrapper for both ``x_uart_rx`` and ``x_uart_tx``.

### Parameters
 
 - p_clk_hz: ``i_clk`` frequency in Hz
 - p_baud: Baud rate

### Signals

  
