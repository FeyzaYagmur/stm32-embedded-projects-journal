/**
 * @file    main.c
 * @brief   Application Layer: Non-Blocking Telemetry Ingestion via HAL_I2C_Mem_Read_IT
 * @author  Feyza Yagmur Arat
 * @note    Hardware inits (Clock, GPIO, I2C1 with NVIC) configured via CubeMX.
 */

#include "main.h"
#include "hcsr04_i2c.h"

I2C_HandleTypeDef hi2c1;

/* Telemetry register buffer */
uint8_t measured_distance = 0;

/* Private function prototypes */
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_I2C1_Init();

  /* 1. Device Presence Handshake */
  if (HCSR04_IsReady(&hi2c1) == 0)
  {
    /* Device missing: Enter visual fault alarm */
    while (1)
    {
      HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
      HAL_Delay(100);
    }
  }

  while (1)
  {
    /* 2. Dispatch non-blocking register read request */
    HCSR04_Read_Distance_IT(&hi2c1, &measured_distance, 1);

    /* Superloop pace without blocking I2C operations */
    HAL_Delay(200);
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
    /* Distance data updated: Proximity alert threshold check (< 20 cm) */
    if (measured_distance < 20)
    {
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
    }
    else
    {
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
    }
  }
}
