// sensor_test.cpp
// created on 17/10/13
// by Edward Longman

#include "file.h"
#include "hardware.h"
#include <Wire.h>
#include <math.h>

void setupSerial(){
    Serial.begin(9600);
    while(!Serial){}
    Serial.print("Connecting..");
    while(Serial.available()<=0&millis()>3000){
        Serial.print(".");
    }
    if(Serial.available()>0){
        Serial.print("Connected");
    }
}
