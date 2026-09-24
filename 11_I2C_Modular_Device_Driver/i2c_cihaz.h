/**
 * @file    i2c_cihaz.h
 * @brief   Modular I2C Device Driver Header File
 * @author  Feyza Yagmur Arat
 */

#ifndef I2C_CIHAZ_H
#define I2C_CIHAZ_H

#include "main.h"

/* 7-bit slave address shifted for 8-bit HAL format */
#define ADRESS (0x68 << 1)

/* Driver function prototypes (Returns 1 on success, 0 on failure) */
uint8_t Cihaz_Hazir_Mi(I2C_HandleTypeDef *hi2c, uint8_t adress);
uint8_t Cihaz_Yaz(I2C_HandleTypeDef *hi2c, uint8_t adress, uint8_t gonder_veri);
uint8_t Cihaz_Oku(I2C_HandleTypeDef *hi2c, uint8_t adress, uint8_t *gelen_veri);

#endif /* I2C_CIHAZ_H */
