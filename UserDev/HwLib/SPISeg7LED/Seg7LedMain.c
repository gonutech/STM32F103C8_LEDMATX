/// @file Seg7LedMain.c
/// @brief Seven segmant LED main function definition

#include "Seg7Led.h"
#include "UartDebug.h"
#include "DriveSeg7Led.h"

/// @brief Led7Seg scanner call back function
/// @param [in] None
/// @param [out] None
/// @return None
void led7SegI_LedScannerInit(void) {
  /* To do */
  setLedDisplayAll(ledDigitMinus);
  uart_debug("LED 7 Seg initialize\n");
  uart_debug("Do it later\n");
}

/// @brief Led7Seg scanner call back function
/// @param [in] None
/// @param [out] None
/// @return None
void led7SegI_LedScannerTask(void) {
  /* To do */
  // setLedDisplayAll(ledDigitMinus);
  // setLedDisplayPeriod(335, 601);
  ledDisplayUpdatePeriod();
}

void led7SegI_UpdateData(uint16_t temp_int, uint16_t humi_int) {
  ledDisplayUpdateData(temp_int, humi_int);
}
