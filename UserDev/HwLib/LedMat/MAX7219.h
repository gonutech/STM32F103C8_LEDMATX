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

void MAX7219_Init(void);
void Print_Alphabet(uint8_t i);


void Print_RowCheck(uint8_t i);



void ledMatI_ClearDisplay(void);
void ledMatI_SetSingleRowDisplay(uint8_t RowNo, uint8_t RowData);
void ledMatI_SetSingleRowSingleLedDisplay(uint8_t RowNo, uint8_t SingleLedId);
void ledMatI_SetSingleRowIncreaseLedDisplay(uint8_t RowNo, uint8_t IncreaseLedId);
void ledMatI_SetSingleRowIncreaseInvLedDisplay(uint8_t RowNo, uint8_t IncreaseLedId);


#endif /* MAX7219_H_ */
