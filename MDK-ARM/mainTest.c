#include "unity.h"
#include "stm32f1xx_hal.h"
#include "ext_port.h"
#include "alarm.h"
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
    extern TIM_HandleTypeDef htim2;

    RET_STAT fRet;
    fRet = alarmInit(&htim2, TIM_CHANNEL_1);
    TEST_ASSERT_EQUAL(RET_OK, fRet);
/*
    fRet = genBeep(AL_ERR,AL_MOD_SINGLE,1);
    TEST_ASSERT_EQUAL(RET_OK, fRet);
    fRet = genBeepOK();
    TEST_ASSERT_EQUAL(RET_OK, fRet);
    fRet = genBeepFail();
    TEST_ASSERT_EQUAL(RET_OK, fRet);
    fRet = genBeepError();
    TEST_ASSERT_EQUAL(RET_OK, fRet);
    */
     fRet = genAlarm();
    TEST_ASSERT_EQUAL(RET_OK, fRet);
    
}

void testDebugPort(void)
{
    uint8_t strDbgMsg[]= "[TDD] Debug Port Test...\r\n";
    HAL_StatusTypeDef sDBG = sendStr(strDbgMsg);
    TEST_ASSERT(sDBG == HAL_OK);
}
