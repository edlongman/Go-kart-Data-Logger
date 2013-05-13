//
//  file.cpp
//  RPi
//
//  Created by ed on 06/05/2013.
//  Copyright (c) 2013 ed. All rights reserved.
//

#include "file.h"
using namespace std;

//sets the loc name and ext of the file
file::file(string name){
	lastWrite=0;
    name=baseLoc + name + ext;
}

//appends whatever you pass in
bool file::append(string line){
    stream.open(name.c_str());
    stream << line << "\n";
    stream.close();
    return stream.fail();
}

//removes the file from the file system
bool file::destroy(){
    if (remove(name.c_str())==0) {
        return true;
    }
    return false;
}

//removes everything from the file
bool file::wipe(){
    bool gone = destroy();
    stream.open(name.c_str());
    if (gone && !stream.fail()) {
        stream.close();
        return true;
    }
    return false;
}