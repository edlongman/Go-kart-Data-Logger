#include <Wire.h>
#include <SD.h>
#include <math.h>
#include "hardware.h"
#include "file.h"
#include "sensor_test.h"

//
//  main.cpp
//  RPi
//
//  Created by ed on 04/05/2013.
//  Copyright (c) 2013 ed. All rights reserved.
//
  // This is the I2C address


temperature temp1(0);
light light1(0);
irTemperature irTemp1(0);
wheelspeed speed1(0);
display7segment speedDisplay(0x00);
long lastLogTime=millis();
unsigned long currTime;
unsigned long numberOfLogs;
void setup(){
    setupSerial();
    //start clock
    unsigned long currTime=micros();
    SD.begin(10);
    attachInterrupt(0,logWheelInterrupt,RISING);
    numberOfLogs=0;
}
void loop(){
    while(numberOfLogs*500<millis()){//if last log was >half a second ago, log
        //otherwise only update speed
        double currSpeed;
        bool speedResult=speed1.actualSpeed(&currSpeed);
        speedDisplay.setValue(currSpeed);
    }
    
    currTime=millis();
    
    double currTemp;
    bool tempResult=temp1.log(currTime,&currTemp);
    double currLight;
    bool lightResult=light1.log(currTime,&currLight);
    double currIrTemp;
    bool IrTempResult=irTemp1.log(currTime,&currIrTemp);
    double currDist;
    bool speedResult=speed1.log(currTime,&currDist);
    speedDisplay.setValue(currDist);
    lastLogTime=currTime;
}
void logWheelInterrupt(){
    speed1.logWheelRotation();
}
