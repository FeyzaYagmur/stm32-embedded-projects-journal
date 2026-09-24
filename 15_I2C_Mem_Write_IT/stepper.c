/**
 * @file    stepper.c
 * @brief   Asynchronous Interrupt-Driven (IT) Stepper Motor Driver Implementation
 * @author  Feyza Yagmur Arat
 */

#include "stepper.h"

/**
  * @brief  Verifies physical driver presence on the I2C bus
  * @param  hi2c Pointer to I2C handle
  * @retval 1: Device acknowledged, 0: Device unresponsive / bus fault
  */
uint8_t Stepper_IsReady(I2C_HandleTypeDef *hi2c)
{
  if (HAL_I2C_IsDeviceReady(hi2c, STEPPER_ADDR, 1, 100) == HAL_OK)
  {
    return 1;
  }
  return 0;
}

/**
  * @brief  Writes to internal device register asynchronously via hardware interrupts (IT)
  * @param  hi2c     Pointer to I2C handle
  * @param  reg_addr Target internal register address
  * @param  p_data   Pointer to transmission buffer
  * @param  size     Payload byte count
  * @retval 1: Transmission armed, 0: Bus busy or hardware error
  */
uint8_t Stepper_Write_Reg_IT(I2C_HandleTypeDef *hi2c, uint16_t reg_addr, uint8_t *p_data, uint16_t size)
{
  if (HAL_I2C_Mem_Write_IT(hi2c, STEPPER_ADDR, reg_addr, I2C_MEMADD_SIZE_8BIT, p_data, size) == HAL_OK)
  {
    return 1;
  }
  return 0;
}
