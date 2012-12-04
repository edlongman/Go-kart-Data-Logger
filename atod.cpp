//
//  atod.cpp
//  RPi
//
//  Created by ed on 04/12/2012.
//  Copyright (c) 2012 ed. All rights reserved.
//

//this outputs a number between 0 and 1023 from an analogue sensor
#include <iostream>
#include <math.h>
#include "atod.h"
#include "util.h"
#include "GPIO/wiringPi.h"
using namespace std;

//please make sure wiring pi is initialised!
atod::atod(int, int, int, int){};
atod::atod(){ // if no pins are passed in then ask for them
    cout << "What number is the clock pin? : ";
    cin>>clock_pin;
    cout << "What number is the shutdown pin? : ";
    cin>>shutdown_pin;
    cout << "What number is the D out pin? : ";
    cin>>dout_pin;
    cout << "What number is the D in pin? : ";
    cin>>din_pin;
}
int atod::read(){ //returns return value
    pinMode(shutdown_pin,OUTPUT); // known as chip select or shutdown in the MCP data sheet
    digitalWrite(10,1); //to start it must be a falling edge so set it up as running
    
    pinMode(clock_pin,OUTPUT); // CLK when this is turned on the chip will clock
    pinMode(din_pin,OUTPUT); // Din
    //setup the pin for reading serial input
    pinMode(dout_pin,INPUT); //Dout the seral output stream
    
    int sgl=1;  //SGL/DIFF is high
    int odd=0;  //ODD/SIGN low for chanel 0 high for chanel 1
    int msbf=1; //MSBF bit is high
    
    
    //start initialising the chip for reading
    digitalWrite(shutdown_pin,0); //bring bit down to start read sequence
    clock(din_pin,1);     //start bit
    clock(din_pin,sgl);   //send SGL/DIFF
    clock(din_pin,odd);   //like above
    clock(din_pin,msbf);  //like above
    //done intitialisation
    
    
    short_wait();
    //now do the reading of the values an convert the binary into decimal!!
    int dec_value=0; //dec_value must be defined first because it is to be used outside the loop
    
    clock(13); //clock once for the null bit at the beginning
    for(int i=0,bit;i<10;i++,bit=NULL){ //start loop reset bit each loop
        bit = clock(dout_pin);
        int new_val=pow(2,9-i)*bit;
        dec_value+=new_val;
    }
    digitalWrite(shutdown_pin,1); //bring bit up to stop read sequence
    return dec_value;
}
    
void atod::clock(int pin, int value){ //this clocks the chip changing the bit specified beforehand
    digitalWrite(14,0); //turns pin off to write values-
    digitalWrite(pin, value); //writes the value to the pin
    digitalWrite(14,1); //turns pin on to clock processor
}
int atod::clock(int pin){ //this clocks the chip reading the pin value beforehand
    digitalWrite(14,0); //turns pin off to read values-
    int val=digitalRead(pin); //reads the value from the pin
    digitalWrite(14,1); //turns pin on to clock chip
    return val;
}