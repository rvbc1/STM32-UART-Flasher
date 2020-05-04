

#include "FileReader.h"
#include "STM32Flasher.h"
#include "vector"

int main(int argc, char *argv[]) {
    if (argc == 3) {
        STM32Flasher *flasher = new STM32Flasher(std::string(argv[2]));

        FileReader *file = new FileReader(argv[1]);

        flasher->flashFile(file->getFile());
    } else {
        std::cout << "Invalid parameters" << std::endl;
        std::cout << "Right use " << argv[0] << " <file> <port>" << std::endl;
        std::cout << "Example " << argv[0] << " expl.bin ttyUSB0" << std::endl;
    }

    // FileReader *file = /dev/ttyUSB0
    // new FileReader("F103.binary");
    // file->printFile();
    // file->printPages();
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
