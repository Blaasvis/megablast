//
//  ResetButton.c
//  MD Megablast
//
//  Created by Arjan Timmerman on 24-04-16.
//  Copyright © 2016 Arjan Timmerman. All rights reserved.
//

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include "ResetButton.h"
#include "Defines.h"
#include "Eeprom.h"
#include "MegaBlast.h"
#include "LEDControl.h"
#include "Timer.h"

int checkResetButton(void){
    _delay_ms(20);

    return ((~PIND & (1 << PD2) && MCUCR & (1 << ISC01)) || (PIND & (1 << PD2) && MCUCR & ((1 << ISC01) & (1 << ISC00))));
}



void resetbutton(void){
    while(checkResetButton()){
        cycleLEDs();
        _delay_ms(1000);
    }
    setModeOffOfLedModes();
    writeStateToEeprom();
    sei();
    return;
}