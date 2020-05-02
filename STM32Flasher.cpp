#include "STM32Flasher.h"

#include <chrono>
#include <iostream>

void printInfo(std::string info) {
    std::cout << info << std::endl;
}

STM32Flasher::STM32Flasher(std::string port) {
    uart = new UARTLink("");
    uart->openPort();
    //  uint8_t *buffer = uart->getBuff();
    buffer = uart->getBuff();
    //writing_buffer = &uart->writing_buffer;

    openConnection();
    getCommand();

    readMemoryCommand(0x80000FF, 0xFF);

    goCommand(0x8000000);

    /*
    //GET_ID
    writeCommand(0x02);
    checkResponse();

    //GET
    writeCommand(0x00);
    checkResponse();
    

    writeCommand(0x11);
    checkResponse();

    uart->writeData(0x08);
    uart->writeData(0x00);
    uart->writeData(0x00);
    uart->writeData(0x00);
    uart->writeData(0x08);
    checkResponse();

    uart->writeData(0x00);
    uart->writeData(0xFF);
    checkResponse();

    //GET
    writeCommand(0x02);
    checkResponse();

    
//MEMORY
    writeCommand(0x11);
    checkResponse();

    uart->writeData(0x08);
    uart->writeData(0x00);
    uart->writeData(0x00);
    uart->writeData(0x00);
    uart->writeData(0x08);
    checkResponse();

    uart->writeData(0x00);
    uart->writeData(0xFF);
    checkResponse();

    //MEMORY
    writeCommand(0x11);
    checkResponse();

    uart->writeData(0x08);
    uart->writeData(0x00);
    uart->writeData(0x00);
    uart->writeData(0x00);
    uart->writeData(0x08);
    checkResponse();

    uart->writeData(0x00);
    uart->writeData(0xFF);
    checkResponse();

        //MEMORY
    writeCommand(0x11);
    checkResponse();

    uart->writeData(0x08);
    uart->writeData(0x00);
    uart->writeData(0x00);
    uart->writeData(0x00);
    uart->writeData(0x08);
    checkResponse();

    uart->writeData(0x00);
    uart->writeData(0xFF);
    checkResponse();










    //MEMORY
    writeCommand(0x11);
    checkResponse();

    uart->writeData(0x1F);
    uart->writeData(0xFF);
    uart->writeData(0xF8);
    uart->writeData(0x00);
    uart->writeData(0x18);
    checkResponse();

    uart->writeData(0x0F);
    uart->writeData(0xF0);
    checkResponse();


    //MEMORY
    writeCommand(0x11);
    checkResponse();

    uart->writeData(0x1F);
    uart->writeData(0xFF);
    uart->writeData(0xF8);
    uart->writeData(0x00);
    uart->writeData(0x18);
    checkResponse();

    uart->writeData(0x0F);
    uart->writeData(0xF0);
    checkResponse();


    //GET
    writeCommand(0x02);
    checkResponse();


    //MEMORY
    writeCommand(0x11);
    checkResponse();

    uart->writeData(0x08);
    uart->writeData(0x00);
    uart->writeData(0x00);
    uart->writeData(0x00);
    uart->writeData(0x08);
    checkResponse();

    uart->writeData(0xFF);
    uart->writeData(0x00);
    checkResponse();
*/

    // for (uint8_t j = 1; j <= 0xFF; j++) {
    //     int ok = -1;
    //     for (uint8_t i = 0; i <= 0xFF; i++) {
    //         //READ MEMORY command
    //         writeCommand(0x11);
    //         //uart->writeData(0x11);
    //         //uart->writeData(0xEE);

    //         checkResponse();

    //         //write addres
    //         uart->writeData(0x60);
    //         uart->writeData(0x00);
    //         uart->writeData(0x00);
    //         uart->writeData(0x00);
    //         uart->writeData(0x60);
    //         checkResponse();
    //         //size
    //         // writeCommand(0x11);
    //         // uart->writeData(0x11);
    //         // uart->writeData(0xee);
    //         uart->writeData(0x11);
    //         uart->writeData(i);
    //         // writeCommand(0x20);
    //         checkResponse();
    //         //  checkResponse();
    //         if (buffer[0] == ACK) {
    //             ok = i;
    //             break;
    //         }

    //         if (i == 255) {
    //             break;
    //         }

    //         //std::cout << std::endl;
    //     }

    //     std::cout << "For 0x" << std::hex << (int)j <<  " ok is 0x" << std::hex << (int)ok << std::endl;
    //     if(j == 255){
    //         break;
    //     }
    // }
}

void STM32Flasher::openConnection() {
    // uart->writeData(0x7F);
    uart->addDataToBufferTX(0x7F);
    uart->writeData();
    checkResponse();
}

void STM32Flasher::getCommand() {
    writeCommand(0x00);
    checkResponse();

    std::cout << "Frame size: " << std::dec << +buffer->data[1] << std::endl;
    std::cout << "bootloader: " << std::hex << +buffer->data[2] << std::endl;
}

void STM32Flasher::getVersionCommand() {
    writeCommand(0x01);
    checkResponse();
}

void STM32Flasher::getIdCommand() {
    writeCommand(0x02);
    checkResponse();
}

void STM32Flasher::readMemoryCommand(uint32_t start_address, uint8_t length) {
    writeCommand(0x11);
    checkResponse();


    
    // writing_buffer->data[0] = *((uint8_t *) &start_address + 3);
    // writing_buffer->data[0] = *((uint8_t *) &start_address + 3);
    // writing_buffer->data[0] = *((uint8_t *) &start_address + 3);

    // uart->writeData(*((uint8_t *) &start_address + 2));
    // uart->writeData(*((uint8_t *) &start_address + 1));
    // uart->writeData(*((uint8_t *) &start_address + 0));

    // uart->writeData(0x08);
    writeAddress(start_address);
    checkResponse();

    writeCommand(length);
    checkResponse();
}

void STM32Flasher::goCommand(uint32_t address){
    writeCommand(0x21);
    checkResponse();
    writeAddress(address);
    checkResponse();
}

void STM32Flasher::writeCommand(uint8_t command) {
    uart->addDataToBufferTX(command);
    uart->addDataToBufferTX(~command);
    uart->writeData();
    // uart->writeData(command);
    // uart->writeData(~command);
}

void STM32Flasher::writeAddress(uint32_t address){
    uint8_t xor_checksum = 0x00;
    for(int i = 0; i < sizeof(uint32_t); i++){
        uint8_t msb_byte = *((uint8_t *) &address + sizeof(uint32_t) - i - 1);
        uart->addDataToBufferTX(msb_byte);
        xor_checksum ^= msb_byte;
    }
    uart->addDataToBufferTX(xor_checksum);
    uart->writeData();
}

void STM32Flasher::checkResponse() {
    buffer->size = uart->waitForResponse(500);
    if (buffer->size > 0) {
        if (buffer->data[buffer->size - 1] == ACK) {
            printInfo("Response OK!");
        } else if (buffer->data[buffer->size - 1] == NACK) {
            printInfo("Device not recgnize command");
        } else {
            printInfo("unknow response");
        }
    } else {
        printInfo("no response");
    }
}

void STM32Flasher::connect() {
}

//0x8000000

void STM32Flasher::readChip() {
}