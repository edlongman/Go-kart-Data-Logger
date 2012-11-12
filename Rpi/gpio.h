//
//  gpio.h
//  Rpi
//
//  Created by ed on 05/11/2012.
//  Copyright (c) 2012 ed. All rights reserved.
//

#ifndef Rpi_gpio_h
#define Rpi_gpio_h

class pin_ref {
public:
    enum component_type{dip_switch,button,led,temp_sensor};
    string name;
    int pin_no;
    pin_ref(string name, int pin_no, component_type mananger);
private:
    enum status_code{ok,};
    status_code led_init();
    status_code temp_sensor_init();
    status_code button_init();
    status_code dip_switch_init();
};

#endif
