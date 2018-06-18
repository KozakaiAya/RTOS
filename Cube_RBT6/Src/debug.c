#include "debug.h"
#include "stm32f1xx_hal.h"

#include <string.h>


int logger(UART_HandleTypeDef* huart, char* msg)
{
    int len = strlen(msg);
    HAL_UART_Transmit(huart, msg, len, 0xFFFF);
    return 0;
}