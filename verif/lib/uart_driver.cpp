#include <verilated.h>
#include <stdlib.h>
#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include "include/uart_driver.h"
#include <list>

UartDriver::UartDriver(uint32_t clk_hz, uint32_t baud, uint32_t timer_base){ 
   state = UartDriverState::IDLE;
   timer_top = clk_hz / baud;
   base = timer_base;
   timer = 0;
   delay = 0;
}

bool UartDriver::io_delay(void){ 
   switch(delay){
      case 0:  delay = (base*timer_top) + rand() % timer_top;
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
      case UartDriverState::IDLE:
         tx = 1;
         if(!sends.empty() && io_delay()){
            timer = 0;
            state = UartDriverState::START;
         }
         break;
      case UartDriverState::START: 
         tx = 0;
         if(timer == timer_top){
            timer = 0;
            state = UartDriverState::D0;
         }
         break;
      case UartDriverState::D0:
         tx = (sends.front()) & 0x1;
         if(timer == timer_top){
            timer = 0;
            state = UartDriverState::D1;
         }
         break;
      case UartDriverState::D1:
         tx = (sends.front() >> 1) & 0x1;
         if(timer == timer_top){
            timer = 0;
            state = UartDriverState::D2;
         }
         break;
      case UartDriverState::D2:
         tx = (sends.front() >> 2) & 0x1;
         if(timer == timer_top){
            timer = 0;
            state = UartDriverState::D3;
         }
         break;
      case UartDriverState::D3:
         tx = (sends.front() >> 3) & 0x1;
         if(timer == timer_top){
            timer = 0;
            state = UartDriverState::D4;
         }
         break;
      case UartDriverState::D4:
         tx = (sends.front() >> 4) & 0x1;
         if(timer == timer_top){
            timer = 0;
            state = UartDriverState::D5;
         }
         break;
      case UartDriverState::D5:
         tx = (sends.front() >> 5) & 0x1;
         if(timer == timer_top){
            timer = 0;
            state = UartDriverState::D6;
         }
         break;
      case UartDriverState::D6:
         tx = (sends.front() >> 6) & 0x1;
         if(timer == timer_top){
            timer = 0;
            state = UartDriverState::D7;
         }
         break;
      case UartDriverState::D7:
         tx = (sends.front() >> 7) & 0x1;
         if(timer == timer_top){
            timer = 0;
            sends.pop_front();
            state = UartDriverState::STOP;
         }
         break;
      case UartDriverState::STOP:
         tx = 1;
         if(timer == timer_top){
            timer = 0;
            if(!sends.empty() && io_delay()){
               timer = 0;
               state = UartDriverState::START;
            }else{
               state = UartDriverState::IDLE;
            }
         }
         break;
   }
   return tx;
}

void UartDriver::send(uint8_t data) {
   sends.push_back(data);
}     
