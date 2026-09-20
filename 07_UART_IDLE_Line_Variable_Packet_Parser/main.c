/**
 * @file    main.c
 * @brief   Core Logic: Variable-Length UART Reception via HAL_UARTEx IDLE Event
 * @author  Feyza Yagmur Arat
 * @note    Hardware inits (Clock, GPIO, USART2) configured via CubeMX.
 */

#include "main.h"
#include <stdio.h>
#include <string.h>

UART_HandleTypeDef huart2;

#define RX_BUFFER_SIZE 128

/* Reception and Transmission Buffers */
uint8_t rx_buffer[RX_BUFFER_SIZE];
char tx_response[160];

/* Private function prototypes */
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_USART2_UART_Init();

  /* Arm asynchronous reception up to buffer capacity or until IDLE line condition */
  HAL_UARTEx_ReceiveToIdle_IT(&huart2, rx_buffer, RX_BUFFER_SIZE);

  /* Transmit startup banner */
  HAL_UART_Transmit(&huart2, (uint8_t *)"--- UART Extended IDLE Listener Online ---\r\n", 44, 100);

  while (1)
  {
    /* Background loop executes freely; packet reception is event-driven */
  }
}

/**
  * @brief  Reception Event Callback (Triggered upon buffer full or IDLE line event)
  * @param  huart UART handle
  * @param  Size  Total bytes received across the bus
  * @retval None
  */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
  if (huart->Instance == USART2)
  {
    /* Bounds-checked null termination */
    if (Size < RX_BUFFER_SIZE)
    {
      rx_buffer[Size] = '\0';
    }
    else
    {
      rx_buffer[RX_BUFFER_SIZE - 1] = '\0';
    }

    /* Visual confirmation */
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);

    /* Construct and transmit telemetry response */
    sprintf(tx_response, "[IDLE EVENT] Size: %u Bytes | Payload: %s\r\n", Size, (char *)rx_buffer);
    HAL_UART_Transmit(&huart2, (uint8_t *)tx_response, strlen(tx_response), 100);

    /* Re-arm reception for the subsequent packet burst */
    HAL_UARTEx_ReceiveToIdle_IT(&huart2, rx_buffer, RX_BUFFER_SIZE);
  }
}
