/// @file osTaskControlMain.c
/// @brief OS task control support main function definition

#include "osTaskControl.h"

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
  // TH sensor buffer creating
  THSensorInfo_t sensorInfo;
  
  uint16_t temp_ix10, humi_ix10;
  /// Cast unused argument
  (void)argument;
  
  /// Initialilze module 
  sensorHDC1080I_THSensorInit();
  
  /// Wait for device waked up and config fearture
  vTaskDelay(20);
  sensorHDC1080I_THSensorConfig();
  
  /// Loop forever
  while (1) {
    /// Sensor slow task function interface called
    sensorHDC1080I_THSensorSlowTask(&temp_ix10, &humi_ix10);
    sensorInfo.temperature = temp_ix10;
    sensorInfo.humidity    = humi_ix10;
    uart_debug("Temperature: %d\n", sensorInfo.temperature);
    uart_debug("Humidity: %d\n", sensorInfo.humidity);
    /// Put data from t/h sensor to queue
    xQueueSend(xTHSensorQueue, (void*)&sensorInfo, QUEUE_PUT_TIMEOUT);
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
  
  /// Create buffer sensor get
  THSensorInfo_t sensorInfoGet;
  
  /// Casting unused argument
  (void)argument;
  
  /// Config led7sed - initialize and display some thing at start up time
  /* To do */
  
  /// Loop forever
  while (1) {
    /// Led scanner fast task called
    led7SegI_LedScannerTask();
    /// Counting for synch data get set
    if (tick1msCount++ >= 1000) {
      tick1msCount = 0;
      if (pdPASS == xQueueReceive(xTHSensorQueue, &sensorInfoGet, QUEUE_GET_TIMEOUT)) {
        led7SegI_UpdateData(sensorInfoGet.temperature, sensorInfoGet.humidity);
      } else {
        uart_debug("Os get error\n");
      }
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
