/*
 * 	adc14.c
 *
 *  Created on: Apr 8, 2016
 *      Author: Kevin A. Keim
 */

#include "functions.h"

extern Graphics_Context g_sContext;
uint32_t joystickResult, dutyCycle;

void initADC(void)
{

	volatile uint32_t i;

	// Configures Pin 4.0, 4.2, and 6.1 as ADC input for Accelerometer
	MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4, GPIO_PIN0 | GPIO_PIN2, GPIO_TERTIARY_MODULE_FUNCTION);
	MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6, GPIO_PIN1, GPIO_TERTIARY_MODULE_FUNCTION);
	// Configure Pin 6.0 as ADC input for joystick
	MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6, GPIO_PIN0, GPIO_TERTIARY_MODULE_FUNCTION);

	ADC14_enableModule();
	MAP_ADC14_initModule(ADC_CLOCKSOURCE_ADCOSC, ADC_PREDIVIDER_64, ADC_DIVIDER_8, 0);

	// Configuring ADC Memory
	MAP_ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM2, false);
	MAP_ADC14_configureConversionMemory(ADC_MEM0, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A14, false);
	MAP_ADC14_configureConversionMemory(ADC_MEM1, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A13, false);
	MAP_ADC14_configureConversionMemory(ADC_MEM2, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A11, false);

	// Enable the interrupts when the conversion is finished
	MAP_ADC14_enableInterrupt(ADC_INT2);

	// Enable interrupts
	MAP_Interrupt_enableInterrupt(INT_ADC14);
	MAP_Interrupt_enableMaster();

	// Set up automatic sequence to convert
	MAP_ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION);

	// Enabling/Toggling Conversion
	MAP_ADC14_enableConversion();
	MAP_ADC14_toggleConversionTrigger();
}

void display_accel(void)
{
	uint16_t acc_vals[3];
	char string[9];
	uint16_t last_val_x = 0;
	uint16_t last_val_y = 0;
	uint16_t last_val_z = 0;
	uint32_t average_x = 0;
	uint32_t average_y = 0;
	uint32_t average_z = 0;
	uint32_t prev_average_x = 0;
	uint32_t prev_average_y = 0;
	uint32_t prev_average_z = 0;
	uint16_t cycle = 0;

	uint32_t steps = 0;

	/* Display Accelerometer Section Title */
	Graphics_drawStringCentered(&g_sContext, "Accelerometer:", AUTO_STRING_LENGTH, 64, 20, OPAQUE_TEXT);

	while(1)
	{
		/* Wait for an ADC interrupt to pass in new accelerometer values */
		Mailbox_pend(adc_result, acc_vals, BIOS_WAIT_FOREVER);

		/* Accumulate the differences between values and increment count */
		average_x += abs(acc_vals[0] - last_val_x);
		average_y += abs(acc_vals[1] - last_val_y);
		average_z += abs(acc_vals[2] - last_val_z);
		last_val_x = acc_vals[0];
		last_val_y = acc_vals[1];
		last_val_z = acc_vals[2];
		cycle++;

		/* After 200 cycles (~1ms), take the average values and display them */
		if(cycle >= 200)
		{
			average_x /= 200;
			average_y /= 200;
			average_z /= 200;

			if (average_x > prev_average_x + 12 && average_y > prev_average_y + 12 && average_z > prev_average_z + 12) {
				steps++;
			}

			/* Print X average */
			sprintf(string, "X: %5d", average_x);
			Graphics_drawStringCentered(&g_sContext, (int8_t *)string, 8, 64, 30, OPAQUE_TEXT);

			/* Print Y average */
			sprintf(string, "Y: %5d", average_y);
			Graphics_drawStringCentered(&g_sContext, (int8_t *)string, 8, 64, 40, OPAQUE_TEXT);

			/* Print Z average */
			sprintf(string, "Z: %5d", average_z);
			Graphics_drawStringCentered(&g_sContext, (int8_t *)string, 8, 64, 50, OPAQUE_TEXT);

			sprintf(string, "Steps: %5d", steps);
			Graphics_drawStringCentered(&g_sContext, (int8_t *)string, 8, 64, 60, OPAQUE_TEXT);

			/* Reset the averages for the next run */
			prev_average_x = average_x;
			prev_average_y = average_y;
			prev_average_z = average_z;

			average_x = 0;
			average_y = 0;
			average_z = 0;
			cycle = 0;
		}
	}
}
void josytick(void){
	ADC14->CTL0 = ADC14_CTL0_SHT0_5 | ADC14_CTL0_SHP |  ADC14_CTL0_SSEL_1 | ADC14_CTL0_ON ;
	ADC14->CTL1 = ADC14_CTL1_RES_3;

	ADC14->MCTL[0] |= ADC14_MCTLN_INCH_15;    // A15 ADC input select; Vref=AVCC
	ADC14->IER0 |= ADC14_IER0_IE0;          // Enable ADC conv complete interrupt

	// Enable ADC interrupt in NVIC module
	NVIC->ISER[0] = 1 << ((ADC14_IRQn) & 31);

	__enable_interrupt();                   // Enable NVIC global/master interrupt
	SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk;   // Wake up on exit from ISR

	while(1){
		ADC14->CTL0 |= ADC14_CTL0_ENC |ADC14_CTL0_SC ;
				__wfi();                            // alternatively you can also use __sleep();

				joystickResult = ADC14->MEM[0];
	}
}

void ADC14_IRQHandler(void)
{
	uint64_t status = MAP_ADC14_getEnabledInterruptStatus();
	uint16_t curADCResult[3];

	MAP_ADC14_clearInterruptFlag(status);

	if (ADC_INT2 & status)
	{
		curADCResult[0] = ADC14_getResult(ADC_MEM0);
		curADCResult[1] = ADC14_getResult(ADC_MEM1);
		curADCResult[2] = ADC14_getResult(ADC_MEM2);

		Mailbox_post(adc_result, &curADCResult, BIOS_NO_WAIT);
	}
	ADC14->CLRIFGR0 = ADC14_CLRIFGR0_CLRIFG0;
}
