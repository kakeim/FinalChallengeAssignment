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
#include <ti/sysbios/knl/Semaphore.h>

/* DriverLib Includes */
#include "driverlib.h"
#include "grlib.h"
#include "Crystalfontz128x128_ST7735.h"
#include <xdc/cfg/global.h>

/* TI-RTOS Header files */
#include <ti/drivers/GPIO.h>

/* Standard Includes */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/runtime/System.h>

/* Board Header file */
#include "Board.h"


void initClocks(void);
void initGfx(void);
void initADC(void);
void accel_task(void);
void ADC14_IRQHandler(void);
void buttonInit(void);
void SW1_IRQHandler(void);
void SW2_IRQHandler(void);
void initTimers32(void);
void Timer32_IRQHandler(void);
void initPWM(void);

#endif /* FUNCTIONS_H_ */
