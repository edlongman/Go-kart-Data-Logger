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
#include "GPIO/wiringPi.h"
using namespace std;


//
// This is a software loop to wait
// a short while.
//
void short_wait()
{ int w;
    for (w=0; w<10; w++)
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

//please make sure wiring pi is initialised!
atod::atod(int clock, int shutdown, int dout, int din, int chan){
    clock_pin=clock;
    shutdown_pin=shutdown;
    dout_pin=dout;
    din_pin=din;
    channel=chan;
    cout << chan << endl;
};
atod::atod(){ // if no pins are passed in then ask for them
    cout << "What number is the clock pin? : ";
    cin>>clock_pin;
    cout << "What number is the shutdown pin? : ";
    cin>>shutdown_pin;
    cout << "What number is the D out pin? : ";
    cin>>dout_pin;
    cout << "What number is the D in pin? : ";
    cin>>din_pin;
    cout << "What channel to read from? : ";
    cin>>channel;
}
int atod::read(){ //returns actual chip value for the number of loops specified
    pinMode(shutdown_pin,OUTPUT); // known as chip select or shutdown in the MCP data sheet
    digitalWrite(10,1); //to start it must be a falling edge so set it up as running

    pinMode(clock_pin,OUTPUT); // CLK when this is turned on the chip will clock
    pinMode(din_pin,OUTPUT); // Din
    //setup the pin for reading serial input
    pinMode(dout_pin,INPUT); //Dout the seral output stream

    int sgl=1;  //SGL/DIFF is high
    int odd=channel-1;  //ODD/SIGN low for channel 0 high for channel 1
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

int atod::read(int loops){
    int cumulative_sensor=0;
    for (int ii=0;ii<loops;ii++){
        int sensor_val=read();
        cumulative_sensor+=sensor_val;
        delay(1); //slows the program down!
    }
    cumulative_sensor=cumulative_sensor/loops;
    return cumulative_sensor;
}

double atod::voltage(){
    return voltage(1);
}

double atod::voltage(int loops){
    int sensor_val=read(loops);
    double voltage=(sensor_val*3.3)/1024;
    return voltage;
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
