//
//  buttonTest.cpp
//  Rpi
//
//  Created by ed on 20/11/2012.
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
    pinMode(0,INPUT);
    int i=0; //number of times the button has been pressed
    while(true){
        if(digitalRead(0)==1){
            cout << "pressed " << i <<".\n";
            i++;
            while (digitalRead(0)==1) {}
        }
    }
}