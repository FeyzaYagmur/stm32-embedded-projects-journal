/**
 * @file    main.c
 * @brief   Core Logic: UART Command Parser with Non-Blocking Timeout Management
 * @author  Feyza Yagmur Arat
 * @note    Hardware inits (Clock, GPIO, USART2) configured via CubeMX.
 */

#include "main.h"

extern UART_HandleTypeDef huart2;

/* Terminal Prompts and Status Feedback */
const uint8_t msg_menu[]        = "\r\n=== CONTROL MENU ===\r\n[1] LED ON\r\n[0] LED OFF\r\n[b] Buzzer Chirp\r\nCommand: ";
const uint8_t msg_led_on[]      = "\r\n-> STATUS: LED1 Activated.\r\n";
const uint8_t msg_led_off[]     = "\r\n-> STATUS: LED1 Deactivated.\r\n";
const uint8_t msg_buzzer_ok[]   = "\r\n-> STATUS: Buzzer Triggered.\r\n";
const uint8_t msg_invalid[]     = "\r\n-> ERROR: Invalid Command!\r\n";

/* Buffer for incoming single-byte command */
uint8_t rx_data = 0;

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_USART2_UART_Init();

  /* Transmit initial control menu */
  HAL_UART_Transmit(&huart2, (uint8_t *)msg_menu, sizeof(msg_menu) - 1, HAL_MAX_DELAY);

  while (1)
  {
    /*
     * TIMEOUT CONTROL:
     * Using a bounded 50 ms timeout prevents blocking the execution loop
     * indefinitely when no serial byte is transmitted from the host terminal.
     */
    if (HAL_UART_Receive(&huart2, &rx_data, 1, 50) == HAL_OK)
    {
      switch (rx_data)
      {
        case '1':
          HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
          HAL_UART_Transmit(&huart2, (uint8_t *)msg_led_on, sizeof(msg_led_on) - 1, 100);
          break;

        case '0':
          HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
          HAL_UART_Transmit(&huart2, (uint8_t *)msg_led_off, sizeof(msg_led_off) - 1, 100);
          break;

        case 'b':
        case 'B':
          /* Trigger buzzer for 200 ms pulse */
          HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
          HAL_Delay(200);
          HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
          HAL_UART_Transmit(&huart2, (uint8_t *)msg_buzzer_ok, sizeof(msg_buzzer_ok) - 1, 100);
          break;

        case '\r':
        case '\n':
          /* Ignore terminal line delimiters */
          break;

        default:
          /* Handle undefined characters */
          HAL_UART_Transmit(&huart2, (uint8_t *)msg_invalid, sizeof(msg_invalid) - 1, 100);
          break;
      }
    }

    /* Background tasks can be safely executed here without CPU lockup */
  }
}
