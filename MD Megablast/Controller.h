//
//  Controller.h
//  MD Megablast
//
//  Created by Arjan Timmerman on 24-04-16.
//  Copyright © 2016 Arjan Timmerman. All rights reserved.
//

#ifndef Controller_h
#define Controller_h
#include <stdio.h>
void checkController_low(void);
void checkController_high(void);
void handel_button_presses(uint8_t lo_buttons[],uint8_t hi_buttons[]);
void handleWhenCombinationBeenPressed(void);
#endif /* Controller_h */
