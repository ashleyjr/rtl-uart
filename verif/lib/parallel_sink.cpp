#include <verilated.h>
#include <stdlib.h>
#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include "include/parallel_sink.h"
#include <list>

void ParallelSink::advance(bus parallel) {
   if(parallel.valid){
      if(parallel.data != recieves.front()){
         cout << "Error";
      }
      recicves.pop_front()
   }
}

void ParallelSink::recieve(uint32_t data) {
   recieves.push_back(data);
}
