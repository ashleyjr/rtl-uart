#include <verilated.h>
#include <stdlib.h>
#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include "include/uart_sink.h"
#include <list>

UartSink::UartSink(uint32_t clk_hz, uint32_t baud){ 
   state = UartState::IDLE;
   timer_top = clk_hz / baud;
   timer_half = timer_top >> 1;
   timer = 0;
   frame = 0;
}

uint8_t UartSink::advance(uint8_t rx) {
   timer++;
   switch(state){
      case UartState::IDLE: 
         if(rx == 0){
            timer = 0;
            state = UartState::START;
         }
         break;
      case UartState::START:  
         if(timer == timer_half){
            timer = 0;
            state = UartState::D0;
         }
         break;
      case UartState::D0:
         if(timer == timer_top){
            timer = 0;
            frame |= (rx << 7);
            state = UartState::D1;
         }
         break;
      case UartState::D1:
         if(timer == timer_top){
            timer = 0;
            frame >>= 1;
            frame |= (rx << 7);
            state = UartState::D2;
         }
         break;
      case UartState::D2:
         if(timer == timer_top){
            timer = 0;
            frame >>= 1;
            frame |= (rx << 7);
            state = UartState::D3;
         }
         break;
      case UartState::D3:
         if(timer == timer_top){
            timer = 0;
            frame >>= 1;
            frame |= (rx << 7);
            state = UartState::D4;
         }
         break;
      case UartState::D4:
         if(timer == timer_top){
            timer = 0;
            frame >>= 1;
            frame |= (rx << 7);
            state = UartState::D5;
         }
         break;
      case UartState::D5:
         if(timer == timer_top){
            timer = 0;
            frame >>= 1;
            frame |= (rx << 7);  
            state = UartState::D6;
         }
         break;
      case UartState::D6:
         if(timer == timer_top){
            timer = 0;
            frame >>= 1;
            frame |= (rx << 7);
            state = UartState::D7;
         }
         break;
      case UartState::D7:
         if(timer == timer_top){
            timer = 0;
            state = UartState::STOP;
         }
         break;
      case UartState::STOP: 
         if(timer == timer_top){
            timer = 0;
            state = UartState::IDLE;
            
         }
         break;
   }
   return 0;
}

void UartSink::recieve(uint32_t data) {
   recieves.push_back(data);
}

bool UartSink::remaining(void) {
   return !recieves.empty();
}
