#include "STM32Flasher.h"

#include <chrono>
#include <iostream>

void printInfo(std::string info) {
    std::cout << info << std::endl;
}

STM32Flasher::STM32Flasher(std::string port) {
    uart = new UARTLink("");
    uart->openPort();
    uint8_t *buffer = uart->getBuff();

    uart->writeData(0x7F);
    checkResponse();

    //GET
    uart->writeData(0x00);
    uart->writeData(0xFF);

    checkResponse();

    //READ MEMORY command
    uart->writeData(0x11);
    uart->writeData(0xEE);

    checkResponse();

    //write addres
    uart->writeData(0x80);
    uart->writeData(0x00);
    uart->writeData(0x00);
    uart->writeData(0x00);
    uart->writeData(0x80);
    checkResponse();
    uart->writeData(0x11);
    uart->writeData(0xee);
    checkResponse();
    checkResponse();
}



void STM32Flasher::checkResponse() {
    int resp_size = uart->waitForResponse(500);
    if (resp_size > 0) {
        uint8_t *buffer = uart->getBuff();
        if (buffer[resp_size - 1] == ACK) {
            printInfo("Response OK!");
        } else if (buffer[resp_size - 1] == NACK) {
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