/*
 * functions.h
 *
 *  Created on: Apr 19, 2016
 *      Author: Kevin
 */

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

/* DriverLib Includes */
#include "driverlib.h"

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

void initADC(void);
void accel_task(void);
void ADC14_IRQHandler(void);

#endif /* FUNCTIONS_H_ */
