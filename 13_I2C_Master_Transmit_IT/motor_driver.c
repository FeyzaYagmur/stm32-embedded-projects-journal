/**
 * @file    motor_driver.c
 * @brief   I2C Driver Implementation via HAL_I2C_Master_Transmit_IT
 * @author  Feyza Yagmur Arat
 */

#include "motor_driver.h"

/**
  * @brief  Verifies physical target presence on the I2C bus
  * @param  hi2c Pointer to I2C handle
  * @retval 1: Device acknowledged, 0: Device unresponsive / bus fault
  */
uint8_t Motor_Driver_IsReady(I2C_HandleTypeDef *hi2c)
{
  if (HAL_I2C_IsDeviceReady(hi2c, MOTOR_DRIVER_ADDR, 1, 100) == HAL_OK)
  {
    return 1;
  }
  return 0;
}

/**
  * @brief  Dispatches command frame via HAL_I2C_Master_Transmit_IT
  * @param  hi2c   Pointer to I2C handle
  * @param  p_data Pointer to transmission buffer
  * @param  size   Payload byte count
  * @retval 1: Transmission initiated, 0: Bus busy or hardware error
  */
uint8_t Motor_Driver_Transmit_IT(I2C_HandleTypeDef *hi2c, uint8_t *p_data, uint16_t size)
{
  if (HAL_I2C_Master_Transmit_IT(hi2c, MOTOR_DRIVER_ADDR, p_data, size) == HAL_OK)
  {
    return 1;
  }
  return 0;
}
