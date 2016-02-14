extern "C" {

    #include "uart.h"

    /*
     * Opens UART channel
     * returns file descriptor for uart device
     * returns 1 for error
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
             return 1;
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

    /*
     * Sends characters via uart
     * returns 0 for success
     * returns 1 for error
     */
    int uartSend(int uart0_filestream, char *txBuffer, int buffSize)
    {
        if (uart0_filestream != -1)
        {
            //Filestream, bytes to write, number of bytes to write
            int count = write(uart0_filestream, txBuffer, buffSize);
            if (count < 0)
            {
                printf("UART TX error\n");
                return 1;
            }
            else
                return 0;
        }
        else
            return 1;
    }

    /*
     * Reads characters from uart
     * returns 0 success
     * returns 1 error
     */
    int uartRead(int uart0_filestream, char *rxBuffer, int buffSize)
    {
        if (uart0_filestream != -1)
        {
            // Filestream, buffer to store in, number of bytes to read (max)
            int rxLength = read(uart0_filestream, rxBuffer, buffSize);
            if (rxLength <= 0)
            {
                // error no data
                return 1;
            }
            else
            {
                //Bytes received
                rxBuffer[rxLength] = '\0';
                return 0;
            }
        }
        else
            return 1;
    }
}
