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
file::file(string plainname){
	lastWrite=0;
    name=baseLoc + plainname + ext;
}

//appends whatever you pass in
bool file::append(string line){
    stream.open(name.c_str(), fstream::app);
    if(stream.is_open()){
		stream << line << endl;
		stream.close();
		return stream.fail();
    }
    else {cout << "couldn't write";}
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
