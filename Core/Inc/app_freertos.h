/*
 * app_freertos.h
 *
 *      Author: Prashant Jha
 */
#ifndef APP_FREERTOS_H_
#define APP_FREERTOS_H_

/* USER CODE BEGIN Includes */
#include "main.h"
#include "cmsis_os.h"  // This is the bridge to FreeRTOS
#include "mpu6050.h"
#include "bme280.h"
#include <stdio.h>
#include <string.h>

/* USER CODE END Includes */

// 1. Define the Data Package structure for the Queue
typedef struct {
    float ax, ay, az;
    float temp, pres, alt;
} SensorData_t;

// 2. Function Prototypes for our two tasks
void StartSensorHubTask(void *argument);
void StartLoggerTask(void *argument);
#endif
