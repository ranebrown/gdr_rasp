#ifndef UART_H
    #define UART_H

    // pi uart funcitons based on info found at:
    // http://www.raspberry-projects.com/pi/programming-in-c/uart-serial-port/using-the-uart

    #include <stdio.h>
    #include <unistd.h>         //Used for UART
    #include <fcntl.h>          //Used for UART
    #include <termios.h>        //Used for UART


    /*
     * Opens UART
     * returns file descriptor for uart device
     * return -1 for errro
     */
    int initUART();

    /*
     * Sends characters via uart
     * returns 0 for success
     * returns 1 for error
     */
    int uartSend(int uart0_filestream, char *txBuffer, int buffSize);

    /*
     * Reads characters from uart
     * returns 0 success
     * returns 1 error
     */
    int uartRead(int uart0_filestream, char *rxBuffer, int buffSize);

#endif
