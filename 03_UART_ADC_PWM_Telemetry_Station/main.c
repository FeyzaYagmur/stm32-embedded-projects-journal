/**
 * @file    main.c
 * @brief   Core Logic: ADC Sampling, Hardware PWM Modulation & UART Telemetry
 * @author  Feyza Yagmur Arat
 * @note    Hardware inits (Clock, GPIO, ADC1, TIM4, USART2) configured via CubeMX.
 */

#include "main.h"
#include <stdio.h>
#include <string.h>

extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim4;
extern UART_HandleTypeDef huart2;

/* Sensor and Actuator Variables */
uint16_t adc_raw = 0;
float voltage = 0.0f;
uint16_t pwm_duty = 0;
char uart_tx_buffer[80];

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_TIM4_Init();
  MX_USART2_UART_Init();

  /* Start PWM signal generation on TIM4 Channel 1 */
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);

  while (1)
  {
    /* 1. ACQUIRE ANALOG SAMPLE (12-bit Resolution: 0 - 4095) */
    HAL_ADC_Start(&hadc1);
    if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK)
    {
      adc_raw = HAL_ADC_GetValue(&hadc1);
    }
    HAL_ADC_Stop(&hadc1);

    /* 2. SCALE VOLTAGE (0.0V - 3.3V reference) */
    voltage = ((float)adc_raw * 3.3f) / 4095.0f;

    /* 3. MAP ADC VALUE TO PWM AUTO-RELOAD (ARR = 1000) */
    pwm_duty = (uint16_t)(((uint32_t)adc_raw * 1000) / 4095);
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, pwm_duty);

    /* 4. CONSTRUCT TELEMETRY PACKET WITH MEMSET & SPRINTF */
    memset(uart_tx_buffer, 0, sizeof(uart_tx_buffer));
    sprintf(uart_tx_buffer, "[TELEMETRY] ADC: %4u | Voltage: %.2f V | PWM: %4u\r\n", 
            adc_raw, voltage, pwm_duty);

    /* 5. TRANSMIT PACKET OVER UART USING STRLEN */
    HAL_UART_Transmit(&huart2, (uint8_t *)uart_tx_buffer, strlen(uart_tx_buffer), 100);

    /* 1-second sampling and telemetry interval */
    HAL_Delay(1000);
  }
}
