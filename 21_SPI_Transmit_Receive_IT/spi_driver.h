/**
 * @file    spi_driver.h
 * @brief   Asynchronous Interrupt-Driven (IT) SPI Driver Header
 * @author  Feyza Yagmur Arat
 */

#ifndef SPI_DRIVER_H
#define SPI_DRIVER_H

#include "main.h"

/* Software Chip Select (CS) Pin Mapping */
#define CS_GPIO_PORT            GPIOA
#define CS_PIN                  GPIO_PIN_4

/* Target Peripheral Register Map */
#define REG_SHUTDOWN            0x0C  /* Power mode / shutdown register */
#define REG_DATA_X              0x01  /* Telemetry measurement register */

/* Driver API function prototypes */
void SPI_Driver_Write_Reg_IT(SPI_HandleTypeDef *hspi, uint8_t reg_addr, uint8_t data);
void SPI_Driver_Read_Reg_IT(SPI_HandleTypeDef *hspi, uint8_t reg_addr, uint8_t *p_rx_buf);

#endif /* SPI_DRIVER_H */
