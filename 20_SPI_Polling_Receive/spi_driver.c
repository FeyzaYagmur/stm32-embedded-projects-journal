/**
 * @file    spi_driver.c
 * @brief   Synchronous SPI Driver Implementation: Combined Transmit/Receive Operations
 * @author  Feyza Yagmur Arat
 */

#include "spi_driver.h"

/**
  * @brief  Writes a data byte to a target device register over SPI
  * @param  hspi     Pointer to SPI handle
  * @param  reg_addr Target internal register address
  * @param  data     Configuration byte to write
  * @retval 1: Transfer successful, 0: Transfer failed
  */
uint8_t SPI_Driver_Write_Reg(SPI_HandleTypeDef *hspi, uint8_t reg_addr, uint8_t data)
{
  uint8_t tx_frame[2];
  uint8_t status = 0;

  /* Frame payload: [0] = Register Offset, [1] = Target Value */
  tx_frame[0] = reg_addr;
  tx_frame[1] = data;

  /* Assert CS (Active-LOW) to begin transaction */
  HAL_GPIO_WritePin(CS_GPIO_PORT, CS_PIN, GPIO_PIN_RESET);

  /* Transmit 2-byte packet synchronously */
  if (HAL_SPI_Transmit(hspi, tx_frame, 2, 100) == HAL_OK)
  {
    status = 1;
  }

  /* De-assert CS (Active-HIGH) to latch data inside the device */
  HAL_GPIO_WritePin(CS_GPIO_PORT, CS_PIN, GPIO_PIN_SET);

  return status;
}

/**
  * @brief  Reads a data byte from a target device register over SPI
  * @param  hspi     Pointer to SPI handle
  * @param  reg_addr Target internal register address
  * @param  p_rx_buf Pointer to destination buffer in SRAM
  * @retval 1: Read successful, 0: Read failed
  */
uint8_t SPI_Driver_Read_Reg(SPI_HandleTypeDef *hspi, uint8_t reg_addr, uint8_t *p_rx_buf)
{
  uint8_t status = 0;

  /* Assert CS (Active-LOW) to begin transaction */
  HAL_GPIO_WritePin(CS_GPIO_PORT, CS_PIN, GPIO_PIN_RESET);

  /* 1. Transmit the target register offset */
  if (HAL_SPI_Transmit(hspi, &reg_addr, 1, 100) == HAL_OK)
  {
    /* 2. Clock in the response byte while CS remains asserted */
    if (HAL_SPI_Receive(hspi, p_rx_buf, 1, 100) == HAL_OK)
    {
      status = 1;
    }
  }

  /* De-assert CS (Active-HIGH) to terminate transaction safely */
  HAL_GPIO_WritePin(CS_GPIO_PORT, CS_PIN, GPIO_PIN_SET);

  return status;
}
