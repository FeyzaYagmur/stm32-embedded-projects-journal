/**
 * @file    main.c
 * @brief   Application Layer: Non-Blocking Telemetry Acquisition via Chained SPI Interrupts
 * @author  Feyza Yagmur Arat
 * @note    Hardware inits (Clock, GPIO, SPI1 with NVIC) configured via CubeMX.
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

  /* Set CS line idle state to HIGH */
  HAL_GPIO_WritePin(CS_GPIO_PORT, CS_PIN, GPIO_PIN_SET);
  HAL_Delay(10);

  /* 1. Wake up device / exit shutdown mode via interrupt write */
  SPI_Driver_Write_Reg_IT(&hspi1, REG_SHUTDOWN, 0x01);
  HAL_Delay(10);

  while (1)
  {
    /* 2. Dispatch asynchronous chained read request */
    SPI_Driver_Read_Reg_IT(&hspi1, REG_DATA_X, &rx_telemetry);

    /* Evaluate acquired telemetry metric */
    if (rx_telemetry > 50)
    {
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
    }
    else
    {
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
    }

    /* Periodic loop cadence */
    HAL_Delay(250);
  }
}
