#ifndef _ALARM_H_
#define _ALARM_H_
#include "JDefs.h"
typedef enum _ALARM_TYPE_
{
    AL_OK = 499,
    AL_FAIL = 299,
    AL_ERR = 199
}ALARM;

typedef enum _ALARM_MODE_
{
    AL_MOD_SINGLE = 0xE0,
    AL_MOD_CONTIN = 0xE1
}ALARM_MODE;
RET_STAT alarmInit(const TIM_HandleTypeDef *ihTimer, const uint32_t iChannel);

RET_STAT genBeep(const ALARM iBeepType, const ALARM_MODE iMode, const uint32_t iDuration);
RET_STAT genBeepOK(void);
RET_STAT genBeepFail(void);
RET_STAT genBeepError(void);
RET_STAT genAlarm(void);
RET_STAT alarmSetPSC(uint32_t iPSC);

#endif

