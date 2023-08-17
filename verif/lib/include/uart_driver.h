#ifndef UART_DRIVER_H
#define UART_DRIVER_H

#include <list>

enum class UartDriverState { 
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

class UartDriver {
   public:
      UartDriver(uint32_t clk_hz, uint32_t baud, uint32_t timer_base); 
      uint8_t advance(void);
      void send(uint8_t data);
      int left(void);
   private:
      bool io_delay(void);
      UartDriverState state; 
      uint32_t timer_top;
      uint32_t base; 
      uint32_t timer;
      uint32_t delay;
      std::list<uint8_t> sends; 
};
#endif

