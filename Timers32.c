/*
 * timers32.c
 *
 *  Created on: Apr 8, 2016
 *      Author: Kevin A. Keim
 */

#include "functions.h"

Timer_A_PWMConfig backlightPWM =
{
		TIMER_A_CLOCKSOURCE_SMCLK,
		TIMER_A_CLOCKSOURCE_DIVIDER_8,
		6000000/300,
		TIMER_A_CAPTURECOMPARE_REGISTER_1,
		TIMER_A_OUTPUTMODE_RESET_SET,
		0
};

Timer_A_PWMConfig buzzerPWM =
{
		TIMER_A_CLOCKSOURCE_SMCLK,
		TIMER_A_CLOCKSOURCE_DIVIDER_8,
		6000000/300,
		TIMER_A_CAPTURECOMPARE_REGISTER_2,
		TIMER_A_OUTPUTMODE_RESET_SET,
		0
};

/* Port mapper configuration register */
const uint8_t port_mapping[] =
{
		//Port P2:
		PM_TA0CCR0A, PM_TA0CCR0A, PM_TA0CCR0A, PM_NONE, PM_NONE, PM_NONE, PM_TA1CCR1A, PM_TA1CCR2A
};

void initPWM(void)
{
	/* Configure Buzzer and LCD pwm pins */
	PMAP_configurePorts((const uint8_t *) port_mapping, P2MAP, 1, PMAP_DISABLE_RECONFIGURATION);
	GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN6 | GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION);
	Timer_A_generatePWM(TIMER_A1_BASE, &backlightPWM);
	Timer_A_generatePWM(TIMER_A1_BASE, &buzzerPWM);
}

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
