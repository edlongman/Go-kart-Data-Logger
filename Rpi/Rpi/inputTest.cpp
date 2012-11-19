//
//  inputTest.cpp
//  Rpi
//
//  Created by ed on 17/11/2012.
//  Copyright (c) 2012 ed. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "GPIO/wiringPi.h"
using namespace std;

int main(int argc, const char * argv[])
{
    //testing my RPi GPIO
    cout << "GPIO Test Program.\n";
    
    if (wiringPiSetup () == -1){
        cout << "couldn't setup GPIO";
        exit (1) ;
    }
    //setup the pins for reading
    pinMode(1,INPUT);
    pinMode(4,INPUT);
    pinMode(5,INPUT);
    pinMode(6,INPUT);
    for(int i=0;i<10;i++){
        delay(1000);
        cout << "1,4,5,6 respectivly:"  << digitalRead (1) << ","
                                        << digitalRead (4) << ","
                                        << digitalRead (5) << ","
                                        << digitalRead (6) << " ;;;";
    }
}