/* 
  ******************************************************************************
  * File Name          : DriveHdc1080.c
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
/// @brief HDC1080 register and interface function definition

#include "DriveHdc1080.h"
/*
 * HDC1080 measurement procedure: 

1. Configure the acquisition parameters in config register (address 0x02):
    (a) Set the acquisition mode to measure both temperature
        and humidity by setting Bit[12] to 1.
    (b) Set the desired temperature measurement resolution:
        –  Set Bit[10] to 0 for 14 bit resolution.
        –  Set Bit[10] to 1 for 11 bit resolution.
    (c) Set the desired humidity measurement resolution:
        –  Set Bit[9:8] to 00 for 14 bit resolution.
        –  Set Bit[9:8] to 01 for 11 bit resolution.
        –  Set Bit[9:8] to 10 for 8 bit resolution.

2. Trigger the measurements by writing I2C read reg. address with adr = HDC1080_TEMPERATURE.

3. Wait for the measurements to complete, based on the conversion time
        - For 14 bit resolution time for conversion is 6.50ms
        - For 11 bit resolution time for conversion is 3.58ms
        - For 08 bit resolution time for conversion is 2.50ms

4. Read the output data:
Read the temperature data from register address 0x00, followed by the humidity 
data from register address 0x01 in a single transaction. A read operation will 
return a NACK if the contents of the registers have not been updated.

 * @hi2c:  handle to I2C interface
 * @temp_res    :  temperature measurement resolution:
 *                    - HDC1080_T_RES_14 or
 *                    - HDC1080_T_RES_11
 * @humidres    :  humidity readout resolution
 *                    - HDC1080_RH_RES_14 or
 *                    - HDC1080_RH_RES_11 or
 *                    - HDC1080_RH_RES_8 
 * @heater      :  heater enable (0 = disabled or 1 = enabled)
 * @bat_stat    :  supply voltage 
 *                    - 0 when Ucc > 2,8V
 *                    - 1 when Ucc < 2,8V
 * @temperature :  floating point temperature result, unit is °C
 * @humidity    :  floating point humidity result, unit is RH%
 * Returns HAL status.
 */

/* Delay set to 20ms greater than 6.5ms for 14 bit conversion time            */
#define CONVERSION_TIME       25 /* Lower than task cycle/2 at least         */

/******************************************************************************
 * High level API ************************************************************/

/* Gonu updated
 * hdc1080_device_info_get() - Get the device ID from the device
 * @i2c: handle to I2C interface
 * @serial: 40 bit serial number
 * @manuf : 16 bit manufacturer ID
 * @device: 16 bit device ID
 *
 * Returns 0 on success.
 */
HAL_StatusTypeDef hdc1080_device_info_get(I2C_HandleTypeDef *hi2c, uint64_t *serial, uint16_t *manuf, uint16_t *device)
{
  uint16_t ser[3];
  uint64_t tmp_serial = 0;
  HAL_StatusTypeDef error;

  // Read serial data
  error = hdc1080_read_reg_info(hi2c, HDC1080_SERIAL_ID1, &ser[0]);
  if (error != HAL_OK) return error;
  
  error = hdc1080_read_reg_info(hi2c, HDC1080_SERIAL_ID2, &ser[1]);
  if (error != HAL_OK) return error;
  
  error = hdc1080_read_reg_info(hi2c, HDC1080_SERIAL_ID3, &ser[2]);
  if (error != HAL_OK) return error;
  
  for (int i = 0; i < 3; i++) {
    tmp_serial |= ser[i] * (i*256);
  }
  *serial = tmp_serial;
  
  error = hdc1080_read_reg_info(hi2c, HDC1080_ID_MANU, manuf);
  if (error != HAL_OK) return error;

  error = hdc1080_read_reg_info(hi2c, HDC1080_ID_DEV, device);
  if (error != HAL_OK) return error;
  
  return HAL_OK;  /* Success */
}

/* Gonu updated
 * hdc1080_device_fearture_config() - Configure HDC1080 for specific fearture
 * @i2c: handle to I2C interface
 * @heat: Enable or disable heater
 * @mode: Single mearsure of dual temperature and humidity measurement
 * @tres: Resolution of temperature conversion
 * @hres: Resolution of humidity conversion
 *
 * Returns 0 on success.
 */
HAL_StatusTypeDef hdc1080_device_fearture_config(I2C_HandleTypeDef *hi2c,
            uint8_t heat, uint8_t mode, uint8_t tres, uint8_t hres) {
  HAL_StatusTypeDef error;
  uint16_t r = 0;
  uint16_t regConfirm;
  error = hdc1080_read_reg_info(hi2c, HDC1080_CONFIG, &r);
  if (error != HAL_OK) return error;

  r |= tres << 10;
  r |= hres <<  8;
  r |= mode << 12;
  r |= heat << 13;
  // Copy register
  regConfirm = r;
              
  // write config
  error = hdc1080_write_reg_config(hi2c, r);
  if (error != HAL_OK) return error;
  
  // Confirm reg
  error = hdc1080_read_reg_info(hi2c, HDC1080_CONFIG, &r);
  if (error != HAL_OK) return error;
  // Compare reg
  if (r != regConfirm) return HAL_ERROR;
  
  return HAL_OK;
}

/* Gonu updated
 * hdc1080_device_temp_humi_data_get() - Get temperature and humidity from device one shot
 * @i2c: handle to I2C interface
 * @temp: Buffer to store temperature
 * @humi: Buffer to store humidity
 *
 * Returns 0 on success.
 */
HAL_StatusTypeDef hdc1080_device_temp_humi_data_get(I2C_HandleTypeDef *hi2c,
double *temp,  double *humi) {
  HAL_StatusTypeDef error;
  uint16_t r_temp = 0;
  uint16_t r_humi = 0;
  double tmp;

  // Read dual register
  error = hdc1080_read_reg_temp_humi(hi2c, &r_temp, &r_humi);
  if (error != HAL_OK) return error;

  // Calculate temperature from raw data
  tmp = (double)r_temp;
  tmp = (tmp / 65536.0f) * 165.0f - 40.0f;
  *temp = tmp;  // °C

  // Calculate humidity from raw data
  tmp = (double)r_humi;
  tmp = (tmp / 65536.0f) * 100.0f;
  if (tmp>100.0f) tmp = 100.0f;
  if (tmp<0.0f) tmp = 0.0f;
  *humi = tmp;
  
  return HAL_OK;
}

/* Gonu updated
 * hdc1080_device_temp_humi_data_get2() - Get temperature and humidity from device double call
 * @i2c: handle to I2C interface
 * @temp: Buffer to store temperature
 * @humi: Buffer to store humidity
 *
 * Returns 0 on success.
 */
HAL_StatusTypeDef hdc1080_device_temp_humi_data_get2(I2C_HandleTypeDef *hi2c,
double *temp,  double *humi) {
  HAL_StatusTypeDef error;
  uint16_t r_temp = 0;
  uint16_t r_humi = 0;
  double tmp;

  // Read temperature register
  error = hdc1080_read_reg_th(hi2c, HDC1080_TEMPERATURE, &r_temp);
  if (error != HAL_OK) return error;

  // Calculate temperature from raw data
  tmp = (double)r_temp;
  tmp = (tmp / 65536.0f) * 165.0f - 40.0f;
  *temp = tmp;  // °C

  // Read humidity register
  error = hdc1080_read_reg_th(hi2c, HDC1080_HUMIDITY, &r_humi);
  if (error != HAL_OK) return error;
  // Calculate humidity from raw data
  tmp = (double)r_humi;
  tmp = (tmp / 65536.0f) * 100.0f;
  if (tmp>100.0f) tmp = 100.0f;
  if (tmp<0.0f) tmp = 0.0f;
  *humi = tmp;
  
  return HAL_OK;
}

/******************************************************************************
 * Register interface ********************************************************/

/* Gonu updated
 * hdc1080_read_reg_info() - Read device info register
 * @hi2c:  handle to I2C interface
 * @reg: Register address
 * @val: 16-bit register value from the hdc1080
 * Returns HAL status or HAL_ERROR for invalid parameters.
 */
HAL_StatusTypeDef hdc1080_read_reg_info(I2C_HandleTypeDef *hi2c, uint8_t reg, uint16_t *val)
{
  uint8_t buf[2];
  HAL_StatusTypeDef  error;

  // Check argument
  if (!((reg == HDC1080_CONFIG)     ||
        (reg == HDC1080_SERIAL_ID1) ||
        (reg == HDC1080_SERIAL_ID2) ||
        (reg == HDC1080_SERIAL_ID3) ||
        (reg == HDC1080_ID_MANU)    ||
        (reg == HDC1080_ID_DEV))) 
  return HAL_ERROR;
  
  buf[0] = reg;
  /* Read register */
  /* Send the read followed by address */
  error = HAL_I2C_Master_Transmit(hi2c,HDC1080_ADDR<<1,buf,1,100);
  if (error != HAL_OK)
    return error;
  
  /* Receive a 2-byte result */
  error = HAL_I2C_Master_Receive(hi2c, HDC1080_ADDR<<1 | 0x01, buf, 2, 1000);
  if (error != HAL_OK)
    return error;
  
  /* Result */
  *val = buf[0]*256+buf[1]; 

  return HAL_OK;  /* Success */
  
}

/* Gonu patch code for dual register read (temp and humi)
 * hdc1080_read_reg_temp_humi() - Read temp and humi register at the same time
 * @hi2c:  handle to I2C interface
 * @value1: 16-bit register value of temperature from the hdc1080
 * @value2: 16-bit register value of humidity    from the hdc1080
 * Returns HAL status or HAL_ERROR for invalid parameters.
 */
HAL_StatusTypeDef hdc1080_read_reg_temp_humi(I2C_HandleTypeDef *hi2c, uint16_t *value1, uint16_t *value2)
{
  uint8_t buf[4];
  HAL_StatusTypeDef  error;
  
  buf[0] = HDC1080_TEMPERATURE;
  /* Read register */
  /* Send the read followed by address */
  error = HAL_I2C_Master_Transmit(hi2c,HDC1080_ADDR<<1,buf,1,100);
  if (error != HAL_OK)
    return error;

  HAL_Delay(CONVERSION_TIME);
  
  /* Receive a 2-byte result */
  error = HAL_I2C_Master_Receive(hi2c, HDC1080_ADDR<<1 | 0x01, buf, 4, 1000);
  if (error != HAL_OK)
    return error;
  
  /* Result */
  *value1 = buf[0]*256+buf[1]; 
  *value2 = buf[2]*256+buf[3];
  return HAL_OK;  /* Success */
}

/* Gonu patch code for single register read temp or humi
 * hdc1080_read_reg_th() - Read temp or humi register
 * @hi2c:  handle to I2C interface
 * @reg: Register address temperature or humidity
 * @val: 16-bit register value of temperature or humidity from the hdc1080
 * Returns HAL status or HAL_ERROR for invalid parameters.
 */
HAL_StatusTypeDef hdc1080_read_reg_th(I2C_HandleTypeDef *hi2c, uint8_t reg, uint16_t *val)
{
  uint8_t buf[2];
  HAL_StatusTypeDef  error;
  
  // Check argument
  if (!((reg == HDC1080_TEMPERATURE)||
        (reg == HDC1080_HUMIDITY))) 
  return HAL_ERROR;
  
  buf[0] = reg;
  /* Read register */
  /* Send the read followed by address */
  error = HAL_I2C_Master_Transmit(hi2c,HDC1080_ADDR<<1,buf,1,100);
  if (error != HAL_OK)
    return error;

  HAL_Delay(CONVERSION_TIME);
  
  /* Receive a 2-byte result */
  error = HAL_I2C_Master_Receive(hi2c, HDC1080_ADDR<<1 | 0x01, buf, 2, 1000);
  if (error != HAL_OK)
    return error;
  
  /* Result */
  *val = buf[0]*256+buf[1]; 
  return HAL_OK;  /* Success */
}

/*
 * hdc1080_write_reg() - Write register
 * @hi2c:  handle to I2C interface
 * @reg: Register address
 * @val: 8-bit register value from the hdc1080
 * Returns HAL status or HAL_ERROR for invalid arguments.
 */
HAL_StatusTypeDef hdc1080_write_reg_config(I2C_HandleTypeDef *hi2c, uint16_t val)
{
  uint8_t buf[3];
  HAL_StatusTypeDef  error;

  buf[0] = HDC1080_CONFIG;
  buf[1] = (uint8_t)((val >> 8) & 0xff);  // msb
  buf[2] = (uint8_t)(val & 0xff);         // lsb
  /* Write the register */
  /* Send the command and data */
  error = HAL_I2C_Master_Transmit(hi2c,HDC1080_ADDR<<1,buf,3,100);
  if (error != HAL_OK)
    return error;
  else 
    return HAL_OK;  /* Success */
}
