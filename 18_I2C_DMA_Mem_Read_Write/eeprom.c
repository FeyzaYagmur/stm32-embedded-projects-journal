/**
 * @file    eeprom.c
 * @brief   Hardware-Accelerated I2C DMA EEPROM Driver Implementation
 * @author  Feyza Yagmur Arat
 */

#include "eeprom.h"

/**
  * @brief  Verifies physical memory device presence on the I2C bus
  * @param  hi2c Pointer to I2C handle
  * @retval 1: Device acknowledged, 0: Device unresponsive / bus fault
  */
uint8_t EEPROM_IsReady(I2C_HandleTypeDef *hi2c)
{
  if (HAL_I2C_IsDeviceReady(hi2c, EEPROM_ADDR, 1, 100) == HAL_OK)
  {
    return 1;
  }
  return 0;
}

/**
  * @brief  Writes to internal memory register via DMA stream
  * @param  hi2c     Pointer to I2C handle
  * @param  reg_addr Target internal register address
  * @param  p_tx_buf Pointer to source buffer in SRAM
  * @param  size     Payload byte count
  * @retval 1: DMA stream armed, 0: Hardware error or bus busy
  */
uint8_t EEPROM_Write_Mem_DMA(I2C_HandleTypeDef *hi2c, uint16_t reg_addr, uint8_t *p_tx_buf, uint16_t size)
{
  if (HAL_I2C_Mem_Write_DMA(hi2c, EEPROM_ADDR, reg_addr, I2C_MEMADD_SIZE_8BIT, p_tx_buf, size) == HAL_OK)
  {
    return 1;
  }
  return 0;
}

/**
  * @brief  Reads from internal memory register directly into SRAM via DMA
  * @param  hi2c     Pointer to I2C handle
  * @param  reg_addr Target internal register address
  * @param  p_rx_buf Pointer to destination buffer in SRAM
  * @param  size     Expected payload byte count
  * @retval 1: DMA stream armed, 0: Hardware error or bus busy
  */
uint8_t EEPROM_Read_Mem_DMA(I2C_HandleTypeDef *hi2c, uint16_t reg_addr, uint8_t *p_rx_buf, uint16_t size)
{
  if (HAL_I2C_Mem_Read_DMA(hi2c, EEPROM_ADDR, reg_addr, I2C_MEMADD_SIZE_8BIT, p_rx_buf, size) == HAL_OK)
  {
    return 1;
  }
  return 0;
}
