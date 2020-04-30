#ifndef UARTLink_h
#define UARTLink_h

#include <errno.h>  // Error integer and strerror() function
#include <fcntl.h>  // Contains file controls like O_RDWR
#include <stdio.h>
#include <string.h>
#include <termios.h>  // Contains POSIX terminal control definitions
#include <unistd.h>   // write(), read(), close()

#include <string>

class UARTLink {
   public:
    UARTLink(std::string port);
    int openPort();
    void closePort();

    void setFlags();

    void setSpeed();

    void writeData(uint8_t byte);
    //void writeData(char* buff, int size);

    int waitForResponse(uint64_t timeout);

    int available();
    uint8_t* getBuff();

    int com_port = 17;        /* /dev/ttyS0 (COM1 on windows) */
    int baud_rate = 9600;       /* 9600 baud */
    int buffer_size = 256;
    uint8_t* buffer;

   private:
    std::string port;
    int serial_port;
    struct termios tty;
    char read_buf[256];
};

#endif