/// @file osTaskControlMain.c
/// @brief OS task control support main function definition

#include "osTaskControl.h"
#include "Seg7Led.h"
#include "UartDebug.h"

#define QUEUE_PUT_TIMEOUT   0
#define QUEUE_GET_TIMEOUT   0

typedef struct {
  uint16_t temperature;
  uint16_t humidity;
} THSensorInfo_t;

/// @brief Sensor slow task 1s create
/// @param [out] argument this argument is not used at the moment
/// @return None
void startSensorSlowTask1s(void const * argument) {
  TickType_t xNextWakeTime;
  /// Create task with 1Hz exactly
  const TickType_t xFrequency = 1000;

  /// Loop forever
  while (1) {
    /// Sensor slow task function interface called
    /// Delay until
    vTaskDelayUntil(&xNextWakeTime, xFrequency);
  }
}

/// @brief Led7Seg scanner 2ms create
/// @param [out] argument this argument is not used at the moment
/// @return None
void startLed7SegFastTask1ms(void const *argument) {
  TickType_t xNextWakeTime;
  /// Create task with 500Hz exactly
  const TickType_t xFrequency = 1;
  /// Counter for updating sensor info
  uint16_t tick1msCount = 0;
  
  /// Casting unused argument
  (void)argument;
  
  /// Config led7sed - initialize and display some thing at start up time
  ledMatI_LedScannerInit();
  
  /// Loop forever
  while (1) {
    /// Fast task called
    ledMatI_LedScannerTask();
    /// Counting for synch data get set
    if (tick1msCount++ >= 500) {
      tick1msCount = 0;
      // Slow task inside fast task
      ledMatI_UpdateData(1,1);
    }
    /// Delay until
    vTaskDelayUntil(&xNextWakeTime, xFrequency);
  }
}

/// @brief Debug heartbeat slow task 500ms create
/// @param [out] argument this argument is not used at the moment
/// @return None
void startDebugSlowTask500ms(void const * argument) {
  TickType_t xNextWakeTime;
  /// Create task with 1Hz exactly
  const TickType_t xFrequency = 500;
  
  /// Cast unused argument
  (void)argument;
  
  /// Loop forever
  while (1) {
    /// Sensor slow task function interface called
    uartDebugI_DebugSlowTask();
    /// Delay until
    vTaskDelayUntil(&xNextWakeTime, xFrequency);
  }
}
