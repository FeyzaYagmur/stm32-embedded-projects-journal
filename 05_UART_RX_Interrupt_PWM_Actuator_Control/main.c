/**
 * @file    main.c
 * @brief   Core Logic: Interrupt-Driven UART Command Parsing & PWM Actuator Control
 * @author  Feyza Yagmur Arat
 * @note    Hardware inits (Clock, GPIO, TIM4, USART2) configured via CubeMX.
 */

#include "main.h"
#include <string.h>

extern TIM_HandleTypeDef htim4;
extern UART_HandleTypeDef huart2;

/* Single-byte buffer for incoming asynchronous character */
uint8_t rx_data = 0;

/* Status acknowledgement strings */
const uint8_t ack_a[]   = "[ACK] Position A (25% Duty) Configured.\r\n";
const uint8_t ack_b[]   = "[ACK] Position B (75% Duty) Configured.\r\n";
const uint8_t ack_s[]   = "[ACK] System Reset to Default Position.\r\n";
const uint8_t err_msg[] = "[ERROR] Unknown Command Received!\r\n";

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_TIM4_Init();
  MX_USART2_UART_Init();

  /* 1. Start PWM signal generation with initial baseline compare value */
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
  __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 1000);

  /* 2. Arm the UART RX interrupt for single-byte capture */
  HAL_UART_Receive_IT(&huart2, &rx_data, 1);

  while (1)
  {
    /* Visual heartbeat: executes independently without serial interference */
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
    HAL_Delay(500);
  }
}

/**
  * @brief  Rx Transfer completed callback
  * @param  huart UART handle
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART2)
  {
    /* 1. Parse received byte and actuate hardware */
    switch (rx_data)
    {
      case 'A':
      case 'a':
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 1250);
        HAL_UART_Transmit_IT(&huart2, (uint8_t *)ack_a, sizeof(ack_a) - 1);
        break;

      case 'B':
      case 'b':
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 1750);
        HAL_UART_Transmit_IT(&huart2, (uint8_t *)ack_b, sizeof(ack_b) - 1);
        break;

      case 'S':
      case 's':
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 1000);
        HAL_UART_Transmit_IT(&huart2, (uint8_t *)ack_s, sizeof(ack_s) - 1);
        break;

      case '\r':
      case '\n':
        /* Discard line-ending control characters */
        break;

      default:
        HAL_UART_Transmit_IT(&huart2, (uint8_t *)err_msg, sizeof(err_msg) - 1);
        break;
    }

    /* 2. CRITICAL: Re-arm UART RX interrupt to capture the next byte */
    HAL_UART_Receive_IT(&huart2, &rx_data, 1);
  }
}
