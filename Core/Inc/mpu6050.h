/*
 * mpu6050.h
 *
 *      Author: Prashant Jha
 */

#ifndef MPU6050_H_
#define MPU6050_H_

#include "stm32f4xx_hal.h" // Gives us access to I2C functions

// The I2C address we found earlier (0x68 shifted left)
#define MPU6050_ADDR 0x68 << 1

// MPU6050 Register Map
#define WHO_AM_I_REG 0x75
#define PWR_MGMT_1_REG 0x6B
#define ACCEL_XOUT_H_REG 0x3B

// Function Prototypes
/* USER CODE BEGIN 0 */
uint8_t MPU6050_Init(I2C_HandleTypeDef *hi2c);
void MPU6050_Read_Accel(I2C_HandleTypeDef *hi2c, float *Ax, float *Ay, float *Az);
/* USER CODE END 0 */

#endif /* MPU6050_H_ */
