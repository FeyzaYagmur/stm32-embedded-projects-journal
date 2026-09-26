/**
 * @file    main.c
 * @brief   Application Layer: Synchronous SPI Telemetry Ingestion and Actuation
 * @author  Feyza Yagmur Arat
 * @note    Hardware inits (Clock, GPIO, SPI1) configured via CubeMX.
 */

#include "main.h"
#include "spi_driver.h"

SPI_HandleTypeDef hspi1;

/* Operational Data Variables */
uint8_t rx_telemetry = 0;

/* Private function prototypes */
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_SPI1_Init();

  /* Ensure CS line idle state is HIGH */
  HAL_GPIO_WritePin(CS_GPIO_PORT, CS_PIN, GPIO_PIN_SET);

  /* 1. Wake up device / exit shutdown mode */
  SPI_Driver_Write_Reg(&hspi1, REG_SHUTDOWN, 0x01);

  while (1)
  {
    /* 2. Query target register and ingest telemetry */
    if (SPI_Driver_Read_Reg(&hspi1, REG_DATA_X, &rx_telemetry) == 1)
    {
      /* Threshold validation */
      if (rx_telemetry > 50)
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
