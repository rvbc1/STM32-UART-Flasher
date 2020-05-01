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

   static const uint8_t ACK = 0x79;
   static const uint8_t NACK = 0x1F;

   UARTLink *uart;

   uint8_t buffer [256];

   void checkResponse();
};

#endif