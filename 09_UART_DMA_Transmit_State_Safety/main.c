/**
 * @file    main.c
 * @brief   Core Logic: Non-Blocking UART Transmit via DMA with Transfer Guarding
 * @author  Feyza Yagmur Arat
 * @note    Hardware inits (Clock, GPIO, DMA, USART2) configured via CubeMX.
 */

#include "main.h"

UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_tx;

/* Predefined static telemetry strings */
const uint8_t msg_motor_on[]  = "MOTOR: ACTIVE\r\n";
const uint8_t msg_motor_off[] = "MOTOR: INACTIVE\r\n";

/* DMA transfer guard flag (1: Ready, 0: Busy) */
volatile uint8_t dma_ready = 1;

/* Actuator state (0: OFF, 1: ON) */
uint8_t motor_state = 0;

/* Private function prototypes */
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART2_UART_Init(void);

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART2_UART_Init();

  while (1)
  {
    /* 1. BUTTON INPUT MONITORING (PC13 - Active-Low) */
    if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET)
    {
      HAL_Delay(50); // Software debounce filter

      if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET)
      {
        /* Ensure previous DMA transfer completed before issuing a new request */
        if (dma_ready == 1)
        {
          dma_ready = 0; // Lock transmission pipeline

          if (motor_state == 0)
          {
            motor_state = 1;
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
            HAL_UART_Transmit_DMA(&huart2, (uint8_t *)msg_motor_on, sizeof(msg_motor_on) - 1);
          }
          else
          {
            motor_state = 0;
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
            HAL_UART_Transmit_DMA(&huart2, (uint8_t *)msg_motor_off, sizeof(msg_motor_off) - 1);
          }
        }

        /* Latch until button release */
        while (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET);
      }
    }
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
    /* Release transmission lock to permit subsequent transfers */
    dma_ready = 1;
  }
}
