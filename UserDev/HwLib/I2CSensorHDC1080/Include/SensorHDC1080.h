/// @file SensorHDC1080.h
/// @brief Sensor HDC1080 driver function interface

#ifndef __SENSORHDC1080_H__
#define __SENSORHDC1080_H__

// Internal HwLib include
#include "HwLib.h"

// Interface function
void sensorHDC1080I_THSensorInit(void);
void sensorHDC1080I_THSensorConfig(void);
void sensorHDC1080I_THSensorSlowTask(uint16_t *temp, uint16_t *humi);

#endif /* __SENSORHDC1080_H__ */
