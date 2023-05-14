#ifndef PARALLEL_DRIVER_H
#define PARALLEL_DRIVER_H

#include <list>

class ParallelDriver {
   public:
      ParallelDriver(void);
      uint8_t advance(void);
      void send(uint8_t data);
   private:
      bool io_delay(void);
      uint32_t delay;
      std::list<uint32_t> sends; 
};
#endif

