/**
 * @file    sensor.h
 * @brief   Register-Level I2C Sensor Driver Header
 * @author  Feyza Yagmur Arat
 */

#ifndef SENSOR_H
#define SENSOR_H

#include "main.h"

/* 7-bit slave address shifted for 8-bit HAL format */
#define SENSOR_ADDR         (0x68 << 1)

/* Sensor internal register map */
#define REG_CONFIG          0x00   /* Configuration and power management register */
#define REG_DATA_OUT        0x01   /* Telemetry output measurement register */

/* Driver API function prototypes (Returns 1 on success, 0 on failure) */
uint8_t Sensor_Is_Ready(I2C_HandleTypeDef *hi2c);
uint8_t Sensor_Write_Config(I2C_HandleTypeDef *hi2c, uint8_t config_val);
uint8_t Sensor_Read_Data(I2C_HandleTypeDef *hi2c, uint8_t *rx_data);

#endif /* SENSOR_H */
