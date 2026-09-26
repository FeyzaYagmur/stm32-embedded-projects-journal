/**
 * @file    io_expander.c
 * @brief   Hardware-Accelerated I2C DMA IO Expander Driver Implementation
 * @author  Feyza Yagmur Arat
 */

#include "io_expander.h"

/**
  * @brief  Verifies physical peripheral presence on the I2C bus
  * @param  hi2c Pointer to I2C handle
  * @retval 1: Device acknowledged, 0: Device unresponsive / bus fault
  */
uint8_t IO_Expander_IsReady(I2C_HandleTypeDef *hi2c)
{
  if (HAL_I2C_IsDeviceReady(hi2c, IO_EXPANDER_ADDR, 1, 100) == HAL_OK)
  {
    return 1;
  }
  return 0;
}

/**
  * @brief  Dispatches frame transmission directly from memory via DMA
  * @param  hi2c     Pointer to I2C handle
  * @param  p_tx_buf Pointer to source buffer in SRAM
  * @param  size     Payload byte count
  * @retval 1: DMA stream armed, 0: Hardware error or bus busy
  */
uint8_t IO_Expander_Transmit_DMA(I2C_HandleTypeDef *hi2c, uint8_t *p_tx_buf, uint16_t size)
{
  if (HAL_I2C_Master_Transmit_DMA(hi2c, IO_EXPANDER_ADDR, p_tx_buf, size) == HAL_OK)
  {
    return 1;
  }
  return 0;
}

/**
  * @brief  Initiates asynchronous frame reception directly into memory via DMA
  * @param  hi2c     Pointer to I2C handle
  * @param  p_rx_buf Pointer to destination buffer in SRAM
  * @param  size     Expected payload byte count
  * @retval 1: DMA stream armed, 0: Hardware error or bus busy
  */
uint8_t IO_Expander_Receive_DMA(I2C_HandleTypeDef *hi2c, uint8_t *p_rx_buf, uint16_t size)
{
  if (HAL_I2C_Master_Receive_DMA(hi2c, IO_EXPANDER_ADDR, p_rx_buf, size) == HAL_OK)
  {
    return 1;
  }
  return 0;
}
