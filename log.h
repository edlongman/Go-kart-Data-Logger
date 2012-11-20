//
//  log.h
//  Rpi
//
//  Created by ed on 05/11/2012.
//  Copyright (c) 2012 ed. All rights reserved.
//


//this file is where colection from all the different sensors is accumulated into one format
#ifndef Rpi_log_h
#define Rpi_log_h

#include "util.h"
#include "gpio.h"

using namespace std;

class snapshot {
    
public:
    time_t timestamp;
    vector<double> temperatures;
    
    snapshot();
};

void loop_recording(){
    pin_ref pin=pin_ref();
    if(pi_gpio_get_reading(pin)){
        
    }
}

#endif
