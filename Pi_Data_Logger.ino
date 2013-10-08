#include <Wire.h>
#include <SD.h>
#include "sensor.h"
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
wheelspeed speed1(0);
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
}
void logWheelInterrupt(){
    speed1.logWheelRotation();
}
