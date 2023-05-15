#include <verilated.h>
#include <stdlib.h>
#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include "include/parallel_driver.h"
#include <list>

ParallelDriver::ParallelDriver(void){
   delay = 0;
   state = ParallelState::IDLE;
}

bool ParallelDriver::io_delay(void){ 
   switch(delay){
      case 0:  delay = rand() % 10;
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

void ParallelDriver::advance(uint8_t accept) {
   switch(state){
      case ParallelState::IDLE:
         valid = 0;
         if(!sends.empty() && io_delay()){
            valid = 1; 
            if(accept == 0){
               state = ParallelState::PENDING; 
            }else{
               sends.pop_front();
            }
         }
         break;
      case ParallelState::PENDING:
         if(accept == 1){
            valid = 0;
            if(!sends.empty() && io_delay()){
               valid = 1;
            }else{
               state = ParallelState::IDLE;  
            }
            sends.pop_front();

         }
         break;
   }
}

uint32_t ParallelDriver::getData(void) {
   return sends.front();
}

uint8_t ParallelDriver::getValid(void) { 
   return valid;
}

void ParallelDriver::send(uint8_t data) {
   sends.push_back(data);
}   
