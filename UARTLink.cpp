#include "UARTLink.h"

#include <chrono>
#include <iostream>

#include "rs232.h"

UARTLink::UARTLink(std::string port) {
    com_port = RS232_GetPortnr(port.c_str()); /* /dev/ttyS0 (COM1 on windows) */
    baud_rate = 115200;                       /* 9600 baud */
    // buffer_size = 256;
    buffer.size = writing_buffer.size = 0;
    buffer.max_size = writing_buffer.max_size = 256;

    buffer.data = new uint8_t[buffer.max_size];
    writing_buffer.data = new uint8_t[writing_buffer.max_size];
}

uint8_t UARTLink::openPort() {
    char mode[] = {'8', 'E', '1', 0};
    if (RS232_OpenComport(com_port, baud_rate, mode, 0)) {
        printf("Can not open comport\n");
        port_opened = false;

#if defined(__linux__) || defined(__FreeBSD__) || (__Windows__)
        exit(1);
#endif

    } else {
        port_opened = true;
    }

    return port_opened;
}

void UARTLink::addDataToBufferTX(uint8_t data) {
    writing_buffer.data[writing_buffer.size] = data;
    writing_buffer.size++;
}

void UARTLink::closePort() {
}

void UARTLink::setFlags() {
}

void UARTLink::setSpeed() {
}

void UARTLink::writeData() {
    for (int i = 0; i < writing_buffer.size; i++) {
        std::cout << "<< ";

        std::cout << "0x" << std::hex << (int)writing_buffer.data[i] << " ";

        std::cout << std::endl;
    }

    if (port_opened) {
        RS232_SendBuf(com_port, writing_buffer.data, writing_buffer.size);
    } else {
        errorMsg();
    }

    writing_buffer.size = 0;
}

void UARTLink::writeData(uint8_t byte) {
    std::cout << "<< ";

    std::cout << "0x" << std::hex << (int)byte << " ";

    std::cout << std::endl;

    if (port_opened) {
        RS232_SendByte(com_port, byte);
    } else {
        errorMsg();
    }
}

int UARTLink::available() {
}

int UARTLink::waitForResponse(uint64_t timeout) {
    auto start = std::chrono::system_clock::now();

    auto end = start;

    std::chrono::milliseconds ms{1000};

    int reciev_bytes = 0;
    while ((reciev_bytes == 0) && (end - start < ms)) {
        end = std::chrono::system_clock::now();
        //reciev_bytes = RS232_PollComport(com_port, buffer, buffer_size);
        if (port_opened) {
            reciev_bytes = RS232_PollComport(com_port, buffer.data, buffer.max_size);
        } else {
            errorMsg();
        }
        //std::cout << reciev_bytes;
    }

    std::cout << ">> ";
    for (int i = 0; i < reciev_bytes; i++) {
        std::cout << "0x" << std::hex << (int)buffer.data[i] << " ";
    }
    std::cout << std::endl;

    return reciev_bytes;
}

UARTLink::buffer_struct* UARTLink::getBuff() {
    return &buffer;
}

void UARTLink::errorMsg() {
    std::cout << "Port is not open!" << std::endl;
}