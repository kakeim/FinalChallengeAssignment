/*
 * button.c
 *
 *  Created on: Apr 13, 2016
 *      Author: fapal - this guy
 */
#include "functions.h"

extern Graphics_Context g_sContext;
extern Timer_A_PWMConfig buzzerPWM;

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
void button2_task(void) {
	int button2_press = 0;
	int numpress = 0;
	int step_goal = 10;
	char string[9];


	while (1) {
		/* Wait for an button press to update value */
		Mailbox_pend(button2_box, &button2_press, BIOS_WAIT_FOREVER);
		numpress++;
		//if(button2_press == 1){
		//	numpress++;
		//	button2_press=0;
		//}

		switch (numpress) {
		case 1:
			step_goal = 10;
			Mailbox_post(goal_box, &step_goal, BIOS_NO_WAIT);
			//				num_presses++;

			sprintf(string, "Goal: %5d", step_goal);
			Graphics_drawStringCentered(&g_sContext, (int8_t *)string, AUTO_STRING_LENGTH, 64, 70, OPAQUE_TEXT);


			break;
		case 2:
			step_goal = 5000;
			Mailbox_post(goal_box, &step_goal, BIOS_NO_WAIT);
			//				num_presses++;

			sprintf(string, "Goal: %5d", step_goal);
			Graphics_drawStringCentered(&g_sContext, (int8_t *)string, AUTO_STRING_LENGTH, 64, 70, OPAQUE_TEXT);
			break;
		case 3:
			step_goal = 7500;
			Mailbox_post(goal_box, &step_goal, BIOS_NO_WAIT);
			//				num_presses++;

			sprintf(string, "Goal: %5d", step_goal);
			Graphics_drawStringCentered(&g_sContext, (int8_t *)string, AUTO_STRING_LENGTH, 64, 70, OPAQUE_TEXT);
			break;
		case 4:
			step_goal = 10000;
			Mailbox_post(goal_box, &step_goal, BIOS_NO_WAIT);
			//button2_press++;

			sprintf(string, "Goal: %5d", step_goal);
			Graphics_drawStringCentered(&g_sContext, (int8_t *)string, AUTO_STRING_LENGTH, 64, 70, OPAQUE_TEXT);
			break;
		default:
			step_goal = 10;
			numpress = 1;
			Mailbox_post(goal_box, &step_goal, BIOS_NO_WAIT);

			sprintf(string, "Goal: %5d", step_goal);
			Graphics_drawStringCentered(&g_sContext, (int8_t *)string, AUTO_STRING_LENGTH, 64, 70, OPAQUE_TEXT);
			break;
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

	int button2_press = 1;
	uint32_t status;
	status = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P3);
	_delay_cycles(5000000);
	MAP_GPIO_clearInterruptFlag(GPIO_PORT_P3, status);

	Mailbox_post(button2_box, &button2_press, BIOS_NO_WAIT);
}
