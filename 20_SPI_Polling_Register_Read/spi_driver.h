/**
 * @file    spi_driver.h
 * @brief   Synchronous SPI Peripheral Driver Header: Register Read & Write
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

/* Driver API function prototypes (Returns 1 on success, 0 on failure) */
uint8_t SPI_Driver_Write_Reg(SPI_HandleTypeDef *hspi, uint8_t reg_addr, uint8_t data);
uint8_t SPI_Driver_Read_Reg(SPI_HandleTypeDef *hspi, uint8_t reg_addr, uint8_t *p_rx_buf);

#endif /* SPI_DRIVER_H */
