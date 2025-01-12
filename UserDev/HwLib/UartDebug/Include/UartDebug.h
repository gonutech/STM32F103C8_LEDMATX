/// @file UartDebug.h
/// @brief Uart debug function prototype

/// Hardware dependence include
#include "stm32f1xx_hal.h"

/// Internal HwLib include
#include "HwLib.h"

/// Redirect printf if debug enables
#if defined(UART_DEBUG)
  #include <stdio.h>
  #define uart_debug(f_, ...)   printf((f_), ##__VA_ARGS__)
#else
  #define uart_debug(f_, ...)
#endif /* UART_DEBUG */

typedef enum {
  LED_ON = 0,
  LED_OFF
} Led_Status_t;

/// External variable
extern UART_HandleTypeDef huart2;

/// Led debug prototype
void ledDebugI_SetStatus(Led_Status_t status);
void ledDebugI_Toggle(void);

void uartDebugI_DebugSlowTask(void);
