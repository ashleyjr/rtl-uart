#include <stdlib.h>
#include <iostream>
#include <cstdlib>
#include <verilated.h>
#include <verilated_vcd_c.h>
#include "Vx_uart_tx.h"
#include "../lib/include/parallel_driver.h"
#include "../lib/include/uart_sink.h"
#include <stdio.h>

int main(int argc, char** argv, char** env) {
   
   vluint64_t sim_time = 0; 
   Vx_uart_tx *dut = new Vx_uart_tx; 
   Verilated::traceEverOn(true);
   #ifdef TRACE_ENABLED
   VerilatedVcdC *m_trace = new VerilatedVcdC; 
   #endif 

   uint32_t clk = std::stoi(argv[1]);
   uint32_t baud = std::stoi(argv[2]);
   uint32_t pkts = std::stoi(argv[3]);

   std::cout << "CONFIG: clk=" << clk << ",baud=" << baud << ",pkts=" << pkts << "\n";

   uint8_t test_vector;
   UartSink sink(clk, baud);
   ParallelDriver drv;
  
   #ifdef TRACE_ENABLED
   dut->trace(m_trace, 5);
   m_trace->open("waveform.vcd");
   #endif   
   
   dut->i_clk     = 0;
   dut->i_rst     = 0;
   dut->i_data    = 0;
   dut->i_valid   = 0;
   
   // Tick
   dut->eval();
   #ifdef TRACE_ENABLED
   m_trace->dump(sim_time); 
   #endif
   sim_time++;
   
   // Reset
   dut->i_rst = 1;
   
   // Tick
   dut->eval();
   #ifdef TRACE_ENABLED
   m_trace->dump(sim_time); 
   #endif
   sim_time++;

   // Out of Reset
   dut->i_rst = 0;

   // List of sends
   for(uint32_t i=0;i<pkts;i++){
      test_vector = rand(); 
      drv.send(test_vector);
      sink.recieve(test_vector);
   }
   
   while (sink.remaining()) {
      
      // Falling Edge
      dut->i_clk = 0; 
     
      // Tick
      dut->eval();
      #ifdef TRACE_ENABLED
      m_trace->dump(sim_time); 
      #endif
      sim_time++;

      // Transactors
      drv.advance(dut->o_accept);  
      dut->i_data = drv.getData();
      dut->i_valid = drv.getValid();
      if(!sink.advance(dut->o_tx)){      
         #ifdef TRACE_ENABLED  
         m_trace->close();
         #endif
         delete dut;
         std::cout << "FAIL\n";
         exit(EXIT_FAILURE);
      }

      // Rising Edge
      dut->i_clk = 1;
      
      // Tick
      dut->eval();
      #ifdef TRACE_ENABLED
      m_trace->dump(sim_time); 
      #endif
      sim_time++;
   }
     
   #ifdef TRACE_ENABLED
   m_trace->close();
   #endif

   delete dut;
   std::cout << "PASS\n";
   exit(EXIT_SUCCESS);
}

