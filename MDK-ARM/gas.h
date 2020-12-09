#ifndef _GAS_H_
#define _GAS_H_
#include "JDefs.h"
/******************************************************************************/

#define     GAS_HEATER_ON_TIME  60
#define     GAS_HEATER_OFF_TIME (GAS_HEATER_ON_TIME + 90)
RET_STAT    gasInit(ADC_HandleTypeDef * iGasADC, TIM_HandleTypeDef * iHeaterTimer);
RET_STAT    gasHeatOn(void);
RET_STAT    gasHeatOff(void);
RET_STAT    gasRead(void);

#endif
