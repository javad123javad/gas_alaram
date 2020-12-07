#include "ext_port.h"

extern UART_HandleTypeDef huart1;

HAL_StatusTypeDef sendStr(const uint8_t *iStr)
{
    return HAL_UART_Transmit(&huart1, (uint8_t*)iStr, strlen((char*)iStr), 0xFFFF);
}
