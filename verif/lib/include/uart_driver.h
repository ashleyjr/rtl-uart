#ifndef UART_DRIVER_H
#define UART_DRIVER_H

#include <list>

enum class UartState { 
   IDLE,
   START
};

class UartDriver {
   public:
      UartDriver(uint32_t baud); 
      uint8_t advance(void);
      void send(uint8_t data);
   private:
      UartState state; 
      std::list<uint8_t> sends; 
};
#endif

