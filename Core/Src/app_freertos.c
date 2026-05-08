/*
 * app_freertos.c
 *
 *      Author: Prashant Jha
 */
#include "app_freertos.h"
#include "logger.h"
#include <math.h>

// Bring in the handles we defined in main.c
extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart2;
extern osMessageQueueId_t sensorQueueHandle;

/* --- TASK 1: THE PRODUCER (Sensor Hub) --- */
void StartSensorHubTask(void *argument) {
    SensorData_t data; // This is our "package"

    for(;;) {
        // 1. Read Raw Data
        MPU6050_Read_Accel(&hi2c1, &data.ax, &data.ay, &data.az);
        BME280_Read_All(&hi2c1, &data.temp, &data.pres);

        // 2. High-Precision Math for Altitude
        data.alt = 44330.0f * (1.0f - powf((data.pres / 1013.25f), (1.0f / 5.255f)));

        // 3. Drop the package in the mailbox
        // (0, 0) means: don't wait if the mailbox is full, just keep going
        osMessageQueuePut(sensorQueueHandle, &data, 0, 0);

        // Heartbeat LED to show the task is alive
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);

        osDelay(100); // Sample at 10Hz
    }
}

/* --- TASK 2: THE CONSUMER (Logger) --- */
void StartLoggerTask(void *argument) {
    SensorData_t logPackage;
    LOG_Init(&huart2);

    for(;;) {
        // This task goes to sleep until a package arrives in the mailbox
        if (osMessageQueueGet(sensorQueueHandle, &logPackage, NULL, osWaitForever) == osOK) {

            // Package received! Print it to the Terminal
            LOG_Info("RTOS_ITC | Z:%.2f | T:%.1fC | P:%.1f | ALT:%.1fm\r\n",
                     logPackage.az, logPackage.temp, logPackage.pres, logPackage.alt);
        }
    }
}
