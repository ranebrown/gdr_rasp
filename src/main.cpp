#include <cstdlib>
#include <cstring>
extern "C"
{
    #include "uart.h"
    #include <time.h>
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
    float dist = 0;
    int xx = 0;
    struct timespec tim, tim2;      // tim holds the desired sleep time, tim2 is filled with the remaining time if error occurs
    tim.tv_sec  = 0;
    tim.tv_nsec = 500000000L;       // 1/2 second sleep time

    // gpio initialization
    wiringPiSetup();
    pinMode(0,OUTPUT);      // stop sign signal
    digitalWrite(0,LOW);
    pinMode(7,OUTPUT);      // vibration motor signal
    digitalWrite(0,LOW);
    pinMode(15,INPUT);      // power switch
    pullUpDnControl(15, PUD_DOWN);

    // open default camera 0
    VideoCapture capture(0);
    if(!capture.isOpened())
    {
        // TODO need to send a message to pcb
        cout<<"ERROR: webcam problem\n";
        return 1;
    }

    // read current status of power switch
    xx = digitalRead(15);

    // 3 1/2 second vibrations to indicate power on
    digitalWrite(7,HIGH);
    if(nanosleep(&tim,&tim2) == EXIT_FAILURE)
        return EXIT_FAILURE;
    digitalWrite(7,LOW);
    if(nanosleep(&tim,&tim2) == EXIT_FAILURE)
        return EXIT_FAILURE;
    digitalWrite(7,HIGH);
    if(nanosleep(&tim,&tim2) == EXIT_FAILURE)
        return EXIT_FAILURE;
    digitalWrite(7,LOW);
    if(nanosleep(&tim,&tim2) == EXIT_FAILURE)
        return EXIT_FAILURE;
    digitalWrite(7,HIGH);
    if(nanosleep(&tim,&tim2) == EXIT_FAILURE)
        return EXIT_FAILURE;
    digitalWrite(7,LOW);

    // play power on sound track over speaker
    system("omxplayer -o local /home/pi/capstone/gdr_rasp/audio/ready2.mp3");

    // main loop
    while(1)
    {
        // check for shutdown command
        xx = digitalRead(15);
        if(xx == 0)
        {
            // 2 1/2 second vibrations to indicate power off
            digitalWrite(7,HIGH);
            if(nanosleep(&tim,&tim2) == EXIT_FAILURE)
                return EXIT_FAILURE;
            digitalWrite(7,LOW);
            if(nanosleep(&tim,&tim2) == EXIT_FAILURE)
                return EXIT_FAILURE;
            digitalWrite(7,HIGH);
            if(nanosleep(&tim,&tim2) == EXIT_FAILURE)
                return EXIT_FAILURE;
            digitalWrite(7,LOW);

            // play power down audio file
            system("omxplayer -o local /home/pi/capstone/gdr_rasp/audio/power_off.mp3");

            // shutdown raspberry pi
            system("shutdown now");
        }

        Mat frame;                      // holds a captured frame
        capture >> frame;               // get a new frame from camera

        // process image
        Mat red = colorDetect(frame);
        Mat out = shapeDetect(red, &dist);

        // display processed image (testing only) XXX
        /* imshow("result", out); */

        // print distance (testing only) XXX
        /* printf("%f\n",dist); */

        // write pin high if within 6 ft of stop sign
        if(val <= 90 && val > 20)
        {
            digitalWrite(0,HIGH);
        }
        digitalWrite(0,LOW);
        dist = 0;

        // if any key is pressed exit (needed for displaying image) XXX
        /* if(waitKey(30) >= 0) break; */
    }

    return 0;
}
