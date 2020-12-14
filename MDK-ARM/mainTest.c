
#include "stm32f1xx_hal.h"
#include "ext_port.h"
#include "alarm.h"
#include "gas.h"
#include "LCD.h"
#include <stdint.h>
#include <string.h>


#include "unity.h"

extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim7;
extern TIM_HandleTypeDef htim2;
extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart1;

uint32_t    gGasVal;
uint8_t    gValidRead;
void setUp(void)
{
    
}
void tearDown(void)
{
    
}

void testGASSensor(void)
{
    RET_STAT fRet;
    

    
    fRet = gasInit(&hadc1, &htim7);
    TEST_ASSERT_EQUAL(RET_OK, fRet);
    fRet = gasHeatOff();
    TEST_ASSERT_EQUAL(RET_OK, fRet);
    fRet = gasHeatOn();
    TEST_ASSERT_EQUAL(RET_OK, fRet);
    
    
    fRet = gasRead(&gGasVal);
    TEST_ASSERT_EQUAL(RET_OK, fRet);

    
}
void testTempSensor(void)
{
    
}

void testALARMSensor(void)
{
    #if 0

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
void testLCD(void)
{
    lcd_init();
    RET_STAT fRet = I2C_Scan(&hi2c1, &huart1);
    TEST_ASSERT_EQUAL(RET_OK, fRet);
    lcdInit(&hi2c1, (uint8_t)0x27, (uint8_t)4, (uint8_t)20);
    
    // Print text and home position 0,0
    lcdPrintStr((uint8_t*)"Hellow,", 7);
    lcdSetCursorPosition(0, 1);
    lcdPrintStr((uint8_t*)"Javad,", 5);

    while(1);
}
void testDebugPort(void)
{
    uint8_t strDbgMsg[]= "[TDD] Debug Port Test...\r\n";
    HAL_StatusTypeDef sDBG = sendStr(strDbgMsg);
    TEST_ASSERT(sDBG == HAL_OK);
}
void testMainLoop(void)
{
    RET_STAT fRet = RET_FAIL;
    uint8_t tmpBuff[32] = {0};
    // Init Setup
    fRet = alarmInit(&htim2, TIM_CHANNEL_1);
    TEST_ASSERT_EQUAL(RET_OK, fRet);
    
    fRet = gasInit(&hadc1, &htim7);
    TEST_ASSERT_EQUAL(RET_OK, fRet);
    // Greeting Message
    testDebugPort();
    // Main Loop Begin
    while(1)
    {
        if(gValidRead)
        {
            lcd_clear();
            if(gGasVal > GAS_PPM_TH)
            {
                //Alarm On
                genAlarmOn();
                lcd_send_string("GAS Leakage1");
            }
            else
            {
                genAlarmOff();
                lcd_send_string("SYS UP");
            }
            sprintf((char*)tmpBuff, "Val:%u", gGasVal);
            sendStr(tmpBuff);
            gValidRead = 0;
        }
    }
    // Check Status
}
