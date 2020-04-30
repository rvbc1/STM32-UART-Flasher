#include "STM32Flasher.h"

#include <chrono>
#include <iostream>

#include "UARTLink.h"

STM32Flasher::STM32Flasher(std::string port) {
    UARTLink *u = new UARTLink("");
    u->openPort();
    uint8_t *buffer = u->getBuff();

    u->writeData(0x7F);
    int resp_size = u->waitForResponse(500);

    u->writeData(0x00);
    u->writeData(0xFF);

    resp_size = u->waitForResponse(500);
}

void STM32Flasher::connect() {
}

//0x8000000

void STM32Flasher::readChip() {
}