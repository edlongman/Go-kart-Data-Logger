
//
//  file.cpp
//  RPi
//
//  Created by ed on 06/05/2013.
//  Copyright (c) 2013 ed. All rights reserved.
//

#include "file.h"
#include "Arduino.h"
#include <SD.h>

//sets the loc name and ext of the file
save::save(String plainname){
    lastWrite=0;
    name=baseLoc + plainname + ext;
}

//appends whatever you pass in
bool save::append(String line){
    if(sdcard){
        openfile=SD.open(name.c_str());
        openfile.println(line);
        openfile.close();
    }
}

//removes the file from the file system
bool save::destroy(){
    // to implementreturn SD.remove(name.c_str())
}

//removes everything from the file
bool save::wipe(){
    //bool gone = destroy();
    //openfile.open(name.c_str());
    return false;
}    
