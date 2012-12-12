//
//  atod.h
//  RPi
//
//  Created by ed on 04/12/2012.
//  Copyright (c) 2012 ed. All rights reserved.
//

#ifndef __RPi__atod__
#define __RPi__atod__

//please make sure wiring pi is initialised!

class atod {
    int clock_pin,
    shutdown_pin,
    dout_pin,
    din_pin;
public:
    atod(int, int, int, int);
    atod();
    int read();
    int read(int);
    double voltage();
    double voltage(int);
    
private:
    void clock(int pin, int value);//this clocks the chip changing the bit specified beforehand
    int clock(int pin);//this clocks the chip reading the pin value beforehand
};

#endif /* defined(__RPi__atod__) */
