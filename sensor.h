//
//  sensor.h
//  RPi
//
//  Created by Ed Longman on 04/05/2013.
//  Copyright (c) 2013 Ed Longman. All rights reserved.
//

#ifndef __RPi__sensor__
#define __RPi__sensor__

#include <iostream>
#include "atod.h"
#include "file.h"

//this enum associates the sensor with the port that it is connected to for ease of use!
enum sensorPins {temperaturePin=2,lightPin=1};

//this is the basic layer for reading and logging values
class sensor {
public:
	sensor();
    //use the time now to stamp the value input into the file
    //the function writes the reading into the *value
    bool log(clock_t timeNow,double *value);
    virtual bool actual(double *value);
protected:
    sensorPins sensorType;
    std::string sensorName;
    time_t startTime;
    double timeNow;
    atod serialBus;
    clock_t lastLogTime;
    double totalSinceLastLog;
    int loopsSinceLastLog;
    bool rawRead(double *value);
private:
	file logFile;
};


class temperature : public sensor {
public:
	sensorPins sensorType: temperaturePin;
	temperature(time_t startTime);
	//IN DEGREES C
    virtual bool actual(double *value);
};

class light : public sensor {
public:
	sensorPins sensorType: lightPin;
	light(time_t startTime);
	//IN LUX (LUMSNS/sqrFT)
    virtual bool actual(double *value);
};
#endif /* defined(__RPi__sensor__) */
