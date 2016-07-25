//
//  Controller.c
//  MD Megablast
//
//  Created by Arjan Timmerman on 24-04-16.
//  Copyright © 2016 Arjan Timmerman. All rights reserved.
//

#include "Controller.h"
#include "Defines.h"
#include "Timer.h"
#include "LEDControl.h"
#include "Eeprom.h"
#include "MegaBlast.h"
#include "ResetButton.h"

#include <avr/interrupt.h>
#include <util/delay.h>

void handel_button_presses(uint8_t lo_buttons[],uint8_t hi_buttons[]){
    cli();
    // memory is too slow so we wait a bit
    _delay_ms(1);
    uint8_t combinedlo_buttons = 0xff;
    uint8_t combinedhi_buttons = 0xff;
    // merge the data
    combinedlo_buttons &= lo_buttons[0];
    combinedhi_buttons &= hi_buttons[0];
    combinedlo_buttons &= lo_buttons[1];
    combinedhi_buttons &= hi_buttons[1];
    combinedlo_buttons &= lo_buttons[2];
    combinedhi_buttons &= hi_buttons[2];
    //up + a + c + start we are toggleing the overclock mode.
    if( combinedhi_buttons == 0xE1 && combinedlo_buttons == 0xFD)
    {
        if(PINB & (1 << OVERCLOCK_DISABLE)){
            // halt the z80
            unsetZ80Halt();
            _delay_ms(100);
            PORTD ^= (1 << OVERCLKMODE);
            _delay_ms(100);
            // unhalt the z80
            setZ80Halt();
            handleWhenCombinationBeenPressed();
        }
        
    }
    // down + a + c + start we are switching the region mode.
    if( combinedhi_buttons == 0xE2 && combinedlo_buttons == 0xFE)
    {
        // reset!
        resetConsole();
        sei();
        setupTimer();
    }

    // right + a + c + start we are switching the region mode.
    if( combinedhi_buttons == 0xE8 && combinedlo_buttons == 0xFC){
        unsetZ80Halt();// halt the z80
        _delay_ms(100);
        if((PIND & (1<< MODE)) && (PIND & (1<< LANG))){// ntsc to ntsc-j

            unsetLang();
        }else if(~PIND & (1<< MODE) && PIND & (1<< LANG)){ // pal to ntsc
            setMode();
        }else{ // ntsc to pal
            toggleMode();
            toggleLang();
        }
        _delay_ms(100);
        setZ80Halt();// unhalt the z80
        handleWhenCombinationBeenPressed();
        
    }
    //    // left + a + c + start we are switching the region mode.
    if( combinedhi_buttons == 0xE4 && combinedlo_buttons == 0xFC){
        unsetZ80Halt();// halt the z80
        _delay_ms(100);
        // ntsc to pal
        if(PIND & (1<< MODE) && PIND & (1<< LANG)){ // ntsc-j to ntsc
            unsetMode();
        }else if(PIND & (1<< MODE) && ~PIND & (1<< LANG)){ // ntsc to pal
            setLang();
        }else{ //pal to ntsc-j
            toggleLang();
            toggleMode();
        }
        _delay_ms(100);
        setZ80Halt();// unhalt the z80
        handleWhenCombinationBeenPressed();
    }
    
}
/**
 when done, reset timer, write state to eeprom, interrupts back on.
 No button presses read till timer is done 
 **/
void handleWhenCombinationBeenPressed(void){
    sei();
    setLEDs();
    setupTimer();
    OCR1A   = 10000; // set the CTC compare value
    TCNT1 = 0;

    writeStateToEeprom();
}