#include <cstdlib>
#include <iostream>
#include <cstring>
extern "C"
{
    #include "uart.h"
    #include <unistd.h> // used for sleep XXX
}
#include <opencv.hpp>
#include "shapeDetect.hpp"

using namespace cv;
using namespace std;

int main(void)
{
    // variables
    char txBuff[] = "hello";
    char rxBuff[255];
    int txRes = 1, rxRes = 1;

    // uart initialization
    int uart0_filestream = initUART();

    // open default camera 0
    VideoCapture capture(0);
    if(!capture.isOpened())
    {
        // TODO need to send a message to pcb
        cout<<"error with camera\n";
        return 1;
    }

    /* namedWindow("result",1);            // window to display image (for testing only) XXX */
    Mat edges;

    Mat src = cv::imread("../../test/data/shapes.png"); //XXX
    // main loop
    while(1)
    {
        Mat frame;                      // holds a captured frame
        capture >> frame;               // get a new frame from camera

        // process image
        /* cvtColor(frame, edges, CV_BGR2GRAY); */
        /* GaussianBlur(edges, edges, Size(7,7), 1.5, 1.5); */
        /* Canny(edges, edges, 0, 30, 3); */
        /* imshow("result", edges);        // display the proccessed image XXX */
        shapeDetect(frame);
        /* shapeDetect(src);//XXX */

        if(waitKey(30) >= 0) break;
    }


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
