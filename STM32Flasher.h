#ifndef STM32Flasher_h
#define STM32Flasher_h

#include <string>

#include "FileReader.h"
#include "UARTLink.h"

#define DEFAULT_BAUD_RATE_STM_FLASHER 115200

class STM32Flasher {
   public:
    STM32Flasher(std::string port, int baudRate = DEFAULT_BAUD_RATE_STM_FLASHER);

    void connect();

    void readChip();
        void eraseCommand();

    void flashFile(FileReader::file_struct file);
    void flashFile(uint8_t *data, uint16_t size);

   private:
    static const uint8_t START_CODE =               0x7F;
    static const uint8_t ACK =                      0x79;
    static const uint8_t NACK =                     0x1F;

    static const uint8_t COMMAND_GET =              0x00;
    static const uint8_t COMMAND_GET_VERSION =      0x01;
    static const uint8_t COMMAND_GET_ID =           0x02;  
    static const uint8_t COMMAND_READ_MEMORY =      0x11;
    static const uint8_t COMMAND_GO =               0x21;
    static const uint8_t COMMAND_WRITE_MEMORY =     0x31;
    static const uint8_t COMMAND_ERASE =            0x43;
    static const uint8_t COMMAND_EXTENDED_ERASE =   0x44;
    static const uint8_t COMMAND_FULL_CHIP_ERASE =  0xFF;

    static const uint8_t NONE_ERASE_MODE =          0x00;
    static const uint8_t BASIC_ERASE_MODE =         0x01;
    static const uint8_t EXTENDED_ERASE_MODE =      0x02;

    static const uint64_t DEFAULT_TIMEOUT =         1500;

    UARTLink *uart;

    UARTLink::buffer_struct *buffer;

    void openConnection();
    void getCommand();
    void getVersionCommand();
    void getIdCommand();
    void readMemoryCommand(uint32_t start_address, uint8_t length);
    void goCommand(uint32_t address);
    void flashCommand(uint32_t start_address, uint8_t *buffer, uint16_t length);
//    void eraseCommand();

   private:
    uint8_t is_port_open = false;
    uint8_t is_connection_open = false;

    uint8_t eraseChipMode = NONE_ERASE_MODE;

    enum ack_pos { ACK_AT_BEGIN,
                   ACK_AT_END,
                   ACK_AT_BEGIN_AND_END,
                   NONE_ACK };

    void writeCommand(uint8_t command);
    void writeAddress(uint32_t address);
    uint8_t checkResponse(ack_pos pos = NONE_ACK, uint64_t timeout = DEFAULT_TIMEOUT);
};

#endif