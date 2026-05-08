/*
 * bme280.h
 *
 *      Author: Prashant Jha
 */

#ifndef BME280_H_
#define BME280_H_

#include "stm32f4xx_hal.h"

#define BME280_ADDR (0x76 << 1)
#define ID_REG      0xD0
#define CTRL_MEAS_REG 0xF4

// Calibration Data Structure
typedef struct {
    uint16_t dig_T1;
    int16_t  dig_T2, dig_T3;

    uint16_t dig_P1;
    int16_t  dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;

    int32_t  t_fine;
} BME280_Calib;

// 2. Updated Function Prototypes
uint8_t BME280_Init(I2C_HandleTypeDef *hi2c);
void BME280_Read_All(I2C_HandleTypeDef *hi2c, float *Temperature, float *Pressure);
#endif
