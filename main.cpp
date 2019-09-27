/*************** AUTONOMOUS CAR ***************

COURSE: COEN 6711: MICROPROCESSORS AND THEIR APPLICATIONS
SCHOOL: CONCORDIA UNIVERSITY (DEPT. OF ELECTRICAL AND COMPUTER ENGINEERING)
DATE: DECEMBER 3, 2016

BY: GLADYS DANIEL
    HASSNAA EL-DERHALLI
    YASSMEEN ELDERHALLI
    ELNAZ GHAFARADLI
*/

//Include libraries
#include "mbed.h"
#include "ultrasonic.h"

//Initialize Interrupts, LED and PWM
InterruptIn bumpSensor(PTA12);
DigitalOut led(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
PwmOut leftmotor(PTD5);
PwmOut rightmotor(PTD0);

//Move forward procedure
void forward(){leftmotor = 1;
     rightmotor=1;
}

//Move backward procedure
void backward(){
    leftmotor = 0.3;
    rightmotor=0.3;
}

//Stop procedure
void stop(){
    leftmotor = 0.5;
    rightmotor=0.5;
}

//Turn right procedure
void right(){
    leftmotor = 0.7;
    rightmotor= 0.3;
}

//Turn left procedure
void left(){
    leftmotor = 0.3;
    rightmotor=0.7;
}

//ISR of bump sensors
void bump_sensor(void){
    if (bumpSensor==1) {
        led2=0;  //Green LED on Board is on
        stop();
        wait(1);
        backward();
        wait(1);
        left();
        wait(1);
        forward();
        led2=1; 
    }
}

//dist procedure that print the detected distance on screen
int distance;
void dist(int distance){
    printf("Distance %d mm\r\n", distance);
}
//Set the trigger pin to PTC9 and the echo pin to PTD3
//have updates every .1 seconds and a timeout after 1
//second, and call dist when the distance changes
ultrasonic mu(PTC9, PTD3, .1, 1, &dist);


//main function
int main() {
    led= 1;
    led2=1;
    led3=1;
    bumpSensor.rise(&bump_sensor);  //initialize the interrupt on the rising edge
    mu.startUpdates();              //start measuring the distance
    leftmotor = rightmotor=1;       //move forward
    
    while(1){
        led3=1;
        led2=1;
        led=1;
             
        //call checkDistance() as much as possible
        mu.checkDistance();
        distance = mu.getCurrentDistance();
        dist(distance);
        if (distance <600){
            led3=0; //Blue LED on board is on
            leftmotor=0.8 ;
            rightmotor=0.8; //slow down
            wait(0.5);
            leftmotor= 0.7 ;
            rightmotor=0.7; //slow down
            wait(0.5);
            leftmotor=0.6 ;
            rightmotor= 0.6; //slow down
            wait(0.5);
            stop();   //stop
            wait(0.5);
            backward();//move backward for 2 seconds
            wait(2);
            right();//turn right
            wait(1.5);
            forward();//move forward
            led3=1; //Blue LED on board is off
        }
    }
}