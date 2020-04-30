#ifndef STM32Flasher_h
#define STM32Flasher_h

#include <string>

#include "UARTLink.h"

class STM32Flasher {
   public:
    STM32Flasher(std::string port);

    void connect();

    void readChip();

   private:
   uint8_t buffer [256];

   UARTLink *uart;
};

#endif