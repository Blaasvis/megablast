//
//  Timer.h
//  MD Megablast
//
//  Created by Arjan Timmerman on 24-04-16.
//  Copyright © 2016 Arjan Timmerman. All rights reserved.
//

#ifndef Timer_h
#define Timer_h
#define disableTimer()  TIMSK &= ~(1 << OCIE1A);
#define startTimer() TIMSK |= (1 << OCIE1A);
#include <stdio.h>
void setupTimer(void);
void setupTimer1(void);

#endif /* Timer_h */
