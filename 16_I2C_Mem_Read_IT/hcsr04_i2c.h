/**
 * @file    hcsr04_i2c.h
 * @brief   Asynchronous Interrupt-Driven (IT) HC-SR04 I2C Driver Header
 * @author  Feyza Yagmur Arat
 */

#ifndef HCSR04_I2C_H
#define HCSR04_I2C_H

#include "main.h"

/* 7-bit slave address shifted for 8-bit HAL format */
#define HCSR04_ADDR             (0x57 << 1)

/* Sensor internal register map */
#define REG_DISTANCE_DATA       0x01  /* Distance measurement output register */

/* Driver API function prototypes (Returns 1 on success, 0 on failure) */
uint8_t HCSR04_IsReady(I2C_HandleTypeDef *hi2c);
uint8_t HCSR04_Read_Distance_IT(I2C_HandleTypeDef *hi2c, uint8_t *rx_data, uint16_t size);

#endif /* HCSR04_I2C_H */
