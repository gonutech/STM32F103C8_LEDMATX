/// @file InterHDC1080.c
/// @brief HDC1080 user interface function definition

/* Note: This file is created for the interface between user and low level device.
   Expectation of the file is the middle layer between
      + Software high level function call and 
      + Device hardware sensor register and MCU Hal driver
*/
#include "InterHDC1080.h"

#define RETRY_MAX         3

/* Global data (damos) *******************************************************/
HwStatus_t sensorHDC1080InfoRead_xdb   = HwFail; /* Device info read  flag */
HwStatus_t sensorHDC1080Configured_xdb = HwFail; /* Device configured flag */

uint8_t sensorHDC1080Serial_xdu8[NUMBYTE_SERIAL]; /* Device serial buffer  */
uint8_t sensorHDC1080Manuf_xdu8[NUMBYTE_MANUF];   /* Device manuf  buffer  */
uint8_t sensorHDC1080DevID_xdu8[NUMBYTE_DEVID];   /* Device ID     buffer  */
HDC1080DevDataTH_t sensorHDC1080DevData_xdx = {   /* Sensor data           */
  0.0f,
  0.0f,
  VoltageUpper28
};
/* Device config data                                                      */
HDC1080DevFeatureConfig_t sensorHDC1080ConfigData_gdx = {
  FEAR_HEAT_DISABLE,
  FEAR_MODE_TEMP_AND_HUMI,
  FEAR_TRES_14BIT,
  FEAR_HRES_14BIT
};

/* Internal function definition  *********************************************/
/// @brief Set flag device info read
void setDevInfoRead(HwStatus_t status) {
  sensorHDC1080InfoRead_xdb = status;
}
/// @brief Set flag device configured
void setDevConfigured(HwStatus_t status) {
  sensorHDC1080Configured_xdb = status;
}
/// @brief Set sensor data temperature and humidity
void setSensorDataTH(double temp, double humi) {
  sensorHDC1080DevData_xdx.Temperature = temp;
  sensorHDC1080DevData_xdx.Humidity = humi;
}
/// @brief Set sensor serial data
void setSerialData(uint64_t inData) {
  // Ensure buff is 40 bit (5 bytes) data pointer!!!
  for (int i = 0; i < NUMBYTE_SERIAL; i++) {
    *(sensorHDC1080Serial_xdu8 + i) = (uint8_t)(inData >> (i*8));
  }
}
/// @brief Set sensor manufacturer data
void setManufacturerData(uint16_t inData) {
  *sensorHDC1080Manuf_xdu8 = (uint8_t)(inData);
  *(sensorHDC1080Manuf_xdu8 + 1) = (uint8_t)(inData >> 8);
}
/// @brief Set sensor ID data
void setDeviceIDData(uint16_t inData) {
  *sensorHDC1080DevID_xdu8 = (uint8_t)(inData);
  *(sensorHDC1080DevID_xdu8 + 1) = (uint8_t)(inData >> 8);
}

/* Global function definition ************************************************/
/// @brief Getting serial 40 bit data from device
/// @param [out] buff5Bytes Buffer 40 bits to store the serial
/// @return Read serial status (true/false)
HwStatus_t hdc1080P_THSensorGetSerial(uint8_t *buff5Bytes) {
  // If device info have not been read -> return false
  if (sensorHDC1080InfoRead_xdb != HwOk) return HwFail;
  // Otherwise copy to buffer
  for (int i = 0; i < NUMBYTE_SERIAL; i++) {
    *(buff5Bytes + i) = *(sensorHDC1080Serial_xdu8 + i);
  }
  return HwOk;
}

/// @brief Getting manufacturer 16 bit data from device
/// @param [out] buff2Bytes Buffer 16 bits to store the manufacturer
/// @return Read manufacturer status (true/false)
HwStatus_t hdc1080P_THSensorGetManufacturer(uint8_t *buff2Bytes) {
  // If device info have not been read -> return false
  if (sensorHDC1080InfoRead_xdb != HwOk) return HwFail;
  // Otherwise copy to buffer
  for (int i = 0; i < NUMBYTE_MANUF; i++) {
    *(buff2Bytes + i) = *(sensorHDC1080Manuf_xdu8 + i);
  }
  // Return true
  return HwOk;
}

/// @brief Getting device id 16 bit data from device
/// @param [out] buff2Bytes Buffer 16 bits to store the device id
/// @return Read device id status (true/false)
HwStatus_t hdc1080P_THSensorGetDeviceID(uint8_t *buff2Bytes) {
  // If device info have not been read -> return false
  if (sensorHDC1080InfoRead_xdb != HwOk) return HwFail;
  // Otherwise copy to buffer
  for (int i = 0; i < NUMBYTE_MANUF; i++) {
    *(buff2Bytes + i) = *(sensorHDC1080DevID_xdu8 + i);
  }
  // Return true
  return HwOk;
}

/// @brief Initialize the hardware sensor
/// @param None
/// @return Init status
HwStatus_t hdc1080P_THSensorInit (void) {
  // In the wake up time, first 15ms only register of device info are available
  // The initialize will get the device id within the start uptime 
  uint64_t tmpSerial = 0;
  uint16_t tmpManuf, tmpDevID;
  HAL_StatusTypeDef readRegStatus;
  
  for (int i = 0; i < RETRY_MAX; i++) {
    readRegStatus = hdc1080_device_info_get (&hi2c1, &tmpSerial, 
                                                    &tmpManuf, &tmpDevID);
    if (readRegStatus == HAL_OK) break;
  }
  // Retrun fail when regsister cannot be read
  if (readRegStatus != HAL_OK) {
    setDevInfoRead(HwFail);
    return HwFail;
  }

  // Otherwise update data and return true;
  setSerialData(tmpSerial);
  setManufacturerData(tmpManuf);
  setDeviceIDData(tmpDevID);
  setDevInfoRead(HwOk);
  
  return HwOk;
}

/// @brief Configurate the hardware sensor feature
/// @param None
/// @return Config status
HwStatus_t hdc1080P_THSensorConfig(void) {
  HAL_StatusTypeDef status;

  // Configurate the feature of HDC1080
  sensorHDC1080ConfigData_gdx.Heat = FEAR_HEAT_DISABLE;
  sensorHDC1080ConfigData_gdx.Mode = FEAR_MODE_TEMP_AND_HUMI;
  sensorHDC1080ConfigData_gdx.Tres = FEAR_TRES_14BIT;
  sensorHDC1080ConfigData_gdx.Hres = FEAR_HRES_14BIT;
  
  // Calling Gonu API
  status = hdc1080_device_fearture_config(&hi2c1,  sensorHDC1080ConfigData_gdx.Heat,
                                            sensorHDC1080ConfigData_gdx.Mode,
                                            sensorHDC1080ConfigData_gdx.Tres,
                                            sensorHDC1080ConfigData_gdx.Hres);
  if (HAL_OK != status) return HwFail;
  // Set device configured flag
  setDevConfigured(HwOk);
  return HwOk;
}

/// @brief Get temperature and humi data
/// @param [out] temp pointer to temperature buffer
/// @param [out] humi pointer to humidity buffer
/// @return None
THSensorData_t hdc1080P_THSensorReadTempHumi(void) {
  THSensorData_t retData;
  retData.iTemperature = (uint16_t)(sensorHDC1080DevData_xdx.Temperature * 100.0f);
  retData.iHumidity    = (uint16_t)(sensorHDC1080DevData_xdx.Humidity*100.0f);
  return retData;
}

/// @brief Update sensor temperature and humidity data
/// @param None
/// @return Read data status
HwStatus_t hdc1080P_THSensorUpdateDataTH(void) {
  double tmpTemp, tmpHumi;
  // Check for device configured
  if (HwOk != sensorHDC1080Configured_xdb) return HwFail;
  
  // Read temperature and humidity data
  if (HAL_OK != hdc1080_device_temp_humi_data_get(&hi2c1, &tmpTemp, &tmpHumi)) return HwFail;
  else setSensorDataTH(tmpTemp, tmpHumi);
  
  // Return read data success
  return HwOk;
}
