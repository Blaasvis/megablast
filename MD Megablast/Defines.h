//
//  Defines.h
//  MD Megablast
//
//  Created by Arjan Timmerman on 24-04-16.
//  Copyright © 2016 Arjan Timmerman. All rights reserved.
//

#ifndef Defines_h
#define Defines_h

// DEFINES FOR PORTB
#define LEDRED 0
#define LEDGREEN 1
#define LEDBLUE 2
#define OVERCLOCK_DISABLE 4
#define LEDTYPE 5

// DEFINES FOR PORTC
#define CTRL_P1_UP 0
#define CTRL_P2_DOWN 1
#define CTRL_P3_LEFT 2
#define CTRL_P4_RIGHT 3
#define CTRL_P6_C_START 5
#define CTRL_P9_A_B 4

// DEFINES FOR PORTD
#define LANG 0
#define RESET_OUT 1
#define RESETIN 2
#define CTRL_P7_SELECT 3
#define MODE 4
#define OVERCLKMODE 6
#define Z80HALT 7

#define LOWBUTTONCMD 0x30
#define HIBUTTONCMD 0x10


#define setMode() PORTD |= 1 << MODE;
#define unsetMode() PORTD &= ~(1 << MODE);
#define toggleMode() PORTD ^= (1 << MODE);

#define setLang() PORTD |= 1 << LANG;
#define unsetLang() PORTD &= ~(1 << LANG);
#define toggleLang() PORTD ^= (1 << LANG);

#define setReset() PORTD |= 1 << RESET_OUT;
#define unsetReset() PORTD &= ~(1 << RESET_OUT);

#define setOverclock() PORTD |= 1 << OVERCLKMODE;
#define unsetOverclock() PORTD &= ~(1 << OVERCLKMODE);

#define setZ80Halt() PORTD |= 1 << Z80HALT;
#define unsetZ80Halt() PORTD &= ~(1 << Z80HALT);
#endif /* Defines_h */
