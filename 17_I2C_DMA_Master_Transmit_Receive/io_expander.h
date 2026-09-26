/**
 * @file    io_expander.h
 * @brief   Hardware-Accelerated I2C DMA IO Expander Driver Header
 * @author  Feyza Yagmur Arat
 */

#ifndef IO_EXPANDER_H
#define IO_EXPANDER_H

#include "main.h"

/* 7-bit slave address shifted for 8-bit HAL format */
#define IO_EXPANDER_ADDR        (0x20 << 1)

/* Driver API function prototypes (Returns 1 on success, 0 on failure) */
uint8_t IO_Expander_IsReady(I2C_HandleTypeDef *hi2c);
uint8_t IO_Expander_Transmit_DMA(I2C_HandleTypeDef *hi2c, uint8_t *p_tx_buf, uint16_t size);
uint8_t IO_Expander_Receive_DMA(I2C_HandleTypeDef *hi2c, uint8_t *p_rx_buf, uint16_t size);

#endif /* IO_EXPANDER_H */
