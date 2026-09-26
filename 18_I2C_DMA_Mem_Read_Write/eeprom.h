/**
 * @file    eeprom.h
 * @brief   Hardware-Accelerated I2C DMA EEPROM Driver Header
 * @author  Feyza Yagmur Arat
 */

#ifndef EEPROM_H
#define EEPROM_H

#include "main.h"

/* 7-bit slave address shifted for 8-bit HAL format */
#define EEPROM_ADDR             (0x50 << 1)

/* Target internal memory register */
#define REG_DATA_CELL           0x08

/* Driver API function prototypes (Returns 1 on success, 0 on failure) */
uint8_t EEPROM_IsReady(I2C_HandleTypeDef *hi2c);
uint8_t EEPROM_Write_Mem_DMA(I2C_HandleTypeDef *hi2c, uint16_t reg_addr, uint8_t *p_tx_buf, uint16_t size);
uint8_t EEPROM_Read_Mem_DMA(I2C_HandleTypeDef *hi2c, uint16_t reg_addr, uint8_t *p_rx_buf, uint16_t size);

#endif /* EEPROM_H */
