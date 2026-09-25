/**
 * @file    hcsr04_i2c.c
 * @brief   Asynchronous Interrupt-Driven (IT) HC-SR04 I2C Driver Implementation
 * @author  Feyza Yagmur Arat
 */

#include "hcsr04_i2c.h"

/**
  * @brief  Verifies physical sensor presence on the I2C bus
  * @param  hi2c Pointer to I2C handle
  * @retval 1: Device acknowledged, 0: Device unresponsive / bus fault
  */
uint8_t HCSR04_IsReady(I2C_HandleTypeDef *hi2c)
{
  if (HAL_I2C_IsDeviceReady(hi2c, HCSR04_ADDR, 1, 100) == HAL_OK)
  {
    return 1;
  }
  return 0;
}

/**
  * @brief  Reads distance register asynchronously via hardware interrupts (IT)
  * @param  hi2c    Pointer to I2C handle
  * @param  rx_data Pointer to destination buffer
  * @param  size    Expected payload byte count
  * @retval 1: Reception armed, 0: Bus busy or hardware error
  */
uint8_t HCSR04_Read_Distance_IT(I2C_HandleTypeDef *hi2c, uint8_t *rx_data, uint16_t size)
{
  if (HAL_I2C_Mem_Read_IT(hi2c, HCSR04_ADDR, REG_DISTANCE_DATA, I2C_MEMADD_SIZE_8BIT, rx_data, size) == HAL_OK)
  {
    return 1;
  }
  return 0;
}
