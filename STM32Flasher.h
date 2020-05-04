#ifndef STM32Flasher_h
#define STM32Flasher_h

#include <string>

#include "UARTLink.h"
#include "FileReader.h"


class STM32Flasher {
   public:
    STM32Flasher(std::string port);

    void connect();

    void readChip();

    void flashFile(FileReader::file_struct file);
    //void flashFile(uint8_t *data, uint16_t size);

   private:

   static const uint8_t ACK = 0x79;
   static const uint8_t NACK = 0x1F;

   static const uint8_t ERASE_COMMAND = 0x43;
   static const uint8_t FULL_CHIP_ERASE_COMMAND = 0xFF;


   static const uint8_t GO_COMMAND = 0x21;
   static const uint8_t WRITE_MEMORY_COMMAND = 0x31;
   static const uint8_t READ_MEMORY_COMMAND = 0x11; 
   static const uint8_t GET_ID_COMMAND = 0x02;
   static const uint8_t START_CODE = 0x7F;
   static const uint8_t GET_VERSION_COMMAND = 0x01;
   static const uint8_t GET_COMMAND = 0x00;


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
   void flashCommand(uint32_t start_address, uint8_t* buffer, uint16_t length);
   void eraseCommand();
  

   private:
   void writeCommand(uint8_t command);
   void writeAddress(uint32_t address);
};

#endif