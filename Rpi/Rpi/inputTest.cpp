//
//  inputTest.cpp
//  Rpi
//
//  Created by ed on 17/11/2012.
//  Copyright (c) 2012 ed. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "wiringPi.h"
using namespace std;

void clock(int pin, int value){ //this clocks the chip changing the bit specified beforehand
    digitalWrite(1,0); //turns pin off to write values
    delay(1); // alows the chip to clock, much too long but I'm lazy
    digitalWrite(pin, value); //writes the value to the pin
    digitalWrite(1,1); //turns pin on to clock processor
}
int clock(int pin){ //this clocks the chip reading the pin value beforehand
    digitalWrite(1,0); //turns pin off to read values
    delay(1); // alows the chip to clock, much too long but I'm lazy
    int val=digitalRead(pin); //reads the value from the pin
    digitalWrite(1,1); //turns pin on to clock chip
    return val;
}

int main(int argc, const char * argv[])
{
    //testing my RPi GPIO
    cout << "GPIO Test Program.\n";
    
    if (wiringPiSetup () == -1){
        cout << "couldn't setup GPIO";
        exit (1) ;
    }
    pinMode(6,OUTPUT); // known as chip select or shutdown in the MCP data sheet
    digitalWrite(6,1); //to start it must be a falling edgeso set it up as running
    pinMode(1,OUTPUT); // CLK when this is turned on the chip will clock
    pinMode(5,OUTPUT); // Din
    //setup the pin for reading serial input
    pinMode(4,INPUT); //Dout the seral output stream
    
    int sgl=1;  //SGL/DIFF is high
    int odd=0;  //ODD/SIGN low for chanel 0 high for chanel 1
    int msbf=1; //MSBF bit is high
    
    
    //start initialising the chip for reading
    clock(5,1);     //start bit
    clock(5,sgl);   //send SGL/DIFF
    clock(5,odd);   //like above
    clock(5,msbf);  //like above
    //done intitialisation
    
    //now do the reading of the values an convert the binary into decimal!!
    int dec_value; //dec_value must be defined first because it is to be used outside the loop
    for(int i=0,bit;i<10;i++,bit=NULL){ //start loop reset bit each loop
        bit = clock(4);
        dec_value+=pow(2,i)*bit;
    }
    cout<<"decimal value of the resistor"<<dec_value;
}