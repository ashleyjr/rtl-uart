#ifndef PARALLEL_DRIVER_H
#define PARALLEL_DRIVER_H

#include <list>

enum class ParallelState { 
   IDLE,
   PENDING 
};

class ParallelDriver {
   public:
      ParallelDriver(void);
      void advance(uint8_t accept);
      uint32_t getData(void);
      uint8_t getValid(void);
      void send(uint8_t data);
   private:
      ParallelState state;
      bool io_delay(void);
      uint32_t delay;
      uint8_t valid;
      std::list<uint32_t> sends; 
};
#endif

