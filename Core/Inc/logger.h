/*
 * logger.h
 *
 *      Author: Prashant Jha
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

void LOG_Init(UART_HandleTypeDef *huart);
void LOG_Info(const char *fmt, ...);

#endif
