/**
 * @file    spi_driver.c
 * @brief   Hardware-Accelerated DMA SPI Driver Implementation
 * @author  Feyza Yagmur Arat
 */

#include "spi_driver.h"
#include 

/* Static memory buffers to preserve data integrity during DMA operations */
static uint8_t tx_frame[2];
static uint8_t reg_query;
uint8_t *p_target_rx_buf = NULL;

/**
  * @brief  Initiates asynchronous register configuration over SPI via DMA
  * @param  hspi     Pointer to SPI handle
  * @param  reg_addr Target internal register address
  * @param  data     Configuration byte to write
  * @retval None
  */
void SPI_Driver_Write_Reg_DMA(SPI_HandleTypeDef *hspi, uint8_t reg_addr, uint8_t data)
{
  tx_frame[0] = reg_addr;
  tx_frame[1] = data;

  /* Assert CS (Active-LOW) to initiate communication */
  HAL_GPIO_WritePin(CS_GPIO_PORT, CS_PIN, GPIO_PIN_RESET);

  /* Start asynchronous DMA transmission */
  HAL_SPI_Transmit_DMA(hspi, tx_frame, 2);
}

/**
  * @brief  Initiates asynchronous chained register read sequence over SPI via DMA
  * @param  hspi       Pointer to SPI handle
  * @param  reg_addr   Target internal register address to query
  * @param  p_rx_buf   Pointer to destination buffer in SRAM
  * @retval None
  */
void SPI_Driver_Read_Reg_DMA(SPI_HandleTypeDef *hspi, uint8_t reg_addr, uint8_t *p_rx_buf)
{
  reg_query = reg_addr;
  p_target_rx_buf = p_rx_buf;

  /* Assert CS (Active-LOW) to initiate transaction */
  HAL_GPIO_WritePin(CS_GPIO_PORT, CS_PIN, GPIO_PIN_RESET);

  /* Transmit target register address via DMA */
  HAL_SPI_Transmit_DMA(hspi, &reg_query, 1);
}

/**
  * @brief  SPI DMA Tx Transfer completed callback: Chains RX or latches CS
  * @param  hspi Pointer to SPI handle
  * @retval None
  */
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
  if (hspi->Instance == SPI1)
  {
    /* If this transmission is the setup phase of a read, arm the RX DMA */
    if (p_target_rx_buf != NULL)
    {
      HAL_SPI_Receive_DMA(hspi, p_target_rx_buf, 1);
    }
    else
    {
      /* Write operation complete: De-assert CS (Active-HIGH) */
      HAL_GPIO_WritePin(CS_GPIO_PORT, CS_PIN, GPIO_PIN_SET);
    }
  }
}
