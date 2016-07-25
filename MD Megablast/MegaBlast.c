//
//  MegaBlast.c
//  MD Megablast
//
//  Created by Arjan Timmerman on 24-04-16.
//  Copyright © 2016 Arjan Timmerman. All rights reserved.
//
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include <avr/wdt.h>

#include "MegaBlast.h"
#include "LEDControl.h"
#include "Defines.h"
#include "Eeprom.h"
#include "Timer.h"
#include "ResetButton.h"
#include "Controller.h"
int intertype=0;

uint8_t lo_buttons[4] = {0,0,0,0};
uint8_t hi_buttons[4] = {0,0,0,0};
uint8_t hibutton_counter=3;
uint8_t lobutton_counter=3;
uint8_t checkreset_bool= 0;

/*
 Setup all ports
 */
void initIO(void) {
    
    // port b init. settings and overclock stuff :)
    DDRB = 0xFF; // all to output and setting the rest manual.
    DDRB &= ~(1 << OVERCLOCK_DISABLE);    // INT0: input...
    PORTB |= (1 << OVERCLOCK_DISABLE); // standard on 1, can be disable when connected to ground with pullup.
    DDRB &= ~(1 << LEDTYPE);    // INT0: input...
    PORTB |= (1 << LEDTYPE);    // standard common cathode so with pullup.
    
    // port D init.
    DDRD = 0xFF; // all to output and setting the rest manual.
    DDRD &= ~(1 << RESETIN);    // INT0: input...
    DDRD &= ~(1 << CTRL_P7_SELECT);     // Clear the PD7 pin for input
    PORTD |= (1 << RESETIN);    // standard pullup.
    PORTD |= (1 << Z80HALT);    // standard  with pullup.
    PORTD |= (1 << RESET_OUT);    // standard with pullup.
    // Interrupts resetin/select line.
    GICR |= 1 << INT0;
    // interupts on RESETIN
    // detect if it is low by default or high
    if(PIND & (1 << RESETIN)){
        // falling edge
        MCUCR |= (1 << ISC01);
    }else{
        // raising edge
        MCUCR |= (1 << ISC01) &  (1 << ISC00);
    }
}
/*
 Start up and setup sleepmode
 */
void startUp(void){
    initIO();
    readStateFromEeprom(); // get the last known state from the eeprom
    setupTimer();
    set_sleep_mode(SLEEP_MODE_IDLE);
    int8_t data = 0;
    int8_t data2 = 0;
    while (1) {
        sei();
        if(checkreset_bool == 1 ){
            cli();
            _delay_ms(300);
            checkreset_bool = 0;
            // short press ? then reset
            if(checkResetButton() == 0){
                    resetConsole();
                    sei();
            }else{
                resetbutton();
            }
        }
        // wait till low.
        while ((PIND & (1 << CTRL_P7_SELECT)) && checkreset_bool & 1 << 1 ) {}
        if(~PIND & (1 << CTRL_P7_SELECT) && lobutton_counter < 4 ){
            // read the data;
            data = ~PINC;
            lo_buttons[lobutton_counter] = data;
            lobutton_counter = lobutton_counter + 1;
        }
        // wait till high.
        while (~PIND & (1 << CTRL_P7_SELECT)&& checkreset_bool & 1 << 1 ) {}
        if(PIND & (1 << CTRL_P7_SELECT) &&  hibutton_counter < 4){
            // read the data;
            data2 = ~PINC;
            hi_buttons[hibutton_counter] = data2;
            hibutton_counter = hibutton_counter + 1;
        }
        // 4 samples is enough!
        if(lobutton_counter == 4 ){
            // handel the controller
            handel_button_presses(lo_buttons,hi_buttons);
            // reset count
            lobutton_counter= 0;
            hibutton_counter= 0;
            // interrupt back on before sleep
            sei();
            sleep_enable();
            sleep_mode();
            checkreset_bool = 0;

        }
        // enable interrupts
        sei();
    }
}

/*
 The reset of the console.
 */
void resetConsole(void){
    PORTD ^= 1<< RESET_OUT;
    _delay_ms(200);
    PORTD ^= 1<< RESET_OUT;
}

// interrupt handler for the reset button
ISR(INT0_vect){
    // bounce back check.
    if(checkResetButton()){
        checkreset_bool= 1;
    }
}
