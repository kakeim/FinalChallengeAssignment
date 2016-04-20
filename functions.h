/*
 * functions.h
 *
 *  Created on: Apr 19, 2016
 *      Author: Kevin
 */

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Mailbox.h>

/* DriverLib Includes */
#include "driverlib.h"
#include "grlib.h"
#include "Crystalfontz128x128_ST7735.h"
#include <xdc/cfg/global.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

void initADC(void);
void accel_task(void);
void ADC14_IRQHandler(void);
void initGfx(void);


#endif /* FUNCTIONS_H_ */
