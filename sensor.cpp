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

sensor::sensor(): logFile(sensorName), serialBus(14,10,13,12,sensorType){
	//file logFile(sensorName);
    //atod serialBus(14,10,13,12,sensorType);
    startTime=0;
    timeNow=0;
    lastLogTime=0;
	loopsSinceLastLog=0;
	totalSinceLastLog=0;
}
bool sensor::actual(double *value){
	return false;
}

bool sensor::log(clock_t timeNow, double *value){
	//read the value from the sensor with a time.
    bool readSucess=actual(value);
    double logTime = timeNow-startTime;
    //add the latest value to the total and register it in the loops
	totalSinceLastLog+=*value;
	loopsSinceLastLog++;
    //is the last log time over 2 seconds ago? So log it then.
    if((lastLogTime+0.2)<logTime){
    	//convert the average to a string
    	lastLogTime=logTime;
		string logLine;
		ostringstream sstream;
		sstream << logTime << "," << (totalSinceLastLog/loopsSinceLastLog);
		logLine= sstream.str();
		//log the line to the file
		logFile.append(logLine);
		//reset average variables
		cout << sensorName<<": writen logline: "<< logLine <<"    Avg of: "
				<< loopsSinceLastLog <<endl;
		loopsSinceLastLog=0;
		totalSinceLastLog=0;
    }
    return true;
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
    atod serialBus(14,10,13,12,sensorType);
}
bool temperature::actual(double *value){
	double voltage;
	bool success=rawRead(&voltage);
	*value=(voltage-0.5)*100;
	return success;
}


light::light(time_t startTime){
	sensorType=lightPin;
	sensorName="light";
    timeNow=startTime;
    atod serialBus(14,10,13,12,sensorType);
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
