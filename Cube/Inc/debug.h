#include "stm32f1xx_hal.h"

extern UART_HandleTypeDef huart1;

int logger(UART_HandleTypeDef* huart, char* msg);
