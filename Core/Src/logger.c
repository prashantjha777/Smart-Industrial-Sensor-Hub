/*
 * logger.c
 *
 *      Author: Prashant Jha
 */
#include "logger.h"

static UART_HandleTypeDef *log_uart;

void LOG_Init(UART_HandleTypeDef *huart) {
    log_uart = huart;
}

void LOG_Info(const char *fmt, ...) {
    char buf[128];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);

    HAL_UART_Transmit(log_uart, (uint8_t*)buf, strlen(buf), 100);
}

