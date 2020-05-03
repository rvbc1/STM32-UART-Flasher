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

    //readMemoryCommand(0x8000000, 0xFF);
    //eraseCommand();
    //readMemoryCommand(0x8000000, 0xFF);
    //uint8_t test;
    //flashCommand(0x8000000, &test, 0xFF);
    //readMemoryCommand(0x8000000, 0xFF);



}

void STM32Flasher::openConnection() {
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

void STM32Flasher::flashCommand(uint32_t start_address, uint8_t* buffer, uint16_t length){
    writeCommand(0x31);
    checkResponse();
    writeAddress(start_address);
    checkResponse();
    uart->addDataToBufferTX(length);
    uint8_t xor_checksum = 0x00;
    xor_checksum ^= length;
    for(int i = 0; i <= length; i++){
        uart->addDataToBufferTX(buffer[i]);
        xor_checksum ^= buffer[i];
    }
    uart->addDataToBufferTX(xor_checksum);
    uart->writeData();
    checkResponse();
}

void STM32Flasher::eraseCommand(){
    writeCommand(0x43);
    checkResponse();
    // uart->addDataToBufferTX(0x00);
    // uart->addDataToBufferTX(0x00);
    // uart->addDataToBufferTX(0x00);
    // uart->writeData();
    writeCommand(0xFF);
    checkResponse();
}

void STM32Flasher::writeCommand(uint8_t command) {
    uart->addDataToBufferTX(command);
    uart->addDataToBufferTX(~command);
    uart->writeData();
}

void STM32Flasher::flashFile(FileReader::file_struct file){
    int pages = file.size / 256;
    int unfull_page_size = file.size - pages * 256;
    int address = 0;
    eraseCommand();
    for(int page_counter = 0; page_counter < pages; page_counter ++, address += 256){
        flashCommand(0x8000000 + address, &file.data[address], 0xff);
    }
    flashCommand(0x8000000 + address, &file.data[address], unfull_page_size - 1);

    goCommand(0x8000000);
}

// void flashFile(uint8_t *data, uint16_t size){

// }

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

void STM32Flasher::readChip() {
}