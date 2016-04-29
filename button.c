/*
 * button.c
 *
 *  Created on: Apr 13, 2016
 *      Author: fapal - this guy
 */
#include "functions.h"

extern Graphics_Context g_sContext;

/* Timer_A PWM Configuration Parameter */
Timer_A_PWMConfig buzzerPWM =
{
        TIMER_A_CLOCKSOURCE_SMCLK,
        TIMER_A_CLOCKSOURCE_DIVIDER_8,
		20000,
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

void button_task(void) {
	int button_press = 0;

	while (1) {
		/* Wait for an button press to update value */
		Mailbox_pend(button1_box, button_press, BIOS_WAIT_FOREVER);

		if (button_press == 0)
		{
			// change status of LED from red to green or vice versa
			MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN6);	// toggle Red
			MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN4);	// toggle Green

			Timer32_startTimer((uint32_t)TIMER32_0_BASE,0);

			buzzerPWM.dutyCycle = 6000000/300/2;
			Timer_A_generatePWM(TIMER_A1_BASE, &buzzerPWM);
			Task_sleep(2000);
			buzzerPWM.dutyCycle = 0;
			Timer_A_generatePWM(TIMER_A1_BASE, &buzzerPWM);

			button_press = 1;
		}
		else
		{
			// change status of LED from red to green or vice versa
			MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN6);	// toggle Red
			MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN4);	// toggle Green

			Timer32_haltTimer((uint32_t)TIMER32_0_BASE);

			button_press = 0;
		}
	}
}

void buttonInit(void){
	//	GPIO_setCallback(Board_BUTTON0, SW1_IRQHandler);
	GPIO_enableInt(Board_BUTTON0);
	GPIO_enableInt(Board_BUTTON1);

	/* Configure RGB LED pins */
	GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN4 | GPIO_PIN6);
	GPIO_setAsOutputPin(GPIO_PORT_P5, GPIO_PIN6);

	/* Set RGB LED to red */
	GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN6);
	GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN4);
	GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN6);

	// Configuring P2.7 as Timer A PWM output for buzzer9
	PMAP_configurePorts((const uint8_t *) port_mapping, P2MAP, 1, PMAP_DISABLE_RECONFIGURATION);
	GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN6 | GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION);
	Timer_A_generatePWM(TIMER_A1_BASE, &buzzerPWM);

	/* Setup Button 1*/
	GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P5,GPIO_PIN1);
	GPIO_clearInterruptFlag(GPIO_PORT_P5, GPIO_PIN1);
	GPIO_enableInterrupt(GPIO_PORT_P5, GPIO_PIN1);
	GPIO_interruptEdgeSelect(GPIO_PORT_P5, GPIO_PIN1, GPIO_HIGH_TO_LOW_TRANSITION);
	Interrupt_enableInterrupt(INT_PORT5);

	/* Setup Button 2*/
	GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P3, GPIO_PIN5);
	GPIO_clearInterruptFlag(GPIO_PORT_P3, GPIO_PIN5);
	GPIO_enableInterrupt(GPIO_PORT_P3, GPIO_PIN5);
	GPIO_interruptEdgeSelect(GPIO_PORT_P3, GPIO_PIN5, GPIO_HIGH_TO_LOW_TRANSITION);
	Interrupt_enableInterrupt(INT_PORT3);
}


void SW1_IRQHandler(void){
	int button_press = 0;

	uint32_t status;
	status = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P5);
	_delay_cycles(5000000);
	MAP_GPIO_clearInterruptFlag(GPIO_PORT_P5, status);

	if (button_press == 1) {
		button_press = 0;
	} else {
		button_press = 0;
	}

	Mailbox_post(button1_box, button_press, BIOS_NO_WAIT);
}

void SW2_IRQHandler(void) {
	uint32_t status;
	status = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P3);
	_delay_cycles(5000000);
	MAP_GPIO_clearInterruptFlag(GPIO_PORT_P3, status);

	char string[9];
	sprintf(string, "test:");
	Graphics_drawStringCentered(&g_sContext, (int8_t *)string, 8, 64, 70, OPAQUE_TEXT);
}
