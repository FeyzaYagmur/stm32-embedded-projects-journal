/**
 * @file    spi_driver.c
 * @brief   Asynchronous Interrupt-Driven (IT) SPI Driver Implementation
 * @author  Feyza Yagmur Arat
 */

#include "spi_driver.h"
#include 

/* Static buffers to prevent stack eviction during background interrupt execution */
static uint8_t tx_frame[2];
static uint8_t reg_query;
static uint8_t *p_target_rx_buf = NULL;

/**
  * @brief  Initiates asynchronous register configuration over SPI
  * @param  hspi     Pointer to SPI handle
  * @param  reg_addr Target internal register address
  * @param  data     Configuration byte to write
  * @retval None
  */
void SPI_Driver_Write_Reg_IT(SPI_HandleTypeDef *hspi, uint8_t reg_addr, uint8_t data)
{
  tx_frame[0] = reg_addr;
  tx_frame[1] = data;

  /* Assert CS (Active-LOW) to initiate communication */
  HAL_GPIO_WritePin(CS_GPIO_PORT, CS_PIN, GPIO_PIN_RESET);

  /* Transmit 2-byte frame via hardware interrupt */
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

  /* Transmit requested register address via hardware interrupt */
  HAL_SPI_Transmit_IT(hspi, &reg_query, 1);
}

/**
  * @brief  Tx Transfer completed callback: Handles CS latch or triggers chained Rx
  * @param  hspi Pointer to SPI handle
  * @retval None
  */
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
  if (hspi->Instance == SPI1)
  {
    /* If this transmission was phase 1 of a read request, chain the receive operation */
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

/**
  * @brief  Rx Transfer completed callback: Finalizes read transaction and closes CS
  * @param  hspi Pointer to SPI handle
  * @retval None
  */
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
  if (hspi->Instance == SPI1)
  {
    /* De-assert CS (Active-HIGH) to terminate transaction safely */
    HAL_GPIO_WritePin(CS_GPIO_PORT, CS_PIN, GPIO_PIN_SET);
    p_target_rx_buf = NULL;
  }
}
