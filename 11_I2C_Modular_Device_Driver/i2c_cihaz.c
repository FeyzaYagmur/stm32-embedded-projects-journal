/**
 * @file    i2c_cihaz.c
 * @brief   Modular I2C Device Driver Source Implementation
 * @author  Feyza Yagmur Arat
 */

#include "i2c_cihaz.h"

/**
  * @brief  Checks whether target I2C device is active on the bus
  * @param  hi2c   Pointer to I2C handle
  * @param  adress 8-bit shifted device address
  * @retval 1: Device ready, 0: Timeout/Not detected
  */
uint8_t Cihaz_Hazir_Mi(I2C_HandleTypeDef *hi2c, uint8_t adress)
{
  if (HAL_I2C_IsDeviceReady(hi2c, adress, 1, 100) == HAL_OK)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

/**
  * @brief  Transmits a single byte to the target device
  * @param  hi2c        Pointer to I2C handle
  * @param  adress      8-bit shifted device address
  * @param  gonder_veri Data byte to transmit
  * @retval 1: Success, 0: Error
  */
uint8_t Cihaz_Yaz(I2C_HandleTypeDef *hi2c, uint8_t adress, uint8_t gonder_veri)
{
  if (HAL_I2C_Master_Transmit(hi2c, adress, &gonder_veri, 1, 100) == HAL_OK)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

/**
  * @brief  Reads a single byte from the target device
  * @param  hi2c       Pointer to I2C handle
  * @param  adress     8-bit shifted device address
  * @param  gelen_veri Pointer to destination buffer
  * @retval 1: Success, 0: Error
  */
uint8_t Cihaz_Oku(I2C_HandleTypeDef *hi2c, uint8_t adress, uint8_t *gelen_veri)
{
  if (HAL_I2C_Master_Receive(hi2c, adress, gelen_veri, 1, 100) == HAL_OK)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}
