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
#include <wiringPi.h>
using namespace std;

int main(){
    cout << "Data logging program."<<endl;

    if (wiringPiSetup () == -1){
        cout << "couldn't setup GPIO";
        //exit (1) ;
        return 0;
    }
    time_t startTime=time(0);
    //start clock
    //create sensors
    temperature temp1(startTime);
    light light1(startTime);
    clock_t currTime=startTime+clock()/CLOCKS_PER_SEC;
	while ((currTime)<(startTime+2)){
		currTime=clock()/CLOCKS_PER_SEC;
		double currTemp;
		bool tempResult=temp1.log(currTime,&currTemp);
		double currLight;
		bool lightResult=light1.log(currTime,&currLight);
    }
    return 0;
}
