#include "unity.h"
#include "stm32f1xx_hal.h"
#include "ext_port.h"
#include "alarm.h"
#include "gas.h"
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
    RET_STAT fRet;
    extern ADC_HandleTypeDef hadc1;
    extern TIM_HandleTypeDef htim7;

    
    fRet = gasInit(&hadc1, &htim7);
    TEST_ASSERT_EQUAL(RET_OK, fRet);
    fRet = gasHeatOff();
    TEST_ASSERT_EQUAL(RET_OK, fRet);
    fRet = gasHeatOn();
    TEST_ASSERT_EQUAL(RET_OK, fRet);
    
    
    fRet = gasRead();
    TEST_ASSERT_EQUAL(RET_OK, fRet);

    
}
void testTempSensor(void)
{
    
}

void testALARMSensor(void)
{
    #if 0
    extern TIM_HandleTypeDef htim2;

    RET_STAT fRet;
    fRet = alarmInit(&htim2, TIM_CHANNEL_1);
    TEST_ASSERT_EQUAL(RET_OK, fRet);

    fRet = genBeep(AL_ERR,AL_MOD_SINGLE,1);
    TEST_ASSERT_EQUAL(RET_OK, fRet);
    fRet = genBeepOK();
    TEST_ASSERT_EQUAL(RET_OK, fRet);
    fRet = genBeepFail();
    TEST_ASSERT_EQUAL(RET_OK, fRet);
    fRet = genBeepError();
    TEST_ASSERT_EQUAL(RET_OK, fRet);
    
    fRet = genAlarmOn();
    TEST_ASSERT_EQUAL(RET_OK, fRet);
    HAL_Delay(1000);
    fRet = genAlarmOff();
    TEST_ASSERT_EQUAL(RET_OK, fRet);
    #endif

}

void testDebugPort(void)
{
    uint8_t strDbgMsg[]= "[TDD] Debug Port Test...\r\n";
    HAL_StatusTypeDef sDBG = sendStr(strDbgMsg);
    TEST_ASSERT(sDBG == HAL_OK);
}
