/**
 * @file    main.c
 * @brief   Application Layer: Synchronous SPI Register Configuration
 * @author  Feyza Yagmur Arat
 * @note    Hardware inits (Clock, GPIO, SPI1) configured via CubeMX.
 */

#include "main.h"
#include "spi_driver.h"

SPI_HandleTypeDef hspi1;

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

  /* 2. Configure target data register */
  SPI_Driver_Write_Reg(&hspi1, REG_DATA_X, 0x55);

  while (1)
  {
    /* Superloop execution thread */
  }
}
