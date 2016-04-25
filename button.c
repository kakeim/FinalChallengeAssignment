/*
 * button.c
 *
 *  Created on: Apr 13, 2016
 *      Author: fapal - this guy
 */
#include "functions.h"

extern Graphics_Context g_sContext;

void button_task(void) {
	Semaphore_pend(Button_Semaphore, BIOS_WAIT_FOREVER);

	Timer32_startTimer((uint32_t)TIMER32_0_BASE,0);
}

void buttonInit(void){
//	GPIO_setCallback(Board_BUTTON0, SW1_IRQHandler);
	GPIO_enableInt(Board_BUTTON0);

	/* Configure RGB LED pins */
	GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN4 | GPIO_PIN6);
	GPIO_setAsOutputPin(GPIO_PORT_P5, GPIO_PIN6);

	/* Set RGB LED to red */
	GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN6);
//	P2OUT |= BIT6;
//	P2OUT &= ~BIT4;
//	P5OUT &= ~BIT6;

	/* Setup Button */
	GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P5,GPIO_PIN1);
	GPIO_clearInterruptFlag(GPIO_PORT_P5, GPIO_PIN1);
	GPIO_enableInterrupt(GPIO_PORT_P5, GPIO_PIN1);
	GPIO_interruptEdgeSelect(GPIO_PORT_P5, GPIO_PIN1, GPIO_HIGH_TO_LOW_TRANSITION);
	Interrupt_enableInterrupt(INT_PORT5);
}

void SW1_IRQHandler(void){
	uint32_t status;
	status = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P5);
	MAP_GPIO_clearInterruptFlag(GPIO_PORT_P5, status);

	// change status of LED from red to green or vice versa
	MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN6);	// toggle Red
	MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN4);	// toggle Green

	Semaphore_post(Button_Semaphore);
}
