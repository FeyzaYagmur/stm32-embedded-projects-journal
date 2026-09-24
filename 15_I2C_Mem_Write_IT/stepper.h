/**
 * @file    stepper.h
 * @brief   Asynchronous Interrupt-Driven (IT) Stepper Motor Driver Header
 * @author  Feyza Yagmur Arat
 */

#ifndef STEPPER_H
#define STEPPER_H

#include "main.h"

/* 7-bit slave address shifted for 8-bit HAL format */
#define STEPPER_ADDR         (0x60 << 1)

/* Stepper driver internal register map */
#define REG_STEP_COUNT       0x01  /* Step count configuration register */

/* Driver API function prototypes (Returns 1 on success, 0 on failure) */
uint8_t Stepper_IsReady(I2C_HandleTypeDef *hi2c);
uint8_t Stepper_Write_Reg_IT(I2C_HandleTypeDef *hi2c, uint16_t reg_addr, uint8_t *p_data, uint16_t size);

#endif /* STEPPER_H */
