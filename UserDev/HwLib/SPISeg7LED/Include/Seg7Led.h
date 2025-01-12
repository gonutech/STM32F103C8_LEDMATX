/// @file Seg7Led.h
/// @brief Seven segmant LED main function declaration

#ifndef __SEG7LED_H__
#define __SEG7LED_H__

#include "stdint.h"

/// Interface function
void led7SegI_LedScannerTask(void);
void led7SegI_LedScannerInit(void);
void led7SegI_UpdateData(uint16_t temp_int, uint16_t humi_int);

#endif /* __SEG7LED_H__ */
