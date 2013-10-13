//
//  sensor.cpp
//  RPi
//
//  Created by Ed Longman on 04/05/2013.
//  Copyright (c) 2013 Ed Longman. All rights reserved.
//

#include "file.h"
#include "hardware.h"
#include <Wire.h>
#include <math.h>

#define wheelCircumference 1214 //mm
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
    lastButOneReadWheel=micros();
    totalWheelRotations=0;
    lastReadWheel=micros();
}
bool wheelspeed::actual(double *value){
    long timeGap=lastReadWheel-lastButOneReadWheel;
    if(timeGap<2000000){
      //try and calculate speed
      *value=(wheelCircumference*0.000001*0.6213711922373)/(timeGap/3600000000);
    }
    else{
       //not moving or less than ~1mph
      return false; 
    }
}

void wheelspeed::logWheelRotation(){
    lastButOneReadWheel=lastReadWheel;
    totalWheelRotations++;
    lastReadWheel=micros();
}

McpPin::McpPin(byte _port, byte _pin){
    port=_port;
    pin=_pin;
}
McpPin::McpPin(int id){
  port=(byte)6;
  if(id>7){
      id=id-7;
      port=(byte)7;
  }
  pin=pow(2,id);
  
}
byte McpPin::getPortAddress(){
    return port;
}
byte McpPin::getPinAddress(){
    return pin;
}

//display map
//   **16**      **11**
//  *      *    *      *
// 18      15  12      10
//  *      *    *      *
//   **17**      **7***
//  *      *    *      *
//  1      3    5      8
//  *      *    *      *
//   **2***  *4  **6***  *9

display7segment::display7segment(int address){
    expanderAddress=address;
    int numbersAndPinSegment1[][7]={
      {1,2,3,15,16,18},//0
      {3,15},//1
      {1,2,15,16,17},//2
      {2,3,15,16,17},//3
      {3,15,17,18},//4
      {2,3,16,17,18},//5              
      {1,2,3,15,16,17},//6
      {3,15,16},//7
      {1,2,3,15,16,17,18},//8
      {3,15,16,17,18} //9
    };
    int numbersAndPinSegment2[][7]={
      {5,6,8,10,11,12},//0
      {8,10},//1
      {5,6,7,10,11},//2
      {6,7,8,10,11},//3
      {7,8,10,12},//4
      {6,7,8,11,12},//5
      {5,6,7,8,10,11},//6
      {8,10,11},//7
      {5,6,7,8,10,11,12},//8
      {7,8,10,11,12} //9
    };
    pinAndMcpIO[1]=McpPin(0);
    pinAndMcpIO[2]=McpPin(1);
    pinAndMcpIO[3]=McpPin(2);
    pinAndMcpIO[4]=McpPin(3);
    pinAndMcpIO[5]=McpPin(4);
    pinAndMcpIO[6]=McpPin(5);
    pinAndMcpIO[7]=McpPin(6);
    pinAndMcpIO[8]=McpPin(7);
    pinAndMcpIO[9]=McpPin(13);
    pinAndMcpIO[10]=McpPin(12);
    pinAndMcpIO[11]=McpPin(9);
    pinAndMcpIO[12]=McpPin(10);
    pinAndMcpIO[15]=McpPin(11);
    pinAndMcpIO[16]=McpPin(14);
    pinAndMcpIO[17]=McpPin(15);
    pinAndMcpIO[18]=McpPin(16);
    
    //setup MCP
    Wire.beginTransmission(expanderAddress);
    Wire.write(0x00);//IODIRA
    Wire.write(0x00);//Set Outputs
    Wire.endTransmission();
    Wire.beginTransmission(expanderAddress);
    Wire.write(0x01);//IODIRB
    Wire.write(0x00);//Set Outputs
    Wire.endTransmission();
    
}

void display7segment::setValue(double value){
  McpPin dpPosition=pinAndMcpIO[8];
  //get first digit
  if(value<10){
      //show to 1d.p
      value=value*10;
      dpPosition=pinAndMcpIO[5];
  }
  
  //get current pin states
  Wire.beginTransmission(expanderAddress);
  Wire.write(0x12);
  Wire.requestFrom(expanderAddress, 1);
  byte GpioAByte = Wire.receive();
  Wire.endTransmission();
  Wire.beginTransmission(expanderAddress);
  Wire.write(0x13);
  Wire.requestFrom(expanderAddress, 1);
  byte GpioBByte = Wire.receive();
  Wire.endTransmission();
  int firstVal=(int)value;
  for(int i=0;i<size(numbersAndPinSegment1[firstVal]);i++){
      //loop through pins checking whether it's for register A or B and add them to the byte
  }
  //get the second digit and round
  int secondVal=(int)(value-firstVal+0.5);
  
  
}
