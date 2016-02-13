#ifndef UART_H
    #define UART_H

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

#endif
