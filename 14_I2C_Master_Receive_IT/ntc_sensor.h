/**
 * @file    ntc_sensor.h
 * @brief   Asynchronous Interrupt-Driven (IT) NTC Sensor Driver Header
 * @author  Feyza Yagmur Arat
 */

#ifndef NTC_SENSOR_H
#define NTC_SENSOR_H

#include "main.h"

/* 7-bit slave address shifted for 8-bit HAL format */
#define NTC_SENSOR_ADDR         (0x48 << 1)

/* Driver API function prototypes (Returns 1 on success, 0 on failure) */
uint8_t NTC_Sensor_IsReady(I2C_HandleTypeDef *hi2c);
uint8_t NTC_Sensor_Receive_IT(I2C_HandleTypeDef *hi2c, uint8_t *p_data, uint16_t size);

#endif /* NTC_SENSOR_H */
