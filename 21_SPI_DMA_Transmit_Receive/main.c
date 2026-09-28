/**
 * @file    main.c
 * @brief   Application Layer: Hardware-Accelerated Chained SPI DMA Operations
 * @author  Feyza Yagmur Arat
 * @note    Hardware inits (Clock, GPIO, DMA, SPI1) configured via CubeMX.
 */

#include "main.h"
#include "spi_driver.h"

SPI_HandleTypeDef hspi1;
DMA_HandleTypeDef hdma_spi1_tx;
DMA_HandleTypeDef hdma_spi1_rx;

/* DMA Target Buffer */
uint8_t rx_telemetry = 0;

/* Private function prototypes */
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_SPI1_Init(void);

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_SPI1_Init();

  /* Set CS line idle state to HIGH */
  HAL_GPIO_WritePin(CS_GPIO_PORT, CS_PIN, GPIO_PIN_SET);
  HAL_Delay(10);

  /* 1. Wake up device / exit shutdown mode via DMA */
  SPI_Driver_Write_Reg_DMA(&hspi1, REG_SHUTDOWN, 0x01);
  HAL_Delay(10);

  while (1)
  {
    /* 2. Dispatch non-blocking chained DMA read */
    SPI_Driver_Read_Reg_DMA(&hspi1, REG_DATA_X, &rx_telemetry);

    /* Superloop pacing */
    HAL_Delay(250);
  }
}

/**
  * @brief  SPI DMA Rx Transfer completed callback: Latches CS and evaluates data
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

    /* Telemetry acquired into SRAM: Threshold verification */
    if (rx_telemetry > 50)
    {
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
    }
    else
    {
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
    }
  }
}
