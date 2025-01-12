/// @file UartDebugMain.c
/// @brief Uart debug main function definition
#include "UartDebug.h"

#if defined (UART_DEBUG)
  #if defined(__GNUC__)
  int _write(int fd, char * ptr, int len)
  {
    HAL_UART_Transmit(&huart2, (uint8_t *) ptr, len, HAL_MAX_DELAY);
    return len;
  }
  #elif defined (__ICCARM__)
  #include "LowLevelIOInterface.h"
  size_t __write(int handle, const unsigned char * buffer, size_t size)
  {
    HAL_UART_Transmit(&huart2, (uint8_t *) buffer, size, HAL_MAX_DELAY);
    return size;
  }
  #elif defined (__CC_ARM)
  int fputc(int ch, FILE *f)
  {
      HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
      return ch;
  }
  #endif /* __GNUC__ */
#endif /* UART_DEBUG */

void ledDebugI_SetStatus(Led_Status_t status) {
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, (GPIO_PinState)status );
}

void ledDebugI_Toggle(void) {
  HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
}


/// @brief Uart debug call back function
/// @param [in] None 
/// @param [out] None
/// @return None
static uint8_t counter = 0;
void uartDebugI_DebugSlowTask(void) {
  // Do some debug here
  uint8_t bufftest[10] = "Tick \n";
  bufftest[4] = counter + 0x30;
  HAL_UART_Transmit(&huart2, (uint8_t *) bufftest, 6, HAL_MAX_DELAY);
  if (counter++ >= 9) counter = 0;
  // uart_debug("Tick\n");
}

// Interface function for printing some thing
void uartDebugI_PrintDebugInfo(uint8_t * buffer, uint8_t size) {
  // Send data
  HAL_UART_Transmit(&huart2, (uint8_t *) buffer, size, HAL_MAX_DELAY);

}