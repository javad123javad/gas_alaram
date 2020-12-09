#include "alarm.h"

static const    TIM_HandleTypeDef* gAlarmTimer;
static          uint32_t           gAlarmChannel;
uint32_t        gTone=39;

/******************************************************************************/
inline RET_STAT alarmSetPSC(uint32_t iPSC)
{
        gAlarmTimer->Instance->PSC = iPSC;

}
static inline RET_STAT alarmOn()
{
    return HAL_TIM_PWM_Start((TIM_HandleTypeDef *)gAlarmTimer, gAlarmChannel);
}
static inline RET_STAT alarmOff()
{
    return HAL_TIM_PWM_Stop((TIM_HandleTypeDef *)gAlarmTimer, gAlarmChannel);
}
RET_STAT alarmInit(const TIM_HandleTypeDef *ihTimer, const uint32_t iChannel)
{
    RET_STAT fRet = RET_FAIL;
    gAlarmTimer = ihTimer;
    gAlarmChannel = iChannel;
    fRet = alarmOn();
    //HAL_Delay(10);
    fRet = alarmOff();

return fRet;
}
RET_STAT genBeep(const ALARM iBeepType,  ALARM_MODE iMode, const uint32_t iDuration)
{
    RET_STAT fRet = RET_FAIL;
    gAlarmTimer->Instance->PSC = iBeepType;
    fRet = alarmOn();
    HAL_Delay(iDuration);
    if(AL_MOD_SINGLE == iMode)
    {
        fRet = alarmOff();
    }
return fRet;
}
inline RET_STAT genBeepOK(void)
{
    return genBeep(AL_OK, AL_MOD_SINGLE, 200);
 
}

inline RET_STAT genBeepFail(void)
{
    return genBeep(AL_FAIL, AL_MOD_SINGLE, 1000);

}
inline RET_STAT genBeepError(void)
{
    uint8_t i = 0;
    RET_STAT fRet=RET_FAIL;
    while((i++)<10)
    {
        fRet = genBeep(AL_ERR, AL_MOD_SINGLE, 100);
        HAL_Delay(50);
    }
    return fRet;
}

RET_STAT genAlarmOn(void)
{
    RET_STAT fRet=RET_FAIL;
    extern TIM_HandleTypeDef htim6;
    HAL_TIM_Base_Start_IT(&htim6);
    fRet = alarmOn();


    return fRet;
}

RET_STAT genAlarmOff(void)
{
    RET_STAT fRet=RET_FAIL;
    extern TIM_HandleTypeDef htim6;
    
    HAL_TIM_Base_Stop_IT(&htim6);
    fRet = alarmOff();

    return fRet;
}