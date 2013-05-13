//
//  main.cpp
//  RPi
//
//  Created by ed on 04/05/2013.
//  Copyright (c) 2013 ed. All rights reserved.
//

#include <iostream>
#include <time.h>
#include "sensor.h"
#include <GPIO/wiringPi.h>
using namespace std;

int main(){
    cout << "Data logging program."<<endl;

    if (wiringPiSetup () == -1){
        cout << "couldn't setup GPIO";
        exit (1) ;
    }
    time_t startTime=time(0);
    //create sensors
    temperature temp1(startTime);
    light light1(startTime);
    double currTemp;
    bool tempResult=temp1.actual(&currTemp);
    cout << "Temp is:" <<currTemp<< "\u00b0C" << endl;
    double currLight;
    bool lightResult=light1.actual(&currLight);
    cout << "Temp is:" <<currLight<< "\u00b0C" << endl;
    return 0;
}
