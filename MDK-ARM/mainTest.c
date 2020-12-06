#include "unity.h"
#include "stm32f1xx_hal.h"
#include <stdint.h>
#include <string.h>
void setUp(void)
{
    
}
void tearDown(void)
{
    
}

void testGASSensor(void)
{
    
}
void testTempSensor(void)
{
    
}

void testALARMSensor(void)
{
    
}

void testDebugPort(void)
{
    extern UART_HandleTypeDef huart1;
    uint8_t strDbgMsg[]= "[TDD] Debug Port Test...\r\n";
    HAL_StatusTypeDef sDBG = HAL_UART_Transmit(&huart1,strDbgMsg,strlen(strDbgMsg),0xFFFF);
    TEST_ASSERT(sDBG == HAL_OK);
}
