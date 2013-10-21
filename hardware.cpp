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
#include <LiquidCrystal_I2C_ST7032i.h>

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
String sensor::name(){
    return sensorName;
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

irTemperature::irTemperature(double startTime):sensor("irTemp",lightPin){
    sensorType=lightPin;
    sensorName="light";
    timeNow=startTime;
}
bool irTemperature::actual(double *value){
    byte highData = 0x00;
    byte lowData = 0x00;
    
    Wire.beginTransmission(0x00);
    Wire.write(0x07);
    Wire.endTransmission(false);//keep alive to do byte read
    
    //recieve that temperature
    Wire.requestFrom(0x00,2);
    lowData = Wire.read();
    highData = Wire.read();
    
    //do the maths
    double kelvin = 0x00;
    kelvin = (double)(((highData & 0x007F)<<8)+lowData)*0.02-0.01;
    return kelvin-273.15;
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
    *value=totalWheelRotations*wheelCircumference;
    return true;
}
bool wheelspeed::actualSpeed(double *value){
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

McpPin::McpPin(int _port, int _pin){
    port=_port;
    pin=_pin;
}
McpPin::McpPin(int id){
  port=0x12;
  if(id>7){
      id=id-7;
      port=0x13;
  }
  pin=0x01<<id;
  
}
McpPin::McpPin(){
    pin=0x00;
}
int McpPin::getPortAddress(){
    return port;
}
int McpPin::getPinAddress(){
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
display7segment::display7segment(){}
display7segment::display7segment(int address){
    expanderAddress=address;
    pinAndMcpIO[1]=McpPin(0x12,0x40);
    pinAndMcpIO[2]=McpPin(0x12,0x80);
    pinAndMcpIO[3]=McpPin(0x13,0x01);
    pinAndMcpIO[4]=McpPin(0x13,0x02);
    pinAndMcpIO[5]=McpPin(0x13,0x04);
    pinAndMcpIO[6]=McpPin(0x13,0x08);
    pinAndMcpIO[7]=McpPin(0x13,0x10);
    pinAndMcpIO[8]=McpPin(0x13,0x20);
    pinAndMcpIO[9]=McpPin(0x13,0x40);
    pinAndMcpIO[10]=McpPin(0x12,0x02);
    pinAndMcpIO[11]=McpPin(0x12,0x04);
    pinAndMcpIO[12]=McpPin(0x12,0x08);
    pinAndMcpIO[15]=McpPin(0x12,0x01);
    pinAndMcpIO[16]=McpPin(0x13,0x80);
    pinAndMcpIO[17]=McpPin(0x12,0x10);
    pinAndMcpIO[18]=McpPin(0x12,0x20);
    
    //setup MCP
    //Wire.begin();
    /*Wire.beginTransmission(0x20);
    Wire.write(0x00);//IODIRA
    Wire.write(0x00);//Set Outputs
    Wire.endTransmission();/*
    Wire.beginTransmission(expanderAddress);
    Wire.write(0x01);//IODIRB
    Wire.write(0x00);//Set Outputs
    Wire.endTransmission();*/
    
}

void display7segment::setValue(double value){
        int numbersAndPinSegment1[][7]={
          {1,2,3,15,16,18},//0
          {3,15},//1
          {1,2,15,16,17},//2
          {2,3,15,16,17},//3
          {3,15,17,18},//4
          {2,3,16,17,18},//5              
          {1,2,3,16,17,18},//6
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
          {5,6,7,8,11,12},//6
          {8,10,11},//7
          {5,6,7,8,10,11,12},//8
          {7,8,10,11,12} //9
        };
        McpPin dpPosition=pinAndMcpIO[9];
        //get first digit
        if(value<10){
            //show to 1d.p
            value=value*10;
            dpPosition=pinAndMcpIO[4];
        }
        
        int GpioAChange=0x00;
        int GpioBChange=0x00;
        int firstVal=(int)(value/10);
        for(int i=0;i<sizeof(numbersAndPinSegment1[firstVal])/sizeof(int);i++){
            McpPin iterationPin=pinAndMcpIO[numbersAndPinSegment1[firstVal][i]];
            //loop through pins checking whether it's for register A or B and add them to the int
            if(iterationPin.getPortAddress()==0x12){
                GpioAChange|=iterationPin.getPinAddress();
            }
            else{
                GpioBChange|=iterationPin.getPinAddress();
            }
        }
        //get the second digit and round
        int secondVal=(int)(value-firstVal*10+0.5);
        for(int i=0;i<sizeof(numbersAndPinSegment2[secondVal])/sizeof(int);i++){
            McpPin iterationPin=pinAndMcpIO[numbersAndPinSegment2[secondVal][i]];
            //loop through pins checking whether it's for register A or B and add them to the int
            if(iterationPin.getPortAddress()==0x12){
                GpioAChange|=iterationPin.getPinAddress();
            }
            else{
                GpioBChange|=iterationPin.getPinAddress();
            }
        }
        //loop through pins checking whether it's for register A or B and add them to the int
        if(dpPosition.getPortAddress()==0x12){
            GpioAChange|=dpPosition.getPinAddress();
        }
        else{
            GpioBChange|=dpPosition.getPinAddress();
        }
        Wire.beginTransmission(0x20);
        Wire.write(0x12);
        Wire.write(~GpioAChange);
        Wire.endTransmission();
        Wire.beginTransmission(0x20);
        Wire.write(0x13);
        Wire.write(~GpioBChange);
        Wire.endTransmission();
}
