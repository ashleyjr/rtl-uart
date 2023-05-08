#include <verilated.h>
#include <stdlib.h>
#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include "include/parallel_sink.h"
#include <list>

ParallelSink::ParallelSink(void){}

bool ParallelSink::advance(uint8_t valid, uint32_t bus) {
   if(valid){
      if(bus != recieves.front()){
         return false; 
      }
      recieves.pop_front();
   }
   return true;
}

void ParallelSink::recieve(uint32_t data) {
   recieves.push_back(data);
}

bool ParallelSink::remaining(void) {
   return !recieves.empty();
}
