#ifndef PARALLEL_SINK_H
#define PARALLEL_SINK_H

#include <list>

class ParallelSink {
   public:
      ParallelSink(void);
      bool advance(uint8_t valid, uint32_t bus);
      void recieve(uint32_t data);
      bool remaining(void);
   private:
      std::list<uint32_t> recieves; 
};
#endif

