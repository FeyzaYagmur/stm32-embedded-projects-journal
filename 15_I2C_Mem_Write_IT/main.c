/**
 * @file    main.c
 * @brief   Application Layer: Non-Blocking Register Access via HAL_I2C_Mem_Write_IT
 * @author  Feyza Yagmur Arat
 * @note    Hardware inits (Clock, GPIO, I2C1 with NVIC) configured via CubeMX.
 */

#include "main.h"
#include "stepper.h"

I2C_HandleTypeDef hi2c1;

/* Target step count payload */
uint8_t step_count_data = 200;

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
  if (Stepper_IsReady(&hi2c1) == 0)
  {
    /* Device missing: Enter visual fault alarm */
    while (1)
    {
      HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
      HAL_Delay(100);
    }
  }

  /* 2. Dispatch non-blocking register configuration */
  Stepper_Write_Reg_IT(&hi2c1, REG_STEP_COUNT, &step_count_data, 1);

  while (1)
  {
    /* CPU remains completely unblocked while register write completes in background */
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
    /* Memory write successfully completed across physical bus */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
  }
}
