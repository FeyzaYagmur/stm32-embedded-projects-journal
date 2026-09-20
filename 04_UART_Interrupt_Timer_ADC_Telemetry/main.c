/**
 * @file    main.c
 * @brief   Core Logic: Interrupt-Driven ADC Sampling & Non-Blocking UART Telemetry
 * @author  Feyza Yagmur Arat
 * @note    Hardware inits (Clock, GPIO, ADC1, TIM2, USART2) configured via CubeMX.
 */

#include "main.h"
#include <stdio.h>
#include <string.h>

extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim2;
extern UART_HandleTypeDef huart2;

/* Telemetry and Acquisition Variables */
uint16_t adc_raw = 0;
float voltage = 0.0f;
char tx_buffer[64];

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_TIM2_Init();
  MX_USART2_UART_Init();

  /* Start periodic 1-second timebase interrupt */
  HAL_TIM_Base_Start_IT(&htim2);

  while (1)
  {
    /*
     * Empty background loop:
     * Entire architecture is non-blocking and interrupt-driven.
     */
  }
}

/**
  * @brief  Period elapsed callback in non-blocking mode (TIM2 1-second tick)
  * @param  htim TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM2)
  {
    /* 1. Acquire analog conversion */
    HAL_ADC_Start(&hadc1);
    if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK)
    {
      adc_raw = HAL_ADC_GetValue(&hadc1);
    }
    HAL_ADC_Stop(&hadc1);

    /* 2. Compute voltage (0.0V - 3.3V) */
    voltage = ((float)adc_raw * 3.3f) / 4095.0f;

    /* 3. Format telemetry string */
    sprintf(tx_buffer, "Voltage: %.2f V | ADC: %u\r\n", voltage, adc_raw);

    /* 4. Transmit packet via UART interrupt (non-blocking) */
    HAL_UART_Transmit_IT(&huart2, (uint8_t *)tx_buffer, strlen(tx_buffer));
  }
}

/**
  * @brief  Tx Transfer completed callback
  * @param  huart UART handle
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART2)
  {
    /* Packet delivery confirmed: toggle status LED */
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
  }
}
