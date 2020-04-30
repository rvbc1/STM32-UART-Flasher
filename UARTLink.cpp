#include "UARTLink.h"

#include <iostream>

#include "rs232.h"

#include <iostream>

#include <chrono>


UARTLink::UARTLink(std::string port) {
    com_port = 17;        /* /dev/ttyS0 (COM1 on windows) */
    baud_rate = 115200;       /* 9600 baud */
    buffer_size = 256;

    

    buffer = new uint8_t [buffer_size];
}

int UARTLink::openPort() {
    char mode[]={'8','E','1',0};
    if(RS232_OpenComport(com_port, baud_rate, mode, 0))
    {
        printf("Can not open comport\n");
    }
}

void UARTLink::closePort(){
  
}

void UARTLink::setFlags() {

}

void UARTLink::setSpeed() {

}

void UARTLink::writeData(uint8_t byte){
    std::cout  << "<< ";

    std::cout << "0x" << std::hex << (int) byte << " ";

    std::cout  << std::endl;
    
    RS232_SendByte(com_port, byte);
}

// void UARTLink::writeData(char* buff, int size){
//     RS232_SendByte(com_port, 0x7F);
// }

int UARTLink::available(){

}

int UARTLink::waitForResponse(uint64_t timeout){
      auto start = std::chrono::system_clock::now();

      auto end = start;

      std::chrono::milliseconds ms{1000};

      int reciev_bytes = 0;
      while((reciev_bytes == 0) && (end-start < ms)){
        end = std::chrono::system_clock::now();
        //reciev_bytes = RS232_PollComport(com_port, buffer, buffer_size);
        reciev_bytes = RS232_PollComport(com_port, buffer, buffer_size);
        //std::cout << reciev_bytes;
      }

    std::cout  << ">> ";
    for(int i = 0; i < reciev_bytes; i++){
      std::cout << "0x" << std::hex << (int) buffer[i] << " ";
    }
    std::cout  << std::endl;

    return reciev_bytes;
}

uint8_t* UARTLink::getBuff(){
    return buffer;
}