//
//  Eeprom.c
//  MD Megablast
//
//  Created by Arjan Timmerman on 24-04-16.
//  Copyright © 2016 Arjan Timmerman. All rights reserved.
//
#include <avr/eeprom.h>
#include <avr/io.h>
#include <util/delay.h>
#include "Eeprom.h"
#include "Defines.h"
#include "LEDControl.h"
/**
 * pos 0 = lang
 * pos 1 = mode
 * pos 2 = overclock
 *
 **/
void writeStateToEeprom(void){
    eeprom_busy_wait();
    uint8_t ByteOfData = 0x00;
    // write the data to a byte just 3 positions needed
    // english
    if(PIND & (1<<LANG)){
        ByteOfData |= 1<<0;
    }
    // 60hz
    if(PIND & (1<<MODE)){
        ByteOfData |= 1<<1;
    }
    // overclock enabled
    if(PIND & (1<<OVERCLKMODE)){
        ByteOfData |= 1<<2;
    }
    eeprom_update_byte((uint8_t*)26,ByteOfData);
    eeprom_busy_wait();
}
/**
 * pos 0 = lang
 * pos 1 = mode
 * pos 2 = overclock
 *
 **/
void readStateFromEeprom(void){
    eeprom_busy_wait();
    uint8_t ByteOfData = 0x00;
    ByteOfData = eeprom_read_byte((uint8_t*)26);
    // debug stuff
    // default ground for mode so 50hz
    unsetMode();
    // is bit set go 60hz ledmodes from 2 up it is 60hz
    if(ByteOfData & 1<<1){
        setMode();
    }
    // high since we want english by default
    setLang()
    // is bit set go japanese ledmodes from 4 up it is japanese
    if(~ByteOfData & (1<<0)){
        unsetLang();
    }
    // is bit set go overclocked
    if(ByteOfData & 1<<2){
        // check if it is allready set or not, if so we want to halt the z80 for a bit
            unsetZ80Halt();
            _delay_ms(50);
            setOverclock();
            _delay_ms(300);
            setZ80Halt();
    }
    // set the led to the right color
    setLEDs();
}
