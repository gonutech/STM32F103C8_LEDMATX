/// @file InterHDC1080.h
/// @brief HDC1080 user interface function definition
#ifndef __INTERHDC1080_H__
#define __INTERHDC1080_H__

// Internal include file
#include "HwLib.h"
// External include file
#include "DriveHdc1080.h"

#define NUMBYTE_SERIAL    5 /* 40 bits */
#define NUMBYTE_MANUF     2 /* 16 bits */
#define NUMBYTE_DEVID     2 /* 16 bits */

// Heating config feature
#define FEAR_HEAT_DISABLE       (0)
#define FEAR_HEAT_ENABLE        (1)
// Mode config fearture
#define FEAR_MODE_TEMP_OR_HUMI  (0)
#define FEAR_MODE_TEMP_AND_HUMI (1)
// Temperature resolution config feature
#define FEAR_TRES_14BIT         (0)
#define FEAR_TRES_11BIT         (1)
// Humidity resolution config feature
#define FEAR_HRES_14BIT         (0x00)
#define FEAR_HRES_11BIT         (0x01)
#define FEAR_HRES_8BIT          (0x10)

typedef enum {
  VoltageUpper28 = 0,
  VoltageLower28,
} HDC1080DevVoltageStatus_t;

// HDC1080 configurable fearture
typedef struct {
  uint8_t Heat;
  uint8_t Mode;
  uint8_t Tres;
  uint8_t Hres;
} HDC1080DevFeatureConfig_t;

// HDC1080 sensor main output temperature and humidity
typedef struct {
  double Temperature;
  double Humidity;
  HDC1080DevVoltageStatus_t VoltageStatus;
} HDC1080DevDataTH_t;

// HDC1080 sensor raw data
typedef struct {
  uint16_t iTemperature;
  uint16_t iHumidity;
} THSensorData_t;

// External variable from I2C 
extern I2C_HandleTypeDef hi2c1;

// Function interface provided
HwStatus_t hdc1080P_THSensorInit(void);
HwStatus_t hdc1080P_THSensorGetSerial(uint8_t *buff5Bytes);
HwStatus_t hdc1080P_THSensorGetManufacturer(uint8_t *buff2Bytes);
HwStatus_t hdc1080P_THSensorGetDeviceID(uint8_t *buff2Bytes);

HwStatus_t hdc1080P_THSensorConfig(void);
HwStatus_t hdc1080P_THSensorUpdateDataTH(void);
THSensorData_t hdc1080P_THSensorReadTempHumi(void);

#endif /* __INTERHDC1080_H__ */
