/*
 * mpu6050.c
 *
 *      Author: Prashant Jha
 */
#include "mpu6050.h"

uint8_t MPU6050_Init(I2C_HandleTypeDef *hi2c) {
    uint8_t check;
    uint8_t data;

    // 1. Check if the device is actually an MPU6050
    HAL_I2C_Mem_Read(hi2c, MPU6050_ADDR, WHO_AM_I_REG, 1, &check, 1, 100);

    if (check == 0x68) { // 0x68 is the "signature" of this chip
        // 2. Wake the sensor up (it starts in sleep mode)
        data = 0;
        HAL_I2C_Mem_Write(hi2c, MPU6050_ADDR, PWR_MGMT_1_REG, 1, &data, 1, 100);
        return 0; // Success
    }
    return 1; // Error
}

void MPU6050_Read_Accel(I2C_HandleTypeDef *hi2c, float *Ax, float *Ay, float *Az) {
    uint8_t data[6];
    int16_t raw_x, raw_y, raw_z;

    // Read 6 bytes starting from ACCEL_XOUT_H (0x3B)
    HAL_I2C_Mem_Read(hi2c, MPU6050_ADDR, ACCEL_XOUT_H_REG, 1, data, 6, 100);

    // Combine High and Low bytes into 16-bit integers
    raw_x = (int16_t)(data[0] << 8 | data[1]);
    raw_y = (int16_t)(data[2] << 8 | data[3]);
    raw_z = (int16_t)(data[4] << 8 | data[5]);

    // Convert to 'g' (1g = earth gravity)
    *Ax = raw_x / 16384.0;
    *Ay = raw_y / 16384.0;
    *Az = raw_z / 16384.0;
}

