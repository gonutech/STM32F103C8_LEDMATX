/// @file Seg7Led.h
/// @brief Seven segmant LED main function declaration

#ifndef __SEG7LED_H__
#define __SEG7LED_H__

#include "stdint.h"
#include "stm32f1xx_hal.h"

extern ADC_HandleTypeDef hadc1;

/// Interface function
void ledMatI_LedScannerInit(void);

// Fast task 1ms
uint16_t ledMatI_LedScannerTask(void);
// Slow task 1s
void ledMatI_UpdateData(void);

#endif /* __SEG7LED_H__ */
