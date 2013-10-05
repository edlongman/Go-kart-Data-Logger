//
//  sensor.cpp
//  RPi
//
//  Created by Ed Longman on 04/05/2013.
//  Copyright (c) 2013 Ed Longman. All rights reserved.
//

#include <sstream>
#include "file.h"
#include "sensor.h"
#include <Wire.h>

#define DS1307_I2C_ADDRESS 0x68
#define I2C_WRITE Wire.write 
#define I2C_READ Wire.read
byte zero;
byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
using namespace std;

sensor::sensor(String sensorName, sensorPins sensorType):logFile(sensorName){
    startTime=0;
    timeNow=0;
    logsSoFar=0;
	loopsSinceLastLog=0;
	totalSinceLastLog=0;
}
bool sensor::actual(double *value){
	return false;
}

bool sensor::log(double timeNow, double *value){
	//read the value from the sensor with a time.
    bool readSucess=actual(value);
    long logTime = timeNow-startTime;
    //add the latest value to the total and register it in the loops
	totalSinceLastLog+=*value;
	loopsSinceLastLog++;
    //is the last log time over 0.2 seconds ago? So log it then.
    if((logsSoFar*0.2+0.2)<logTime){
    	//convert the average to a string
    	logsSoFar++;
		String logLine;
                char* sensorAvg;
                dtostrf(totalSinceLastLog/loopsSinceLastLog,5,3,sensorAvg);
		logLine = logTime + "," + String(sensorAvg);
		//log the line to the file
		logFile.append(logLine);
		//reset average variables
		//could put log serial output here!
		loopsSinceLastLog=0;
		totalSinceLastLog=0;
    }
    return true;
}

//this is the function that will need to be dramatically changed
//when hardware is changed as this is hardware dependant
bool sensor::rawRead(double *value){
	*value=analogRead(sensorType);
	return true;
}
temperature::temperature(double startTime):sensor("temperature",temperaturePin){
	sensorType=temperaturePin;
	sensorName="temperature";
    timeNow=startTime;
    save logFile(sensorName);
}
bool temperature::actual(double *value){
	double voltage;
	bool success=rawRead(&voltage);
	*value=(voltage-0.5)*100;
	return success;
}


light::light(double startTime):sensor("light",lightPin){
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

wheelspeed::wheelspeed(double startTime):sensor("wheelspeed",wheelspeedPin){
	sensorType=wheelspeedPin;
	sensorName="wheelspeed";
    timeNow=startTime;
}
bool wheelspeed::actual(double *value){
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

void logWheelRotation(){
    lastButOneReadWheel=lastReadWheel;
    lastReadWheel=micros();
    totalWheelDistance+=wheelCircumference;
}
