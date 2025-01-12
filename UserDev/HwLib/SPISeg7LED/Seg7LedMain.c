/// @file Seg7LedMain.c
/// @brief Seven segmant LED main function definition

#include "Seg7Led.h"
#include "MAX7219.h"
#include "UartDebug.h"

/// @brief Led7Seg scanner call back function
/// @param [in] None
/// @param [out] None
/// @return None
void ledMatI_LedScannerInit(void) {
  /* To do */
  static uint8_t dbInitCompleteBuff[] = "LedInitFinished\n";

  MAX7219_Init();

  uartDebugI_PrintDebugInfo(dbInitCompleteBuff, 16);
}

/// @brief Led7Seg scanner call back function
/// @param [in] None
/// @param [out] None
/// @return None
void ledMatI_LedScannerTask(void) {
  /* To do */
  // setLedDisplayAll(ledDigitMinus);
  // setLedDisplayPeriod(335, 601);

}

void ledMatI_UpdateData(uint16_t temp_int, uint16_t humi_int) {
  static uint8_t alpha = 0;
  if (alpha++ >= 38) alpha = 0;
  Print_Alphabet(alpha);
}
