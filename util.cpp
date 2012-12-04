//
//  util.cpp
//  RPi
//
//  Created by ed on 04/12/2012.
//  Copyright (c) 2012 ed. All rights reserved.
//

#include "util.h"


//
// This is a software loop to wait
// a short while.
//
void short_wait()
{ int w;
    for (w=0; w<100; w++)
    { w++;
        w--;
    }
} // short_wait

//
// Simple SW wait loop
//
void long_wait(int v)
{ int w;
    while (v--)
        for (w=-800000; w<800000; w++)
        { w++;
            w--;
        }
} // long_wait