#include <stdlib.h>
#include <iostream>
#include <cstdlib>
#include <verilated.h>
#include <verilated_vcd_c.h>
#include "Vx_uart_rx.h"
#include "../lib/include/parallel_sink.h"
#include "../lib/include/uart_driver.h"
#include <stdio.h>

#define PKTS 10000

int main(int argc, char** argv, char** env) {
   
   vluint64_t sim_time = 0; 
   Vx_uart_rx *dut = new Vx_uart_rx; 
   Verilated::traceEverOn(true);
   VerilatedVcdC *m_trace = new VerilatedVcdC; 
   
   uint8_t test_vector;
   UartDriver drv(1000000, 9600);
   ParallelSink sink;
   
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

   // List of sends
   for(int i=0;i<PKTS;i++){
      test_vector = rand(); 
      drv.send(test_vector);
      sink.recieve(test_vector);
   }
   
   while (sink.remaining()) {
      
      // Falling Edge
      dut->i_clk = 0; 
     
      // Tick
      dut->eval();
      m_trace->dump(sim_time); 
      sim_time++;

      // Transactors
      dut->i_rx = drv.advance();
      if(!sink.advance(dut->o_valid, dut->o_data)){      
         m_trace->close();
         delete dut;
         std::cout << "FAIL\n";
         exit(EXIT_FAILURE);
      }

      // Rising Edge
      dut->i_clk = 1;
           
      // Tick
      dut->eval();
      m_trace->dump(sim_time); 
      sim_time++;
   }
      
   m_trace->close();
   delete dut;
   std::cout << "PASS\n";
   exit(EXIT_SUCCESS);
}

