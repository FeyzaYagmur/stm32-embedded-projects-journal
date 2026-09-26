/**
 * @file    main.c
 * @brief   Application Layer: I2C Master Transmit & Receive via DMA Streams
 * @author  Feyza Yagmur Arat
 * @note    Hardware inits (Clock, GPIO, DMA, I2C1) configured via CubeMX.
 */

#include "main.h"
#include "io_expander.h"

I2C_HandleTypeDef hi2c1;
DMA_HandleTypeDef hdma_i2c1_tx;
DMA_HandleTypeDef hdma_i2c1_rx;

/* DMA Transaction Buffers */
uint8_t tx_data = 0xAA;
uint8_t rx_data = 0;

/* Private function prototypes */
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_I2C1_Init(void);

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_I2C1_Init();

  /* 1. Device Presence Handshake */
  if (IO_Expander_IsReady(&hi2c1) == 0)
  {
    /* Device missing: Enter visual fault alarm */
    while (1)
    {
      HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
      HAL_Delay(100);
    }
  }

  /* 2. Startup initialization write via DMA */
  IO_Expander_Transmit_DMA(&hi2c1, &tx_data, 1);

  while (1)
  {
    /* 3. Periodic telemetry acquisition request via DMA */
    IO_Expander_Receive_DMA(&hi2c1, &rx_data, 1);

    /* Superloop pacing */
    HAL_Delay(250);
  }
}

/**
  * @brief  Master Tx Transfer completed callback
  * @param  hi2c Pointer to I2C handle
  * @retval None
  */
void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
  if (hi2c->Instance == I2C1)
  {
    /* DMA transmission stream finished */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
  }
}

/**
  * @brief  Master Rx Transfer completed callback
  * @param  hi2c Pointer to I2C handle
  * @retval None
  */
void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
  if (hi2c->Instance == I2C1)
  {
    /* DMA reception completed: Evaluate threshold condition */
    if (rx_data > 50)
    {
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
    }
    else
    {
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
    }
  }
}
