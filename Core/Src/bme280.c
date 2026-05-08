/*
 * bme280.c
 *
 *      Author: Prashant Jha
 */
#include "bme280.h"
#include <math.h>

BME280_Calib CalibData;

uint8_t BME280_Init(I2C_HandleTypeDef *hi2c) {
    uint8_t check;
    uint8_t calib_raw[24]; // Increased to read all parameters

    HAL_I2C_Mem_Read(hi2c, BME280_ADDR, ID_REG, 1, &check, 1, 100);

    if (check == 0x60) {
        // 1. Read Temp Calibration (6 bytes)
        HAL_I2C_Mem_Read(hi2c, BME280_ADDR, 0x88, 1, calib_raw, 6, 100);
        CalibData.dig_T1 = (calib_raw[1] << 8) | calib_raw[0];
        CalibData.dig_T2 = (calib_raw[3] << 8) | calib_raw[2];
        CalibData.dig_T3 = (calib_raw[5] << 8) | calib_raw[4];

        // 2. Read Pressure Calibration (18 bytes)
        HAL_I2C_Mem_Read(hi2c, BME280_ADDR, 0x8E, 1, calib_raw, 18, 100);
        CalibData.dig_P1 = (calib_raw[1] << 8) | calib_raw[0];
        CalibData.dig_P2 = (calib_raw[3] << 8) | calib_raw[2];
        CalibData.dig_P3 = (calib_raw[5] << 8) | calib_raw[4];
        CalibData.dig_P4 = (calib_raw[7] << 8) | calib_raw[6];
        CalibData.dig_P5 = (calib_raw[9] << 8) | calib_raw[8];
        CalibData.dig_P6 = (calib_raw[11] << 8) | calib_raw[10];
        CalibData.dig_P7 = (calib_raw[13] << 8) | calib_raw[12];
        CalibData.dig_P8 = (calib_raw[15] << 8) | calib_raw[14];
        CalibData.dig_P9 = (calib_raw[17] << 8) | calib_raw[16];

        // 3. Set to Normal Mode, Pressure x1, Temp x1
        uint8_t data = 0x27;
        HAL_I2C_Mem_Write(hi2c, BME280_ADDR, CTRL_MEAS_REG, 1, &data, 1, 100);
        return 0;
    }
    return 1;
}

void BME280_Read_All(I2C_HandleTypeDef *hi2c, float *Temperature, float *Pressure) {
    uint8_t data[6];
    HAL_I2C_Mem_Read(hi2c, BME280_ADDR, 0xF7, 1, data, 6, 100);

    int32_t adc_P = (int32_t)((data[0] << 12) | (data[1] << 4) | (data[2] >> 4));
    int32_t adc_T = (int32_t)((data[3] << 12) | (data[4] << 4) | (data[5] >> 4));

    // Temperature Compensation (Calculates t_fine first)
    int32_t var1_T, var2_T, T;
    var1_T = ((((adc_T >> 3) - ((int32_t)CalibData.dig_T1 << 1))) * ((int32_t)CalibData.dig_T2)) >> 11;
    var2_T = (((((adc_T >> 4) - ((int32_t)CalibData.dig_T1)) * ((adc_T >> 4) - ((int32_t)CalibData.dig_T1))) >> 12) * ((int32_t)CalibData.dig_T3)) >> 14;
    CalibData.t_fine = var1_T + var2_T;
    T = (CalibData.t_fine * 5 + 128) >> 8;
    *Temperature = (float)T / 100.0;

    // Pressure Compensation
    int64_t var1, var2, p;
    var1 = ((int64_t)CalibData.t_fine) - 128000;
    var2 = var1 * var1 * (int64_t)CalibData.dig_P6;
    var2 = var2 + ((var1 * (int64_t)CalibData.dig_P5) << 17);
    var2 = var2 + (((int64_t)CalibData.dig_P4) << 35);
    var1 = ((var1 * var1 * (int64_t)CalibData.dig_P3) >> 8) + ((var1 * (int64_t)CalibData.dig_P2) << 12);
    var1 = (((((int64_t)1) << 47) + var1)) * ((int64_t)CalibData.dig_P1) >> 33;

    if (var1 == 0) {
        *Pressure = 0; // Avoid division by zero
    } else {
        p = 1048576 - adc_P;
        p = (((p << 31) - var2) * 3125) / var1;
        var1 = (((int64_t)CalibData.dig_P9) * (p >> 13) * (p >> 13)) >> 25;
        var2 = (((int64_t)CalibData.dig_P8) * p) >> 19;
        p = ((p + var1 + var2) >> 8) + (((int64_t)CalibData.dig_P7) << 4);
        *Pressure = (float)p / 256.0 / 100.0; // In hPa
    }
}
