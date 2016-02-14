#include <cstdlib>
#include <iostream>
#include <cstring>
extern "C"
{
    #include "uart.h"
    #include <unistd.h>
}

using namespace std;

int main(void)
{
    // variables
    char txBuff[] = "hello";
    char rxBuff[255];
    int txRes = 1, rxRes = 1;

    // uart initialization
    int uart0_filestream = initUART();

    // send data over uart
    txRes = uartSend(uart0_filestream, txBuff, strlen(txBuff));
    if(txRes != 0)
    {
        cout<<"Error sending uart data\n";
        return 0;
    }

    // delay so data can be echoed back
    sleep(1);

    // read uart data
    rxRes = uartRead(uart0_filestream, rxBuff, sizeof(rxBuff));
    if(rxRes != 0)
    {
        cout<<"Error reading uart data\n";
        return 1;
    }

    // print data stored in rx buffer
    int i = 0;
    while(rxBuff[i] != '\0')
    {
        cout<<rxBuff[i];
        i++;
    }
    cout<<"\n";

    // close uart
    close(uart0_filestream);

    return 0;
}
