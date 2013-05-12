//
//  sensor.cpp
//  RPi
//
//  Created by Ed Longman on 04/05/2013.
//  Copyright (c) 2013 Ed Longman. All rights reserved.
//

#include <time.h>
#include <sstream>
#include "file.h"
#include "sensor.h"

using namespace std;

sensor::sensor(){
	logFile file(sensorName);
    serialBus atod(14,10,13,12,sensorType);
    startTime=time(0);
    timeNow=startTime;
    lastSensorTime=timeNow;
}
bool sensor::actual(double *value){
	return false;
}

bool sensor::log(clock_t timeNow, double *value){
    bool readSucess=actual(value);
    double logTime = startTime+ timeNow/CLOCKS_PER_SEC;
    string logLine;
    ostringstream sstream;
    sstream << logTime << "," << value;
    logLine= sstream.str();
    logFile.append(logLine);
}

//this is the function that will need to be dramatically changed
//when hardware is changed as this is hardware dependant
bool sensor::rawRead(double *value){
	*value=serialBus.voltage();
	return true;
}
temperature::temperature(time_t startTime){
	sensorType=temperaturePin;
	sensorName="temperature";
    timeNow=startTime;
}
bool temperature::actual(double *value){
	double voltage;
	bool success=rawRead(&voltage);
	*value=(voltage*10)+50;
	return success;
}


light::light(time_t startTime){
	sensorType=lightPin;
	sensorName="light";
    timeNow=startTime;
}
bool light::actual(double *value){
	double voltage;
	bool success=rawRead(&voltage);
	if(!(voltage==0.0)){
		//sensor resitsance it 10000ohms when at 0LUX
		double resistance=1000.0*(3.3-voltage)/voltage;
		*value=100/resistance;
		return true;
	}else{
		return false;
	}
}
