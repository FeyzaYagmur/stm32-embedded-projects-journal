/**
 * @file    main.c
 * @brief   Application Layer: Periodic Telemetry Ingestion via Modular I2C Driver
 * @author  Feyza Yagmur Arat
 * @note    Hardware inits (Clock, GPIO, I2C1) configured via CubeMX.
 */

#include "main.h"
#include "i2c_cihaz.h"

I2C_HandleTypeDef hi2c1;

/* Operational Data Variables */
uint8_t tx_data = 0xAA;
uint8_t rx_data = 0;

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

  /* 1. Verify device readiness on the physical bus */
  if (Cihaz_Hazir_Mi(&hi2c1, ADRESS) == 1)
  {
    /* Device acknowledged: write startup payload */
    Cihaz_Yaz(&hi2c1, ADRESS, tx_data);
  }
  else
  {
    /* Device missing: enter error fault state */
    while (1)
    {
      HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
      HAL_Delay(100);
    }
  }

  while (1)
  {
    /* 2. Periodic polling read */
    if (Cihaz_Oku(&hi2c1, ADRESS, &rx_data) == 1)
    {
      /* Threshold validation */
      if (rx_data > 50)
      {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
      }
      else
      {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
      }
    }

    HAL_Delay(200);
  }
}
