#include "ext_port.h"

extern UART_HandleTypeDef huart1;
void sendChar(char ch)
{
    uint8_t iStr[2] = {0};
    iStr[0]=ch;
    HAL_UART_Transmit(&huart1, (uint8_t*)iStr, 1, 0xFFFF);
}
HAL_StatusTypeDef sendStr(const uint8_t *iStr)
{
    return HAL_UART_Transmit(&huart1, (uint8_t*)iStr, strlen((char*)iStr), 0xFFFF);
}
