#ifndef _GAS_H_
#define _GAS_H_
#include "JDefs.h"
/******************************************************************************/

#define     GAS_HEATER_ON_TIME  5
#define     GAS_HEATER_OFF_TIME (GAS_HEATER_ON_TIME + 10)
#define     GAS_PPM_TH      3000

RET_STAT    gasInit(ADC_HandleTypeDef * iGasADC, TIM_HandleTypeDef * iHeaterTimer);
RET_STAT    gasHeatOn(void);
RET_STAT    gasHeatOff(void);
RET_STAT    gasRead(uint32_t *o_val);

#endif
