extern "C" {

    #include "uart.h"

    /*
     * Opens UART
     * returns file descriptor for uart device
     * return -1 for errro
     */
    int initUART()
    {
        int uart0_filestream = -1;

        /* Open UART
         * Access modes:
         *       O_RDONLY - Open for reading only.
         *       O_RDWR - Open for reading and writing.
         *       O_WRONLY - Open for writing only.
         * Non-blocking mode:
         *       returns immediately with a failure status if no input available or can't write to output
         *       O_NDELAY / O_NONBLOCK
         * Additonal flag:
         *       prevents terminal device from becoming the controlling terminal for the process
         *       O_NOCTTY
         */
        //Open in non blocking read/write mode
        uart0_filestream = open("/dev/ttyAMA0", O_RDWR | O_NOCTTY | O_NDELAY);

        if (uart0_filestream == -1)
        {
             printf("Error - Unable to open UART.\n");
             return -1;
        }

        /* UART Options
         * Baud rate:- B9600, B115200, etc.
         * CSIZE:- CS5, CS6, CS7, CS8
         * CLOCAL - Ignore modem status lines
         * CREAD - Enable receiver
         * IGNPAR = Ignore characters with parity errors
         * ICRNL - Map CR to NL on input
         * (Use for ASCII comms where you want to auto correct end of line characters - don't use for bianry comms!)
         * PARENB - Parity enable
         * PARODD - Odd parity (else even)
         */
        struct termios options;
        tcgetattr(uart0_filestream, &options);
        options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
        options.c_iflag = IGNPAR;
        options.c_oflag = 0;
        options.c_lflag = 0;
        tcflush(uart0_filestream, TCIFLUSH);
        tcsetattr(uart0_filestream, TCSANOW, &options);

        return uart0_filestream;
    }
}
