
//
//  file.h
//  RPi
//
//  Created by ed on 06/05/2013.
//  Copyright (c) 2013 ed. All rights reserved.
//

#ifndef __RPi__file__
#define __RPi__file__

#include <iostream>
#include <SD.h>
#include <fstream>
#include <WString.h>
#define baseLoc "log/"
#define ext ".txt"


using namespace std;
class file{
    String name;
    int lastWrite;
    bool sdcard;
public:
    file(String name);
    bool append(String line);
    bool wipe();
    bool destroy();
};

#endif /* defined(__RPi__file__) */
