/**
 * @file    main.c
 * @brief   Core Logic: Deterministic Actuator Control via UART DMA Receive
 * @author  Feyza Yagmur Arat
 * @note    Hardware inits (Clock, GPIO, DMA, USART2) configured via CubeMX.
 */

#include "main.h"

UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_rx;

/* Command register */
uint8_t rx_cmd = 0;

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

  /* Arm DMA single-byte reception */
  HAL_UART_Receive_DMA(&huart2, &rx_cmd, 1);

  while (1)
  {
    /* Main execution thread remains completely idle */
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
    /* Forward */
    if (rx_cmd == 'F' || rx_cmd == 'f')
    {
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
    }
    /* Backward */
    else if (rx_cmd == 'B' || rx_cmd == 'b')
    {
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
    }
    /* Safe Stop / Neutral */
    else if (rx_cmd == 'S' || rx_cmd == 's')
    {
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
    }

    /* Re-arm DMA pipeline for next command byte */
    HAL_UART_Receive_DMA(&huart2, &rx_cmd, 1);
  }
}
