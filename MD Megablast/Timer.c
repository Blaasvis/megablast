//
//  Timer.c
//  MD Megablast
//
//  Created by Arjan Timmerman on 24-04-16.
//  Copyright © 2016 Arjan Timmerman. All rights reserved.
//

#include "Timer.h"
#include "Defines.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/delay.h>


void setupTimer(void){
    // set up timer with prescaler = 64 and CTC mode
    TCCR1B |= (1 << WGM12)|(1 << CS12)|(1 << CS10);
    
    // initialize counter
    TCNT1 = 0;
    
    // initialize compare value
    OCR1A = 512;
    
    // enable compare interrupt
    TIMSK |= (1 << OCIE1A);
}


ISR (TIMER1_COMPA_vect){
    OCR1A = 512;
    sleep_disable();
   }

