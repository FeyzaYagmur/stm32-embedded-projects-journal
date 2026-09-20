/**
 * @file    main.c
 * @brief   Core Logic: DMA UART Reception with IDLE Line & Dynamic PWM Control
 * @author  Feyza Yagmur Arat
 * @note    Hardware inits (Clock, GPIO, DMA, TIM2, USART2) configured via CubeMX.
 */

#include "main.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_rx;
TIM_HandleTypeDef htim2;

#define RX_BUFFER_SIZE 64

/* Buffers */
uint8_t rx_buffer[RX_BUFFER_SIZE];
char tx_feedback[64];

/* Private function prototypes */
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_TIM2_Init(void);
static void MX_USART2_UART_Init(void);

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM2_Init();
  MX_USART2_UART_Init();

  /* 1. Start PWM generation with initial zero duty cycle */
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);

  /* 2. Arm DMA reception for variable-length IDLE line capture */
  HAL_UARTEx_ReceiveToIdle_DMA(&huart2, rx_buffer, RX_BUFFER_SIZE);

  /* Transmit startup banner */
  HAL_UART_Transmit(&huart2, (uint8_t *)"--- DMA IDLE Motor/PWM Controller Online ---\r\n", 47, 100);

  while (1)
  {
    /* Heartbeat routine runs freely without serial ingestion blocking */
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
    HAL_Delay(500);
  }
}

/**
  * @brief  Reception Event Callback (Triggered upon DMA buffer full or IDLE line event)
  * @param  huart UART handle
  * @param  Size  Total bytes received across the bus
  * @retval None
  */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
  if (huart->Instance == USART2)
  {
    /* Null-terminate incoming payload */
    if (Size < RX_BUFFER_SIZE)
    {
      rx_buffer[Size] = '\0';
    }
    else
    {
      rx_buffer[RX_BUFFER_SIZE - 1] = '\0';
    }

    /* 1. Parse STOP command */
    if (strstr((char *)rx_buffer, "STOP") != NULL || strstr((char *)rx_buffer, "DUR") != NULL)
    {
      __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);
      HAL_UART_Transmit(&huart2, (uint8_t *)"[OK] Motor Stopped.\r\n", 21, 100);
    }
    /* 2. Parse SPEED command with numerical value */
    else if (strstr((char *)rx_buffer, "SPEED") != NULL || strstr((char *)rx_buffer, "HIZ") != NULL)
    {
      /* Locate first digit following command prefix */
      char *digit_ptr = (char *)rx_buffer;
      while (*digit_ptr && (*digit_ptr < '0' || *digit_ptr > '9'))
      {
        digit_ptr++;
      }

      int new_speed = atoi(digit_ptr);

      /* Enforce safety boundary constraint (0 to 1000) */
      if (new_speed >= 0 && new_speed <= 1000)
      {
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, (uint32_t)new_speed);
        sprintf(tx_feedback, "[OK] Speed Set to: %d\r\n", new_speed);
        HAL_UART_Transmit(&huart2, (uint8_t *)tx_feedback, strlen(tx_feedback), 100);
      }
      else
      {
        HAL_UART_Transmit(&huart2, (uint8_t *)"[ERR] Speed Out of Bounds (0-1000)!\r\n", 37, 100);
      }
    }
    else
    {
      HAL_UART_Transmit(&huart2, (uint8_t *)"[ERR] Unknown Command!\r\n", 24, 100);
    }

    /* 3. Flush buffer and restore reception state */
    memset(rx_buffer, 0, RX_BUFFER_SIZE);

    /* 4. Re-arm DMA reception for subsequent command frame */
    HAL_UARTEx_ReceiveToIdle_DMA(&huart2, rx_buffer, RX_BUFFER_SIZE);
  }
}
