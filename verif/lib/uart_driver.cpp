#include <verilated.h>
#include <stdlib.h>
#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include "include/uart_driver.h"
#include <list>

UartDriver::UartDriver(uint32_t baud){ 
   state = UartState::IDLE;
}

uint8_t UartDriver::advance(void) {
   uint8_t tx;
   switch(state){
      case UartState::IDLE:
         if(sends.empty()){
            tx = 1;
         }else{
            tx = 0;
            state = UartState::START;
         }
         break;
   }
   return tx;
}

void UartDriver::send(uint8_t data) {
   sends.push_back(data);
}     
