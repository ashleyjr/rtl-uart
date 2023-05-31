#include <verilated.h>
#include <stdlib.h>
#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include "include/uart_sink.h"
#include <list>

UartSink::UartSink(uint32_t clk_hz, uint32_t baud){ 
   state = UartSinkState::IDLE;
   timer_top = clk_hz / baud;
   timer_half = timer_top >> 1;
   timer = 0;
   frame = 0;
}

uint8_t UartSink::advance(uint8_t rx) {
   timer++;
   //std::cout << unsigned(frame) << "\n";
   switch(state){
      case UartSinkState::IDLE: 
         if(rx == 0){
            timer = 0;
            state = UartSinkState::START;
         }
         break;
      case UartSinkState::START:  
         if(timer == timer_half){
            timer = 0;
            state = UartSinkState::D0;
         }
         break;
      case UartSinkState::D0:
         if(timer == timer_top){
            timer = 0;
            frame >>= 1;
            frame |= (rx << 7);
            state = UartSinkState::D1;
         }
         break;
      case UartSinkState::D1:
         if(timer == timer_top){
            timer = 0;
            frame >>= 1;
            frame |= (rx << 7);
            state = UartSinkState::D2;
         }
         break;
      case UartSinkState::D2:
         if(timer == timer_top){
            timer = 0;
            frame >>= 1;
            frame |= (rx << 7);
            state = UartSinkState::D3;
         }
         break;
      case UartSinkState::D3:
         if(timer == timer_top){
            timer = 0;
            frame >>= 1;
            frame |= (rx << 7);
            state = UartSinkState::D4;
         }
         break;
      case UartSinkState::D4:
         if(timer == timer_top){
            timer = 0;
            frame >>= 1;
            frame |= (rx << 7);
            state = UartSinkState::D5;
         }
         break;
      case UartSinkState::D5:
         if(timer == timer_top){
            timer = 0;
            frame >>= 1;
            frame |= (rx << 7);
            state = UartSinkState::D6;
         }
         break;
      case UartSinkState::D6:
         if(timer == timer_top){
            timer = 0;
            frame >>= 1;
            frame |= (rx << 7);
            state = UartSinkState::D7;
         }
         break;
      case UartSinkState::D7:
         if(timer == timer_top){
            timer = 0;
            frame >>= 1;
            frame |= (rx << 7);
            state = UartSinkState::STOP;
         }
         break;
      case UartSinkState::STOP: 
         if(timer == timer_top){
            timer = 0;
            if(frame != recieves.front()){
               std::cout << unsigned(frame) << " != " << unsigned(recieves.front()) << "\n";
               return false; 
            }
            recieves.pop_front();
            state = UartSinkState::IDLE;         
         }
         break;
   }
   return true;
}

void UartSink::recieve(uint32_t data) {
   recieves.push_back(data);
}

bool UartSink::remaining(void) {
   return !recieves.empty();
}
