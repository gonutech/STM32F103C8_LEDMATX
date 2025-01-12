/// @file Seg7Led.h
/// @brief Seven segmant LED main function declaration

#ifndef __SEG7LED_H__
#define __SEG7LED_H__

#include "stdint.h"

/// Interface function
void ledMatI_LedScannerInit(void);

// Fast task 1ms
void ledMatI_LedScannerTask(void);
// Slow task 1s
void ledMatI_UpdateData(uint16_t temp_int, uint16_t humi_int);

#endif /* __SEG7LED_H__ */
