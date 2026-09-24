/**
 * @file    sensor.c
 * @brief   Register-Level I2C Sensor Driver Implementation
 * @author  Feyza Yagmur Arat
 */

#include "sensor.h"

/**
  * @brief  Checks whether the sensor is present and responding on the bus
  * @param  hi2c Pointer to I2C handle
  * @retval 1: Device ready, 0: Error / Not detected
  */
uint8_t Sensor_Is_Ready(I2C_HandleTypeDef *hi2c)
{
  if (HAL_I2C_IsDeviceReady(hi2c, SENSOR_ADDR, 1, 100) == HAL_OK)
  {
    return 1;
  }
  return 0;
}

/**
  * @brief  Writes configuration parameter to the sensor control register
  * @param  hi2c       Pointer to I2C handle
  * @param  config_val Configuration byte to write into REG_CONFIG
  * @retval 1: Write success, 0: Write failure
  */
uint8_t Sensor_Write_Config(I2C_HandleTypeDef *hi2c, uint8_t config_val)
{
  if (HAL_I2C_Mem_Write(hi2c, SENSOR_ADDR, REG_CONFIG, I2C_MEMADD_SIZE_8BIT, &config_val, 1, 100) == HAL_OK)
  {
    return 1;
  }
  return 0;
}

/**
  * @brief  Reads telemetry value from the sensor data register
  * @param  hi2c     Pointer to I2C handle
  * @param  rx_data  Pointer to destination buffer
  * @retval 1: Read success, 0: Read failure
  */
uint8_t Sensor_Read_Data(I2C_HandleTypeDef *hi2c, uint8_t *rx_data)
{
  if (HAL_I2C_Mem_Read(hi2c, SENSOR_ADDR, REG_DATA_OUT, I2C_MEMADD_SIZE_8BIT, rx_data, 1, 100) == HAL_OK)
  {
    return 1;
  }
  return 0;
}
