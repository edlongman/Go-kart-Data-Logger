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
#include <fstream>
#define baseLoc "/output/"
#define ext ".txt"


using namespace std;
class file{
    string name;
    time_t lastWrite;
    ofstream stream;
public:
    file(string name);
    bool append(string line);
    bool wipe();
    bool destroy();
};

#endif /* defined(__RPi__file__) */
