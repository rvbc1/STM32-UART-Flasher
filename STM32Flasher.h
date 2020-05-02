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

   UARTLink::buffer_struct *buffer;
   //UARTLink::buffer_struct *writing_buffer;

  // uint8_t buffer [256];

   void checkResponse();

   void openConnection();
   void getCommand();
   void getVersionCommand();
   void getIdCommand();
   void readMemoryCommand(uint32_t start_address, uint8_t length);
   void goCommand(uint32_t address);

   private:
   void writeCommand(uint8_t command);
   void writeAddress(uint32_t address);
};

#endif