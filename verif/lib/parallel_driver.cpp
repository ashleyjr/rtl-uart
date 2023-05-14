#include <verilated.h>
#include <stdlib.h>
#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include "include/parallel_driver.h"
#include <list>

ParallelDriver::ParallelDriver(void){}

bool ParallelDriver::io_delay(void){ 
   switch(delay){
      case 0:  delay = rand() % 100;
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

uint8_t ParallelDriver::advance(void) {
   
   return 0;
}

void ParallelDriver::send(uint8_t data) {
   sends.push_back(data);
}   
