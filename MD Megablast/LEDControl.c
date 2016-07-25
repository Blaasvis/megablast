//
//  LEDControl.c
//  MD Megablast
//
//  Created by Arjan Timmerman on 24-04-16.
//  Copyright © 2016 Arjan Timmerman. All rights reserved.
//

#include "LEDControl.h"
#include "Defines.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
int ledmodes = 0;

void cycleLEDs(void){
    turnOffLEDs();
    // we have max 6 modes so back to the first one.
    if(ledmodes > 5 ){
        ledmodes = 0;
    }
    switch (ledmodes) {
            // pal
        case 0:
            PORTB ^= 1 << LEDGREEN;
            break;
            // pal + overclock
        case 1:
            PORTB ^= (1 << LEDGREEN) | (1 << LEDBLUE);
            break;
            // ntsc
        case 2:
            PORTB ^= (1 << LEDRED);
            break;
            // ntsc + overclock
        case 3:
            PORTB ^= (1 << LEDRED) | (1 << LEDGREEN);
            break;
            // ntsc-j
        case 4:
            PORTB ^= 1 << LEDBLUE;
            break;
            // ntsc-j + overclock
        case 5:
            PORTB ^= (1 << LEDBLUE) | (1 << LEDRED);
            break;
        default:
            break;
    }
    // increase by one so the next mode has been setup.
    ledmodes++;
}
void turnOffLEDs(void){
    // turn off leds. You won't notice it.
    if(PINB & (1 << LEDTYPE)){ // if high then common cathode
        PORTB &= ~(1 << LEDGREEN) & ~(1 << LEDBLUE) & ~(1 << LEDRED);
    }else{
        PORTB |= (1 << LEDGREEN) | (1 << LEDBLUE) | (1 << LEDRED);
    }
}

void setLEDs(void){
    turnOffLEDs();
    // pal
    if(~PIND & (1<< MODE) && PIND & (1<< LANG)){
        PORTB ^= (1 << LEDGREEN);
        if(PIND & 1<< OVERCLKMODE){
            PORTB ^= (1 << LEDBLUE);
        }
    }
    // ntsc
    if(PIND & (1<< MODE) && PIND & (1<< LANG)){
        PORTB ^= (1 << LEDRED);
        if(PIND & 1<< OVERCLKMODE){
            PORTB ^= (1 << LEDGREEN);
        }
    }
    // jap
    if(PIND & (1<< MODE) && ~PIND & (1<< LANG)){
        PORTB ^= (1 << LEDBLUE);
        if(PIND & 1<< OVERCLKMODE){
            PORTB ^= (1 << LEDRED);
        }
    }
    
}
/*
 * Read the state of the leds and set ledmodes this is used on start up and controller functions;
 */
void setModeOffOfLedModes(void){
    // at the last one back to 0 else -1 one since the count is being upped before that.
    if(ledmodes > 6 ){
        ledmodes = 0;
    }else{
        ledmodes--;
    }
    unsetZ80Halt();
    _delay_ms(100);
    // ntsc to jap
    // low 50hz / high 60hz
    if(ledmodes <2){
        unsetMode();
    }else{
        setMode();
    }
    // lang high == eng , low = jap
    // ntsc to jap
    if(ledmodes <4){
        setLang();
    }else{
        unsetLang();
    }
    _delay_ms(100);
    setZ80Halt();
    _delay_ms(300);
    unsetZ80Halt();
    _delay_ms(100);
    // overclock yes or no ?
    if(PINB & (1 << OVERCLOCK_DISABLE)){
        if(ledmodes % 2){
            setOverclock();
        }
        else{
            unsetOverclock();
        }
    }
    _delay_ms(100);
    setZ80Halt();
    setLEDs();
    // back to the normal value
    ledmodes++;
}