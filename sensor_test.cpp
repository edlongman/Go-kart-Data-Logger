// sensor_test.cpp
// created on 17/10/13
// by Edward Longman

#include "file.h"
#include "hardware.h"
#include <Wire.h>
#include <math.h>
#include "sensor_test.h"

void setupTests(sensor sensors[]){
    if(!setupSerial()){
        //serial not connected so not for debug
        return;
    }
    testSensor(sensors);
    
}

bool setupSerial(){
    Serial.begin(9600);
    while(!Serial){}
    Serial.print("Connecting..");
    while(Serial.available()<=0&millis()>3000){//only for the first 3 seconds
        Serial.print(".");
    }
    if(Serial.available()>0){
        Serial.print("Connected");
        return true;
    }
    return false;
    
}

bool testSensor(sensor sensors[]){
    Serial.println("Press a key to read that sensor:");
    for(int i=0;i<sizeof(sensors)/sizeof(sensor);i++){
        Serial.println(((String) i+1)+". "+sensors[i].name());
    }
    int option=(int)Serial.read();
    double value;
    sensors[option].actual(&value);
    Serial.print("Value:")
    Serial.println(value));
    return true;
}


