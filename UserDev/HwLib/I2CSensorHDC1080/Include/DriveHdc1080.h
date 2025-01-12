/* 
  ******************************************************************************
  * File Name          : DriveHdc1080.h
  * Description        : Hdc1080 driver
  ******************************************************************************
  *
  * COPYRIGHT(c) 2016 s54mtb
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  *
  ******************************************************************************

   The hdc1080 communicates with the host controller over a digital I2C 
   interface. The 7-bit base slave address is 0x40
   This file have been updated by Gonu
*/

/// @file DriveHdc1080.h
/// @brief HDC1080 register and interface function declaration

#ifndef __hdc1080_h__
#define __hdc1080_h__

#include "stm32f1xx_hal.h"

/* Register addresses */
#define HDC1080_TEMPERATURE       0x00
#define HDC1080_HUMIDITY          0x01
#define HDC1080_CONFIG            0x02
#define HDC1080_SERIAL_ID1        0xfb
#define HDC1080_SERIAL_ID2        0xfc
#define HDC1080_SERIAL_ID3        0xfd
#define HDC1080_ID_MANU           0xfe
#define HDC1080_ID_DEV            0xff

#define HDC1080_RH_RES_14         0x00
#define HDC1080_RH_RES_11         0x01
#define HDC1080_RH_RES8           0x02

#define HDC1080_T_RES_14          0x00
#define HDC1080_T_RES_11          0x01 

#define HDC1080_ADDR              0x40

/* High level API */
HAL_StatusTypeDef hdc1080_device_info_get(I2C_HandleTypeDef *hi2c, uint64_t *serial, uint16_t *manuf, uint16_t *device);
HAL_StatusTypeDef hdc1080_device_fearture_config(I2C_HandleTypeDef *hi2c, uint8_t heat, uint8_t mode, uint8_t tres, uint8_t hres);
HAL_StatusTypeDef hdc1080_device_temp_humi_data_get(I2C_HandleTypeDef *hi2c, double *temp,  double *humi);

/* Register interface */
HAL_StatusTypeDef hdc1080_read_reg_info(I2C_HandleTypeDef *hi2c, uint8_t reg, uint16_t *val);
HAL_StatusTypeDef hdc1080_read_reg_temp_humi(I2C_HandleTypeDef *hi2c, uint16_t *value1, uint16_t *value2);
HAL_StatusTypeDef hdc1080_read_reg_th(I2C_HandleTypeDef *hi2c, uint8_t reg, uint16_t *val);
HAL_StatusTypeDef hdc1080_write_reg_config(I2C_HandleTypeDef *hi2c, uint16_t val);

#endif
