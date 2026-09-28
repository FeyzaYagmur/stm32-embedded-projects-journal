/**
 * @file    spi_driver.c
 * @brief   Asynchronous Interrupt-Driven (IT) SPI Driver Implementation
 * @author  Feyza Yagmur Arat
 */

#include "spi_driver.h"
#include 

/* Static memory buffers preventing stack corruption during background interrupt operations */
static uint8_t tx_frame[2];
static uint8_t reg_query;
uint8_t *p_target_rx_buf = NULL;

/**
  * @brief  Initiates asynchronous register configuration over SPI via interrupts
  * @param  hspi     Pointer to SPI handle
  * @param  reg_addr Target internal register address
  * @param  data     Configuration byte to write
  * @retval None
  */
void SPI_Driver_Write_Reg_IT(SPI_HandleTypeDef *hspi, uint8_t reg_addr, uint8_t data)
{
  tx_frame[0] = reg_addr;
  tx_frame[1] = data;

  /* Assert CS (Active-LOW) to initiate transaction */
  HAL_GPIO_WritePin(CS_GPIO_PORT, CS_PIN, GPIO_PIN_RESET);

  /* Transmit 2-byte command frame in background */
  HAL_SPI_Transmit_IT(hspi, tx_frame, 2);
}

/**
  * @brief  Initiates asynchronous chained register read sequence over SPI
  * @param  hspi       Pointer to SPI handle
  * @param  reg_addr   Target internal register address to query
  * @param  p_rx_buf   Pointer to destination buffer in SRAM
  * @retval None
  */
void SPI_Driver_Read_Reg_IT(SPI_HandleTypeDef *hspi, uint8_t reg_addr, uint8_t *p_rx_buf)
{
  reg_query = reg_addr;
  p_target_rx_buf = p_rx_buf;

  /* Assert CS (Active-LOW) to initiate transaction */
  HAL_GPIO_WritePin(CS_GPIO_PORT, CS_PIN, GPIO_PIN_RESET);

  /* Transmit target register address via hardware interrupt */
  HAL_SPI_Transmit_IT(hspi, &reg_query, 1);
}

/**
  * @brief  SPI Tx Transfer completed callback: Chains RX or latches CS
  * @param  hspi Pointer to SPI handle
  * @retval None
  */
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
  if (hspi->Instance == SPI1)
  {
    /* If this transmission was phase 1 of a read, arm the chained receive */
    if (p_target_rx_buf != NULL)
    {
      HAL_SPI_Receive_IT(hspi, p_target_rx_buf, 1);
    }
    else
    {
      /* Write operation complete: De-assert CS (Active-HIGH) */
      HAL_GPIO_WritePin(CS_GPIO_PORT, CS_PIN, GPIO_PIN_SET);
    }
  }
}
