
#include "STM32Flasher.h"

#include <iostream>

int main() {
    STM32Flasher *flasher = new STM32Flasher("/dev/ttyUSB0");
    //flasher->connect();
    //flasher->readChip();
    std::cout << std::endl;
}