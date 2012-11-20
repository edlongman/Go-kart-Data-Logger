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


//
// This is a software loop to wait
// a short while.
//
void short_wait()
{ int w;
    for (w=0; w<100; w++)
    { w++;
        w--;
    }
} // short_wait

//
// Simple SW wait loop
//
void long_wait(int v)
{ int w;
    while (v--)
        for (w=-800000; w<800000; w++)
        { w++;
            w--;
        }
} // long_wait

void clock(int pin, int value){ //this clocks the chip changing the bit specified beforehand
    digitalWrite(14,0); //turns pin off to write values
    short_wait();
    digitalWrite(pin, value); //writes the value to the pin
    digitalWrite(14,1); //turns pin on to clock processor
}
int clock(int pin){ //this clocks the chip reading the pin value beforehand
    digitalWrite(14,0); //turns pin off to read values
    short_wait();
    int val=digitalRead(pin); //reads the value from the pin
    digitalWrite(14,1); //turns pin on to clock chip
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
    pinMode(10,OUTPUT); // known as chip select or shutdown in the MCP data sheet
    digitalWrite(10,1); //to start it must be a falling edgeso set it up as running
    pinMode(14,OUTPUT); // CLK when this is turned on the chip will clock
    pinMode(12,OUTPUT); // Din
    //setup the pin for reading serial input
    pinMode(13,INPUT); //Dout the seral output stream
    
    int sgl=1;  //SGL/DIFF is high
    int odd=0;  //ODD/SIGN low for chanel 0 high for chanel 1
    int msbf=1; //MSBF bit is high
    
    
    //start initialising the chip for reading
    digitalWrite(10,0); //bring bit down to start read sequence
    clock(12,1);     //start bit
    clock(12,sgl);   //send SGL/DIFF
    clock(12,odd);   //like above
    clock(12,msbf);  //like above
    //done intitialisation
    
    //now do the reading of the values an convert the binary into decimal!!
    int dec_value=0; //dec_value must be defined first because it is to be used outside the loop
    for(int i=0,bit;i<10;i++,bit=NULL){ //start loop reset bit each loop
        bit = clock(13);
        cout<<"bit "<<i<<":"<<bit<<"\n";
        dec_value+=pow(2,10-i)*bit;
    }
    digitalWrite(10,1); //bring bit up to stop read sequence
    cout<<"decimal value of the resistor"<<dec_value<<"\n";
}