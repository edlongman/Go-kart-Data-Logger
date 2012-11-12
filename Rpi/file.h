//
//  file.h
//  Rpi
//
//  Created by ed on 05/11/2012.
//  Copyright (c) 2012 ed. All rights reserved.
//
#ifndef Rpi_file_h
#define Rpi_file_h

#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>

using namespace std;

int write_out();
void read_to_vector( vector<string> & file_lines );

#endif
