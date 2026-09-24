/**
 * @file    main.c
 * @brief   Application Layer: Non-Blocking Telemetry Ingestion via HAL_I2C_Master_Receive_IT
 * @author  Feyza Yagmur Arat
 * @note    Hardware inits (Clock, GPIO, I2C1 with NVIC) configured via CubeMX.
 */

#include "main.h"
#include "ntc_sensor.h"

I2C_HandleTypeDef hi2c1;

/* Ingested temperature telemetry register */
uint8_t rx_temperature = 0;

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
  if (NTC_Sensor_IsReady(&hi2c1) == 0)
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
    /* 2. Arm non-blocking background reception pipeline */
    NTC_Sensor_Receive_IT(&hi2c1, &rx_temperature, 1);

    /* Superloop pace without blocking I2C transactions */
    HAL_Delay(500);
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
    /* Telemetry acquired: Evaluate threshold condition */
    if (rx_temperature > 50)
    {
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
    }
    else
    {
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
    }
  }
}
