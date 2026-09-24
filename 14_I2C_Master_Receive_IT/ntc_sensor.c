/**
 * @file    ntc_sensor.c
 * @brief   Asynchronous Interrupt-Driven (IT) NTC Sensor Driver Implementation
 * @author  Feyza Yagmur Arat
 */

#include "ntc_sensor.h"

/**
  * @brief  Verifies physical sensor presence on the I2C bus
  * @param  hi2c Pointer to I2C handle
  * @retval 1: Device acknowledged, 0: Device unresponsive / bus fault
  */
uint8_t NTC_Sensor_IsReady(I2C_HandleTypeDef *hi2c)
{
  if (HAL_I2C_IsDeviceReady(hi2c, NTC_SENSOR_ADDR, 1, 100) == HAL_OK)
  {
    return 1;
  }
  return 0;
}

/**
  * @brief  Initiates asynchronous frame reception via hardware interrupts (IT)
  * @param  hi2c   Pointer to I2C handle
  * @param  p_data Pointer to destination buffer
  * @param  size   Expected payload byte count
  * @retval 1: Reception pipeline armed, 0: Bus busy or hardware error
  */
uint8_t NTC_Sensor_Receive_IT(I2C_HandleTypeDef *hi2c, uint8_t *p_data, uint16_t size)
{
  if (HAL_I2C_Master_Receive_IT(hi2c, NTC_SENSOR_ADDR, p_data, size) == HAL_OK)
  {
    return 1;
  }
  return 0;
}
