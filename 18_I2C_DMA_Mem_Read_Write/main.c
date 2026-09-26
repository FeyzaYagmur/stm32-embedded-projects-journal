/**
 * @file    main.c
 * @brief   Application Layer: DMA Memory-Mapped Transfers via HAL_I2C_Mem_Write/Read_DMA
 * @author  Feyza Yagmur Arat
 * @note    Hardware inits (Clock, GPIO, DMA, I2C1) configured via CubeMX.
 */

#include "main.h"
#include "eeprom.h"

I2C_HandleTypeDef hi2c1;
DMA_HandleTypeDef hdma_i2c1_tx;
DMA_HandleTypeDef hdma_i2c1_rx;

/* Memory buffers for DMA channels */
uint8_t tx_data = 75; // Preloaded value (> 50)
uint8_t rx_data = 0;  // Telemetry destination buffer

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
  if (EEPROM_IsReady(&hi2c1) == 0)
  {
    /* Device missing: Enter visual fault alarm */
    while (1)
    {
      HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
      HAL_Delay(100);
    }
  }

  /* 2. Startup register write via DMA */
  EEPROM_Write_Mem_DMA(&hi2c1, REG_DATA_CELL, &tx_data, 1);

  while (1)
  {
    /* 3. Periodic register ingestion via DMA */
    EEPROM_Read_Mem_DMA(&hi2c1, REG_DATA_CELL, &rx_data, 1);

    /* Superloop pacing */
    HAL_Delay(250);
  }
}

/**
  * @brief  Memory Tx Transfer completed callback
  * @param  hi2c Pointer to I2C handle
  * @retval None
  */
void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
  if (hi2c->Instance == I2C1)
  {
    /* DMA memory write transfer successfully completed */
  }
}

/**
  * @brief  Memory Rx Transfer completed callback
  * @param  hi2c Pointer to I2C handle
  * @retval None
  */
void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
  if (hi2c->Instance == I2C1)
  {
    /* Telemetry data acquired into SRAM: Threshold verification */
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
