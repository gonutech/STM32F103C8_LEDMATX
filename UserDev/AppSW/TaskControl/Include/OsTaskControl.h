/// @file osTaskControl.h
/// @brief OS task control support

#ifndef __OSTASKCONTROL_H__
#define __OSTASKCONTROL_H__

// External include files
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
// Internal include files
#include "HwLib.h"

// Os task control function prototype
void startSensorSlowTask1s(void const * argument);
void startDebugSlowTask500ms(void const * argument);
void startLed7SegFastTask1ms(void const * argument);


// External specific task function interface
extern void uartDebugI_DebugSlowTask(void);

// External queue handle
extern QueueHandle_t xTHSensorQueue;

#endif /* __OSTASKCONTROL_H__ */
