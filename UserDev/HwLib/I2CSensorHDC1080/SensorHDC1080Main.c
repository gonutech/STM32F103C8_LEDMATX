/// @file SensorHDC1080.c
/// @brief Temperature and humidity sensor function definition

#include "UartDebug.h"

/// @brief HDC1080 Initialize function
/// @param [in]  None 
/// @param [out] None
/// @return None
void sensorHDC1080I_THSensorInit(void) {
  uint8_t serial[5], manuf[2], devID[2];
  uint8_t statusCheck = 0;
  /// Init device by getting device info
  hdc1080P_THSensorInit();
  /// Read information all
  statusCheck += (uint8_t)hdc1080P_THSensorGetSerial(serial);
  statusCheck += (uint8_t)hdc1080P_THSensorGetManufacturer(manuf);
  statusCheck += (uint8_t)hdc1080P_THSensorGetDeviceID(devID);

  if (statusCheck != 0) {
    uart_debug("Init failure!!!\n");
  } else {
    uart_debug("Init ok:\n");
    // Serial info
    uart_debug("Sensor serial = 0x");
    for (int i = NUMBYTE_SERIAL-1; i >= 0; i--) uart_debug("%x", serial[i]);
    uart_debug("\n");
    // Manufacturer info
    uart_debug("Sensor manufacturer = 0x");
    for (int i = NUMBYTE_MANUF-1; i >= 0; i--) uart_debug("%x", manuf[i]);
    uart_debug("\n");
    // DeviceID info
    uart_debug("Sensor device ID = 0x");
    for (int i = NUMBYTE_DEVID-1; i >= 0; i--) uart_debug("%x", devID[i]);
    uart_debug("\n");
  }
}

/// @brief HDC1080 config function
/// @param [in]  None 
/// @param [out] None
/// @return None
/*  Note: Be carefull when calling this function
    It should be called at least 20ms after device power up */
void sensorHDC1080I_THSensorConfig(void) {
  if (HwOk != hdc1080P_THSensorConfig()) {
    uart_debug("Invalid configuration HDC1080!!!\n");
  } else {
    uart_debug("HDC1080 config success\n");
  }
}


/// @brief HDC1080 call back function
/// @param [in]  None 
/// @param [out] None
/// @return None
void sensorHDC1080I_THSensorSlowTask(uint16_t *temp_x10, uint16_t *humi_x10) {
  uint16_t temp_int, humi_int;
  THSensorData_t sensorData;
  /// Callback some thing here
  ledDebugI_Toggle();
  /// Update TH data
  if (HwOk != hdc1080P_THSensorUpdateDataTH()) {
    uart_debug("Error update TH data!!!\n");
  } else {
    uart_debug("Data TH updated\n");
    /// Read data
    sensorData =  hdc1080P_THSensorReadTempHumi();
    temp_int = (sensorData.iTemperature/10u);
    humi_int = (sensorData.iHumidity/10u);
  }

  *temp_x10 = temp_int;
  *humi_x10 = humi_int;
  
}
