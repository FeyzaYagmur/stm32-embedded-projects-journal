/**
 * @file    main.c
 * @brief   Application Layer: Non-Blocking Transmission via HAL_I2C_Master_Transmit_IT
 * @author  Feyza Yagmur Arat
 * @note    Hardware inits (Clock, GPIO, I2C1 with NVIC) configured via CubeMX.
 */

#include "main.h"
#include "motor_driver.h"

I2C_HandleTypeDef hi2c1;

/* Actuator command frame: [0] = Direction (0x01: Forward), [1] = Duty (0xFF: Max Speed) */
uint8_t motor_payload[2] = {0x01, 0xFF};

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
  if (Motor_Driver_IsReady(&hi2c1) == 1)
  {
    /* 2. Dispatch frame using HAL_I2C_Master_Transmit_IT */
    Motor_Driver_Transmit_IT(&hi2c1, motor_payload, sizeof(motor_payload));
  }
  else
  {
    /* Hardware missing: Enter visual fault alarm */
    while (1)
    {
      HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
      HAL_Delay(100);
    }
  }

  while (1)
  {
    /* Main execution loop remains completely unblocked */
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
    /* Transmission successfully completed across the physical bus */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
  }
}
