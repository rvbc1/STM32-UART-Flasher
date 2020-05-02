
#include <fstream>
#include <iostream>

#include "STM32Flasher.h"

int main() {
    STM32Flasher *flasher = new STM32Flasher("/dev/ttyUSB0");
    //flasher->connect();
    //flasher->readChip();
    // uint32_t start_address = 0x8000000;


    // std::cout << std::hex <<(int) *((uint8_t *) &start_address + 0) << std::endl;
    // std::cout << std::hex <<(int) *((uint8_t *) &start_address + 1) << std::endl;
    // std::cout << std::hex <<(int) *((uint8_t *) &start_address + 2) << std::endl;
    // std::cout << std::hex <<(int) *((uint8_t *) &start_address + 3) << std::endl;




    //  uint8_t x = 0x11;
    //  uint8_t x2 = ~x;
    //  std::cout << std::hex << +x << " " << +((uint8_t)(x2)) << std::endl;

    //std::streampos size;
    // char* memblock;

    // std::ifstream file("F103.binary", std::ios::in | std::ios::binary | std::ios::ate);
    // if (file.is_open()) {
    //     size = file.tellg();
    //     memblock = new char[size];
    //     file.seekg(0, std::ios::beg);
    //     file.read(memblock, size);
    //     file.close();

    //     std::cout << "the entire file content is in memory, " << size << " bytes";

    //     delete[] memblock;
    // } else
    //     std::cout << "Unable to open file";


    // for (int i = 0; i < size; i++) {
    //     if (i % 16 == 0) {
    //         std::cout << std::endl;
    //     }

    //     if (((uint8_t)memblock[i]) < 16) {
    //         std::cout << "0x0" << std::hex << +((uint8_t)memblock[i]) << "\t";
    //     } else {
    //         std::cout << "0x" << std::hex << +((uint8_t)memblock[i]) << "\t";
    //     }
    // }
}

// reading an entire binary file
