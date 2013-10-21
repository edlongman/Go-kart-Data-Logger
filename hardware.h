#include <LiquidCrystal_I2C_ST7032i.h>

//
//  sensor.h
//  RPi
//
//  Created by Ed Longman on 04/05/2013.
//  Copyright (c) 2013 Ed Longman. All rights reserved.
//

#ifndef __RPi__sensor__
#define __RPi__sensor__

#include "file.h"


//this enum associates the sensor with the port that it is connected to for ease of use!
enum sensorPins {temperaturePin=2,lightPin=1,wheelspeedPin=3};


//this is the basic layer for reading and logging values
class sensor {
public:
    sensor(String sensorName, sensorPins sensorType);
    //use the time now to stamp the value input into the file
    //the function writes the reading into the *value
    bool log(double timeNow,double *value);
    virtual bool actual(double *value);
    String name();
protected:
    sensorPins sensorType;
    String sensorName;
    int startTime;
    double timeNow;
    int logsSoFar;
    double totalSinceLastLog;
    int loopsSinceLastLog;
    bool rawRead(double *value);
    save logFile;
};


class temperature : public sensor {
public:
    sensorPins sensorType: temperaturePin;
    temperature(double startTime);
    //IN DEGREES C
    virtual bool actual(double *value);
};

class light : public sensor {
public:
    sensorPins sensorType: lightPin;
    light(double startTime);
    //IN LUX (LUMSNS/sqrFT)
    virtual bool actual(double *value);
};

class irTemperature : public sensor {
public:
    sensorPins sensorType: lightPin;
    irTemperature(double startTime);
    //IN LUX (LUMSNS/sqrFT)
    virtual bool actual(double *value);
};

class wheelspeed : public sensor {
public:
    sensorPins sensorType: wheelspeedPin;
    wheelspeed(double startTime);
    virtual bool actual(double *value);
    void logWheelRotation();
    bool actualSpeed(double *value);
private:
    long lastReadWheel;
    long lastButOneReadWheel;
    long totalWheelRotations;
};

class McpPin{
public:
    McpPin(int _port,int _pin);
    McpPin(int id);
    McpPin();
    int getPortAddress();
    int getPinAddress();
private:
    int port;
    int pin;
};

//display mapp
//   **16**      **11**
//  *      *    *      *
// 18      15  12      10
//  *      *    *      *
//   **17**      **7***
//  *      *    *      *
//  1      3    5      8
//  *      *    *      *
//   **2***  *4  **6***  *9
class display7segment{
public:
    display7segment();
    display7segment(int expanderAddress);
    void setValue(double value);
private:
    void setPin(bool on);
    int expanderAddress;
    //send this to the chip and the digit applies like on diagram
//   **1**   
//  *     *  
//  2     3 
//  *     *  
//   **4**    
//  *     *  
//  5     6  
//  *     * 
//   **7**  *8
    int numbersAndPinSegment1[10][7];
    int numbersAndPinSegment2[10][7];
    McpPin pinAndMcpIO[18];
    McpPin dpPosition;
};



#endif /* defined(__RPi__sensor__) */
