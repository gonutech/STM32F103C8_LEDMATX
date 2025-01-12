/*
 * MAX7219.h
 *
 *  Created on: 16 Jan 2023
 *      Author: EDISON NGUNJIRI
 */
#ifndef MAX7219_H_
#define MAX7219_H_

/// Hardware dependence include
#include "stm32f1xx_hal.h"

/// Internal HwLib include
#include "HwLib.h"


#include "stdint.h"
// External HAL driver include
#include "spi.h"

extern SPI_HandleTypeDef hspi2;


void Print_Alphabet(uint8_t i);
void MAX7219_Init(void);


#endif /* MAX7219_H_ */
