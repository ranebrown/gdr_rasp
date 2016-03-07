#include <cstdlib>
#include <cstring>
extern "C"
{
    #include "uart.h"
    #include <unistd.h> // used for sleep XXX
    #include <wiringPi.h> // gpio
}
#include "master.hpp"
#include "shapeDetect.hpp"
#include "colorDetect.hpp"
#include "dist2obj.hpp"

int main(void)
{
    // variables
    char val = 0;
    /* char *txBuff = &val; */
    /* char rxBuff[255]; */
    /* int txRes = 1, rxRes = 1; */
    float dist = 0;

    // uart initialization
    /* int uart0_filestream = initUART(); */

    // gpio initialization
    wiringPiSetup();
    pinMode(0,OUTPUT);
    digitalWrite(0,LOW);

    // open default camera 0
    VideoCapture capture(0);
    if(!capture.isOpened())
    {
        // TODO need to send a message to pcb
        cout<<"error with camera\n";
        return 1;
    }

    // main loop
    while(1)
    {
        Mat frame;                      // holds a captured frame
        capture >> frame;               // get a new frame from camera

        // process image
        Mat red = colorDetect(frame);
        Mat out = shapeDetect(red, &dist);

        // display processed image (testing only) XXX
        /* imshow("result", out); */

        // print distance
        /* printf("%f\n",dist); */

        // convert distance to char so it can be sent over uart
        val = (char)dist;
        if(val <= 90 && val > 20)
        {
            digitalWrite(0,HIGH);

            // send data over uart
            /* txRes = uartSend(uart0_filestream, txBuff, strlen(txBuff)); */
            /* if(txRes != 0) */
            /* { */
            /*     printf("bad val\n"); */
            /*     // TODO error handling */
            /* } */
        }
        digitalWrite(0,LOW);
        dist = 0;

        // if any key is pressed exit
        /* if(waitKey(30) >= 0) break; */
    }


    // send data over uart
    /* txRes = uartSend(uart0_filestream, txBuff, strlen(txBuff)); */
    /* if(txRes != 0) */
    /* { */
    /*     cout<<"Error sending uart data\n"; */
    /*     return 0; */
    /* } */

    // delay so data can be echoed back
    /* sleep(1); */

    // read uart data
    /* rxRes = uartRead(uart0_filestream, rxBuff, sizeof(rxBuff)); */
    /* if(rxRes != 0) */
    /* { */
    /*     cout<<"Error reading uart data\n"; */
    /*     return 1; */
    /* } */

    /* // print data stored in rx buffer */
    /* int i = 0; */
    /* while(rxBuff[i] != '\0') */
    /* { */
    /*     cout<<rxBuff[i]; */
    /*     i++; */
    /* } */
    /* cout<<"\n"; */

    /* // close uart */
    /* close(uart0_filestream); */

    return 0;
}
