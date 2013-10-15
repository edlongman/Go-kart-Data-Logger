#include <Wire.h>
#include <SD.h>
#include <math.h>
#include "hardware.h"
#include "file.h"

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
unsigned long currTime;
void setup(){
    //start clock
    unsigned long currTime=micros();
    SD.begin(10);
    attachInterrupt(0,logWheelInterrupt,RISING);
}
void loop(){
    double currTemp;
    bool tempResult=temp1.log(currTime,&currTemp);
    double currLight;
    bool lightResult=light1.log(currTime,&currLight);
    double currIrTemp;
    bool IrTempResult=irTemp1.log(currTime,&currIrTemp);
    double currSpeed;
    bool speedResult=irTemp1.log(currTime,&currSpeed);
    speedDisplay.setValue(currSpeed);
}
void logWheelInterrupt(){
    speed1.logWheelRotation();
}
