#include <stdlib.h>
#include <iostream>
#include <cstdlib>
#include <verilated.h>
#include <verilated_vcd_c.h>
#include "Vx_uart_rx.h"
#include <stdio.h>

#define STOP 10000

int main(int argc, char** argv, char** env) {
   
   vluint64_t sim_time = 0;
   uint64_t cycles = 0;
   Vx_uart_rx *dut = new Vx_uart_rx; 
   Verilated::traceEverOn(true);
   VerilatedVcdC *m_trace = new VerilatedVcdC; 
      
   dut->trace(m_trace, 5);
   m_trace->open("waveform.vcd");
   
   dut->i_clk     = 0;
   dut->i_rst     = 0;
   dut->i_rx      = 1;
   
   // Tick
   dut->eval();
   m_trace->dump(sim_time); 
   sim_time++;
   
   // Reset
   dut->i_rst = 1;
   
   // Tick
   dut->eval();
   m_trace->dump(sim_time); 
   sim_time++;

   // Out of Reset
   dut->i_rst = 0;

   while (cycles < STOP) {
      
      // Falling Edge
      dut->i_clk = 0; 
     
      // Tick
      dut->eval();
      m_trace->dump(sim_time); 
      sim_time++;
     

      switch(cycles){
         case 100:   dut->i_rx = 0;
      }

      // Rising Edge
      dut->i_clk = 1;
           
      // Tick
      dut->eval();
      m_trace->dump(sim_time); 
      sim_time++;

      // Event
      cycles++; 
   }
      
   m_trace->close();
   delete dut;
   exit(EXIT_SUCCESS);
}

