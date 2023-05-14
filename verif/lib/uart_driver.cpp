#include <verilated.h>
#include <stdlib.h>
#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include "include/uart_driver.h"
#include <list>

UartDriver::UartDriver(uint32_t clk_hz, uint32_t baud){ 
   state = UartState::IDLE;
   timer_top = clk_hz / baud;
   timer = 0;
   delay = 0;
}

bool UartDriver::io_delay(void){ 
   switch(delay){
      case 0:  delay = rand() % timer_top;
               if(0 == delay){
                  return true;
               }
               break; 
      case 1:  delay = 0;
               return true;
      default: delay--;
               break;
   }
   return false;
}
uint8_t UartDriver::advance(void) {
   uint8_t tx;
   timer++;
   switch(state){
      case UartState::IDLE:
         tx = 1;
         if(!sends.empty() && io_delay()){
            timer = 0;
            state = UartState::START;
         }
         break;
      case UartState::START: 
         tx = 0;
         if(timer == timer_top){
            timer = 0;
            state = UartState::D0;
         }
         break;
      case UartState::D0:
         tx = (sends.front()) & 0x1;
         if(timer == timer_top){
            timer = 0;
            state = UartState::D1;
         }
         break;
      case UartState::D1:
         tx = (sends.front() >> 1) & 0x1;
         if(timer == timer_top){
            timer = 0;
            state = UartState::D2;
         }
         break;
      case UartState::D2:
         tx = (sends.front() >> 2) & 0x1;
         if(timer == timer_top){
            timer = 0;
            state = UartState::D3;
         }
         break;
      case UartState::D3:
         tx = (sends.front() >> 3) & 0x1;
         if(timer == timer_top){
            timer = 0;
            state = UartState::D4;
         }
         break;
      case UartState::D4:
         tx = (sends.front() >> 4) & 0x1;
         if(timer == timer_top){
            timer = 0;
            state = UartState::D5;
         }
         break;
      case UartState::D5:
         tx = (sends.front() >> 5) & 0x1;
         if(timer == timer_top){
            timer = 0;
            state = UartState::D6;
         }
         break;
      case UartState::D6:
         tx = (sends.front() >> 6) & 0x1;
         if(timer == timer_top){
            timer = 0;
            state = UartState::D7;
         }
         break;
      case UartState::D7:
         tx = (sends.front() >> 7) & 0x1;
         if(timer == timer_top){
            timer = 0;
            sends.pop_front();
            state = UartState::STOP;
         }
         break;
      case UartState::STOP:
         tx = 1;
         if(timer == timer_top){
            timer = 0;
            if(!sends.empty() && io_delay()){
               timer = 0;
               state = UartState::START;
            }else{
               state = UartState::IDLE;
            }
         }
         break;
   }
   return tx;
}

void UartDriver::send(uint8_t data) {
   sends.push_back(data);
}     
