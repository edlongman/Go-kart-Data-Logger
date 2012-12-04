//
//  voltage output.cpp
//  RPi
//
//  Created by ed on 04/12/2012.
//  Copyright (c) 2012 ed. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include "util.h"
#include "atod.h"
using namespace std;
int main(int argc, const char * argv[])
{
    //input voltage equasion
    //                    value of chip * input voltage
    // analogue voltage = -----------------------------
    //                                 1024
    
    //setup chip
    atod sensor(14,10,13,12); //clock,shutdown,dout,din
    
    while (true) {
        int sensor_val=sensor.read();
        double voltage=(sensor_val*3.3)/1024;
        printf("Reading : %u     Voltage : %f",sensor.read(),voltage);
        putchar(0x0D); // go to start of the line
        short_wait();
    }
}