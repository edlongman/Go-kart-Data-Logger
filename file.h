
//
//  file.h
//  RPi
//
//  Created by ed on 06/05/2013.
//  Copyright (c) 2013 ed. All rights reserved.
//

#ifndef __RPi__file__
#define __RPi__file__

#include <SD.h>
#include "Arduino.h"
#include <WString.h>
#define baseLoc "log/"
#define ext ".txt"


using namespace std;
class save{
    String name;
    int lastWrite;
    bool sdcard;
    File openfile;
public:
    save(String name);
    bool append(String line);
    bool wipe();
    bool destroy();
};

#endif /* defined(__RPi__file__) */
