/**
 * @file    main.c
 * @brief   Core Logic: UART State Machine & Idle Timeout Telemetry
 * @note    Hardware inits (SystemClock, GPIO, USART2) generated via CubeMX.
 */

#include "main.h"

extern UART_HandleTypeDef huart2;

/* Telemetry Strings */
const uint8_t msg_system_ready[] = "--- System Initialized (UART Polling Active) ---\r\n";
const uint8_t msg_led_on[]       = "[STATE] Button Pressed -> LED ON!\r\n";
const uint8_t msg_led_off[]      = "[STATE] Button Pressed -> LED OFF!\r\n";
const uint8_t msg_idle_timeout[] = "[INFO] System Idle, awaiting button input...\r\n";

/* State Variables */
uint8_t led_state = 0;           // 0: OFF, 1: ON
uint32_t last_activity_tick = 0; // Timestamp tracking for idle timeout

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_USART2_UART_Init();

  /* Send system ready banner */
  HAL_UART_Transmit(&huart2, (uint8_t *)msg_system_ready, sizeof(msg_system_ready) - 1, HAL_MAX_DELAY);
  
  last_activity_tick = HAL_GetTick();

  while (1)
  {
    /* 1. BUTTON INPUT MONITORING (PC13 - Active-Low with Debounce) */
    if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET) 
    {
      HAL_Delay(50); // Debounce filter

      if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET)
      {
        if (led_state == 0)
        {
          HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
          HAL_UART_Transmit(&huart2, (uint8_t *)msg_led_on, sizeof(msg_led_on) - 1, 100);
          led_state = 1;
        }
        else
        {
          HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
          HAL_UART_Transmit(&huart2, (uint8_t *)msg_led_off, sizeof(msg_led_off) - 1, 100);
          led_state = 0;
        }

        /* Wait until button is released */
        while (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET);

        last_activity_tick = HAL_GetTick();
      }
    }

    /* 2. NON-BLOCKING IDLE TIMEOUT (3000 ms) */
    if (HAL_GetTick() - last_activity_tick >= 3000)
    {
      HAL_UART_Transmit(&huart2, (uint8_t *)msg_idle_timeout, sizeof(msg_idle_timeout) - 1, 100);
      last_activity_tick = HAL_GetTick();
    }
  }
}
