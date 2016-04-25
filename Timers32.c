/*
 * timers32.c
 *
 *  Created on: Apr 8, 2016
 *      Author: Kevin A. Keim
 */

#include "functions.h"

void Timer32_ISRHandler(void)
{
	Timer32_clearInterruptFlag((uint32_t)TIMER32_0_BASE);
	ADC14_toggleConversionTrigger();
}

void initTimers32(void) {
	/* Enable 5ms timer */
	Timer32_initModule((uint32_t)TIMER32_0_BASE,TIMER32_PRESCALER_1,TIMER32_32BIT,TIMER32_PERIODIC_MODE);
	Timer32_setCount((uint32_t)TIMER32_0_BASE,240000);
	Timer32_enableInterrupt((uint32_t)TIMER32_0_BASE);
	Interrupt_enableInterrupt(INT_T32_INT1);
}
