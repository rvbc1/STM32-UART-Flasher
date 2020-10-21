#include "STM32Flasher.h"

#include <chrono>
#include <iostream>

void printInfo(std::string info, uint8_t writeNewLine = true) {
    if (writeNewLine) {
        std::cout << std::dec << info << std::endl;
    } else {
        std::cout << std::dec << info;
    }
}

STM32Flasher::STM32Flasher(std::string port, int baudRate) {
    uart = new UARTLink(port, baudRate);
    is_port_open = uart->openPort();
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
    if (is_port_open) {
        uart->addDataToBufferTX(START_CODE);
        uart->writeData();
        is_connection_open = checkResponse(ACK_AT_BEGIN);

#if defined(__linux__) || defined(__FreeBSD__) || (__Windows__)
        if (is_connection_open == false) {
            printInfo("Cannot start comunication with uC");
            printInfo("Check wiring & set bootloader at chip");
            exit(1);
        }
#endif
    }
}

void STM32Flasher::getCommand() {
    if (is_port_open && is_connection_open) {
        writeCommand(COMMAND_GET);
        checkResponse();

        const uint8_t available_cmds_start_pos = 3;
        const uint8_t available_cmds_size = buffer->data[1];

        const uint8_t bootloader_version_1 = buffer->data[2] / 16;
        const uint8_t bootloader_version_2 = buffer->data[2] % 16;

        printInfo("Bootloader ver. " + std::to_string(bootloader_version_1) + "." + std::to_string(bootloader_version_2) + ", " + std::to_string(available_cmds_size) + " commands available:");

        for (int i = 0; i < available_cmds_size; i++) {
            printInfo("\t", false);
            switch (buffer->data[available_cmds_start_pos + i]) {
                case COMMAND_GET:
                    printInfo("GET");
                    break;
                case COMMAND_GET_VERSION:
                    printInfo("GET_VERSION");
                    break;
                case COMMAND_GET_ID:
                    printInfo("GET_ID");
                    break;
                case COMMAND_READ_MEMORY:
                    printInfo("READ_MEMORY");
                    break;
                case COMMAND_GO:
                    printInfo("GO");
                    break;
                case COMMAND_WRITE_MEMORY:
                    printInfo("WRITE_MEMORY");
                    break;
                case COMMAND_ERASE:
                    printInfo("ERASE");
                    eraseChipMode = BASIC_ERASE_MODE;
                    break;
                case COMMAND_EXTENDED_ERASE:
                    printInfo("EXTENDED_ERASE");
                    eraseChipMode = EXTENDED_ERASE_MODE;
                    break;
                case COMMAND_WRITE_PROTECT:
                    printInfo("WRITE_PROTECT");
                    break;
                case COMMAND_WRITE_UNPROTECT:
                    printInfo("WRITE_UNPROTECT");
                    break;
                case COMMAND_READOUT_PROTECT:
                    printInfo("READOUT_PROTECT");
                    break;
                case COMMAND_READOUT_UNPROTECT:
                    printInfo("READOUT_UNPROTECT");
                    break;
            }
        }
    }
}

void STM32Flasher::getVersionCommand() {
    if (is_port_open && is_connection_open) {
        writeCommand(COMMAND_GET_VERSION);
        checkResponse();
    }
}

void STM32Flasher::getIdCommand() {
    if (is_port_open && is_connection_open) {
        writeCommand(COMMAND_GET_ID);
        checkResponse();
    }
}

void STM32Flasher::readMemoryCommand(uint32_t start_address, uint8_t length) {
    if (is_port_open && is_connection_open) {
        writeCommand(COMMAND_READ_MEMORY);
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
}

void STM32Flasher::goCommand(uint32_t address) {
    if (is_port_open && is_connection_open) {
        writeCommand(COMMAND_GO);
        checkResponse();
        writeAddress(address);
        checkResponse();
    }
}

void STM32Flasher::flashCommand(uint32_t start_address, uint8_t *buffer, uint16_t length) {
    if (is_port_open && is_connection_open) {
        writeCommand(COMMAND_WRITE_MEMORY);
        checkResponse();
        writeAddress(start_address);
        if (!checkResponse(ACK_AT_BEGIN)) {
            exit(0);
        }
        uart->addDataToBufferTX(length);
        uint8_t xor_checksum = 0x00;
        xor_checksum ^= length;
        for (int i = 0; i <= length; i++) {
            uart->addDataToBufferTX(buffer[i]);
            xor_checksum ^= buffer[i];
        }
        uart->addDataToBufferTX(xor_checksum);
        uart->writeData();
        if (!checkResponse(ACK_AT_BEGIN)) {
            exit(0);
        }
    }
}

void STM32Flasher::eraseCommand() {
    if (is_port_open && is_connection_open) {
        if (eraseChipMode == BASIC_ERASE_MODE) {
            writeCommand(COMMAND_ERASE);
            checkResponse();

            writeCommand(COMMAND_FULL_CHIP_ERASE);
            checkResponse();
        } else if (eraseChipMode == EXTENDED_ERASE_MODE) {
            writeCommand(COMMAND_EXTENDED_ERASE);
            checkResponse();

            uart->addDataToBufferTX(COMMAND_FULL_CHIP_ERASE);
            uart->addDataToBufferTX(COMMAND_FULL_CHIP_ERASE);
            uart->addDataToBufferTX(0x00);
            uart->writeData();
            checkResponse(ACK_AT_END, 10000);
        }
    }
}

void STM32Flasher::writeCommand(uint8_t command) {
    if (is_port_open && is_connection_open) {
        uart->addDataToBufferTX(command);
        uart->addDataToBufferTX(~command);
        uart->writeData();
    }
}

void STM32Flasher::flashFile(FileReader::file_struct file) {
    flashFile(file.data, file.size);
}

void STM32Flasher::flashFile(uint8_t *data, uint16_t size) {
    if (is_port_open && is_connection_open) {
        uint16_t pages = size / 256;
        uint16_t unfull_page_size = size - pages * 256;
        uint32_t address = 0;
        eraseCommand();
        for (int page_counter = 0; page_counter < pages; page_counter++, address += 256) {
            std::cout << "\rFlashing " << std::dec << page_counter << "/" << pages;
            flashCommand(0x8000000 + address, &data[address], 0xff);
        }
        std::cout << "\rFlashing " << std::dec << pages << "/" << pages << std::endl;
        flashCommand(0x8000000 + address, &data[address], unfull_page_size - 1);

        goCommand(0x8000000);
    }
}

void STM32Flasher::writeAddress(uint32_t address) {
    uint8_t xor_checksum = 0x00;
    for (uint32_t i = 0; i < sizeof(uint32_t); i++) {
        uint8_t msb_byte = *((uint8_t *)&address + sizeof(uint32_t) - i - 1);
        uart->addDataToBufferTX(msb_byte);
        xor_checksum ^= msb_byte;
    }
    uart->addDataToBufferTX(xor_checksum);
    uart->writeData();
}

uint8_t STM32Flasher::checkResponse(ack_pos pos, uint64_t timeout) {
    uint8_t ack_at_right_pos = false;

    if (is_port_open) {
        if ((pos == ACK_AT_BEGIN) && (pos == NONE_ACK)) {
            buffer->size = uart->waitForFirstResponse(timeout);
        } else {
            buffer->size = uart->waitForResponse(timeout);
        }

        if (buffer->size > 0) {
            switch (pos) {
                case NONE_ACK:
                    ack_at_right_pos = true;
                    break;
                case ACK_AT_BEGIN:
                    if (buffer->data[0] == ACK) {
                        ack_at_right_pos = true;
                    } else {
                        ack_at_right_pos = false;
                    }
                    break;
                case ACK_AT_END:
                    if (buffer->data[buffer->size - 1] == ACK) {
                        ack_at_right_pos = true;
                    } else {
                        ack_at_right_pos = false;
                    }
                    break;
                case ACK_AT_BEGIN_AND_END:
                    if ((buffer->data[0] == ACK) && (buffer->data[buffer->size - 1] == ACK)) {
                        ack_at_right_pos = true;
                    } else {
                        ack_at_right_pos = false;
                    }
                    break;
            };
        }
    }
    return ack_at_right_pos;
}

void STM32Flasher::connect() {
}

void STM32Flasher::readChip() {
}