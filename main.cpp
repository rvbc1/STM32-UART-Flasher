

#include "STM32Flasher.h"
#include "FileReader.h"

int main() {
    STM32Flasher *flasher = new STM32Flasher("/dev/ttyUSB0");

    FileReader *file = new FileReader("F103.binary");

    flasher->flashFile(file->getFile());

    //file->printFile();
    //file->printPages();
    //flasher->connect();
    //flasher->readChip();
    // uint32_t start_address = 0x8000000;
    // std::cout << std::hex << start_address << std::endl;
    // std::cout << std::hex << start_address + 256 << std::endl;



    // std::cout << std::hex <<(int) *((uint8_t *) &start_address + 0) << std::endl;
    // std::cout << std::hex <<(int) *((uint8_t *) &start_address + 1) << std::endl;
    // std::cout << std::hex <<(int) *((uint8_t *) &start_address + 2) << std::endl;
    // std::cout << std::hex <<(int) *((uint8_t *) &start_address + 3) << std::endl;




    //  uint8_t x = 0x11;
    //  uint8_t x2 = ~x;
    //  std::cout << std::hex << +x << " " << +((uint8_t)(x2)) << std::endl;
}

// reading an entire binary file
