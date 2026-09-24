/**
 * @file    main.c
 * @brief   Application Layer: Register-Level Sensor Interfacing via I2C HAL
 * @author  Feyza Yagmur Arat
 * @note    Hardware inits (Clock, GPIO, I2C1) configured via CubeMX.
 */

#include "main.h"
#include "sensor.h"

I2C_HandleTypeDef hi2c1;

/* Operational Data Variables */
uint8_t sensor_config_cmd = 0x01; // Enable/wake-up command
uint8_t sensor_data_val   = 0;    // Ingested metric buffer

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
  if (Sensor_Is_Ready(&hi2c1) == 1)
  {
    /* 2. Configure target sensor register */
    Sensor_Write_Config(&hi2c1, sensor_config_cmd);
  }
  else
  {
    /* Hardware fault: Trap execution with visual alarm */
    while (1)
    {
      HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
      HAL_Delay(100);
    }
  }

  while (1)
  {
    /* 3. Ingest telemetry from output register */
    if (Sensor_Read_Data(&hi2c1, &sensor_data_val) == 1)
    {
      /* Safety/Threshold evaluation */
      if (sensor_data_val > 50)
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
