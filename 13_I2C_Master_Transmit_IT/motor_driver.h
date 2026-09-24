/**
 * @file    motor_driver.h
 * @brief   I2C Driver Header using HAL_I2C_Master_Transmit_IT
 * @author  Feyza Yagmur Arat
 */

#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include "main.h"

/* 7-bit slave address shifted for 8-bit HAL format */
#define MOTOR_DRIVER_ADDR       (0x58 << 1)

/* Driver API function prototypes (Returns 1 on success, 0 on failure) */
uint8_t Motor_Driver_IsReady(I2C_HandleTypeDef *hi2c);
uint8_t Motor_Driver_Transmit_IT(I2C_HandleTypeDef *hi2c, uint8_t *p_data, uint16_t size);

#endif /* MOTOR_DRIVER_H */
