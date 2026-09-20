/**
 * @file    main.c
 * @brief   Core Logic: Asynchronous String Command Parser (CLI) via UART
 * @author  Feyza Yagmur Arat
 * @note    Hardware inits (Clock, GPIO, USART2) configured via CubeMX.
 */

#include "main.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

extern UART_HandleTypeDef huart2;

#define RX_BUFFER_SIZE 64

/* Serial Ingestion & Framing Variables */
uint8_t rx_byte = 0;
char rx_buffer[RX_BUFFER_SIZE];
uint8_t rx_index = 0;
volatile bool cmd_received = false;

char tx_buffer[80];

/* Private function prototypes */
void Process_Serial_Command(void);

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_USART2_UART_Init();

  /* Arm the UART RX interrupt for initial byte capture */
  HAL_UART_Receive_IT(&huart2, &rx_byte, 1);

  while (1)
  {
    /* Service incoming command frame outside ISR context */
    if (cmd_received)
    {
      Process_Serial_Command();
    }
  }
}

/**
  * @brief  Parses verified command frames and executes hardware actions
  * @retval None
  */
void Process_Serial_Command(void)
{
  /* 1. Substring evaluation for parameterized commands */
  if (strstr(rx_buffer, "SET:") != NULL)
  {
    if (strstr(rx_buffer, "LED_ON") != NULL)
    {
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
      HAL_UART_Transmit(&huart2, (uint8_t *)"[OK] LED Turned ON.\r\n", 21, 100);
    }
    else if (strstr(rx_buffer, "LED_OFF") != NULL)
    {
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
      HAL_UART_Transmit(&huart2, (uint8_t *)"[OK] LED Turned OFF.\r\n", 22, 100);
    }
    else
    {
      HAL_UART_Transmit(&huart2, (uint8_t *)"[ERR] Invalid SET parameter!\r\n", 30, 100);
    }
  }
  /* 2. Exact token evaluation for telemetry queries */
  else if (strcmp(rx_buffer, "STATUS") == 0)
  {
    GPIO_PinState led_state = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5);
    sprintf(tx_buffer, "[STATUS] LED State: %s\r\n", (led_state == GPIO_PIN_SET) ? "ACTIVE" : "INACTIVE");
    HAL_UART_Transmit(&huart2, (uint8_t *)tx_buffer, strlen(tx_buffer), 100);
  }
  /* 3. Catch-all for undefined syntax */
  else
  {
    HAL_UART_Transmit(&huart2, (uint8_t *)"[ERR] Unknown Command!\r\n", 24, 100);
  }

  /* 4. Flush buffer and restore reception state */
  memset(rx_buffer, 0, RX_BUFFER_SIZE);
  rx_index = 0;
  cmd_received = false;
}

/**
  * @brief  Rx Transfer completed callback (Byte-by-byte frame collector)
  * @param  huart UART handle
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART2)
  {
    /* Delimiter detected: frame complete */
    if (rx_byte == '\r' || rx_byte == '\n')
    {
      if (rx_index > 0)
      {
        rx_buffer[rx_index] = '\0'; // Null-terminate frame
        cmd_received = true;        // Signal main loop
      }
    }
    else
    {
      /* Enforce buffer bounds against overflow */
      if (rx_index < (RX_BUFFER_SIZE - 1))
      {
        rx_buffer[rx_index++] = (char)rx_byte;
      }
    }

    /* Re-arm interrupt for next character */
    HAL_UART_Receive_IT(&huart2, &rx_byte, 1);
  }
}
