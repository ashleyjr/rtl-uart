#ifndef UART_SINK_H
#define UART_SINK_H

#include <list>

enum class UartSinkState { 
   IDLE,
   START,
   D0,
   D1,
   D2,
   D3,
   D4,
   D5,
   D6,
   D7,
   STOP
};

class UartSink {
   public:
      UartSink(uint32_t clk_hz, uint32_t baud); 
      uint8_t advance(uint8_t rx);
      void recieve(uint32_t data); 
      bool remaining(void); 
   private: 
      UartSinkState state; 
      uint32_t timer_top;
      uint32_t timer_half;
      uint32_t timer;
      uint8_t frame;
      std::list<uint8_t> recieves; 
};
#endif

