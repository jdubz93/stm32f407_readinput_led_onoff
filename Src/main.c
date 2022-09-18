/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Josh Williams
 * @brief          : Blinky application for stm32f407G MCU
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
// 
#include <stdint.h>
#include <stdio.h>

//Testing of bits (&)
//Setting of bits (|)
//Clearing of bits (~ and &)
//Toggling of bits (^)

void delayFunc(void);
void ledOff(uint32_t* pGPIO_D_OutData, uint32_t* pGPIO_D_ModeReg);
void ledOn(uint32_t* pGPIO_D_ModeReg, uint32_t* pGPIO_D_OutData);

int main(void)
{
	uint32_t *pClockCtrlReg			= (uint32_t*)0x40023830;
	uint32_t *pGpiodModeReg			= (uint32_t*)0x40020C00;
	uint32_t *pGpiodOutputDataReg	= (uint32_t*)0x40020C14;

	uint32_t* pGPIO_A_ModeReg 		= (uint32_t*)0x40020000;
	uint32_t* pGPIO_A_InputDataReg	= (uint32_t*)0x40020010;

	// enable the clock for GPIOD peripheral (port 3) in the AHB1ENR
	*pClockCtrlReg |= (1 << 3);
	// enable the clock for GPIOA peripheral (port 0) in the AHB1ENR
	*pClockCtrlReg |= (1 << 0);

	/** clear 0 and 1st bit positions on GPIO Moder A. 01: General purpose output mode
	 ** 3 = 0011, 0 = port 0, this clears the first two bit positions of port 0 */
	*pGPIO_A_ModeReg &= ~(3 << 0); // For example take 0011 and shift it 0 spaces: 0011 ->0011

	/** ugly version */
	// *pGPIO_A_ModeReg &= ~(1 << 0);
	// *pGPIO_A_ModeReg &= ~(1 << 1);
	// *pGPIO_A_ModeReg |= (1 << 0);

	while(1) {
		// read GPIO A input data
		uint8_t pin_PA0_status = (uint8_t)(*pGPIO_A_InputDataReg & 0x01); // or 0x1 would work too.
		if (pin_PA0_status) {
			ledOn(pGpiodModeReg, pGpiodOutputDataReg);
			delayFunc();
		} else {
			ledOff(pGpiodModeReg, pGpiodOutputDataReg);
			delayFunc();
		}

		/** ugly version */
		// if (*pGPIO_A_InputDataReg &(1 << 0)) {
		// 	ledOn(pGpiodModeReg, pGpiodOutputDataReg);
		// 	delayFunc();
		// } else {
		// 	ledOff(pGpiodModeReg, pGpiodOutputDataReg);
		// 	delayFunc();
		// }
	}
}

void delayFunc(void) {
	for(uint64_t i = 0; i < 1000000; i++) {
		// do nothing
	}
}

void ledOff(uint32_t* pGPIO_D_OutData, uint32_t* pGPIO_D_ModeReg) {
	// turn off led
	*pGPIO_D_OutData	&= 0xFFFFFFFF;
	*pGPIO_D_ModeReg	&= 0x0000;
}

void ledOn(uint32_t* pGPIO_D_ModeReg, uint32_t* pGPIO_D_OutData) {
	// clear the bits using mask
	*pGPIO_D_ModeReg	&= ~(3 << 10);
	*pGPIO_D_ModeReg	|= (1 << 10);
	// set the 5th pin
	*pGPIO_D_OutData	|= (1 << 5);
}
