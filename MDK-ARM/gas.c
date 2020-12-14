#include "gas.h"
#include "main.h"
#include <stdio.h>
uint8_t gHeatTimer = 0;
extern ADC_HandleTypeDef hadc1;
extern UART_HandleTypeDef huart1;

inline RET_STAT    gasHeatOn(void)
{
    HAL_GPIO_WritePin(GAS_HEAT_EN_GPIO_Port, GAS_HEAT_EN_Pin, GPIO_PIN_SET);
    return RET_OK;
}
inline RET_STAT    gasHeatOff(void)
{
    HAL_GPIO_WritePin(GAS_HEAT_EN_GPIO_Port, GAS_HEAT_EN_Pin, GPIO_PIN_RESET);
    return RET_OK;
}

RET_STAT    gasInit(ADC_HandleTypeDef * iGasADC, TIM_HandleTypeDef * iHeaterTimer)
{
    RET_STAT fRet = RET_FAIL;
    fRet = gasHeatOn();
    fRet = HAL_TIM_Base_Start_IT(iHeaterTimer);
    fRet = HAL_ADCEx_Calibration_Start(iGasADC);
    fRet = HAL_ADC_Start(iGasADC);
    return fRet;
}

RET_STAT    gasRead(uint32_t *o_val)
{
    RET_STAT fRet = RET_FAIL;
    HAL_ADC_Start(&hadc1);
    fRet = HAL_ADC_PollForConversion(&hadc1,0xFF);
    *o_val = HAL_ADC_GetValue(&hadc1);
    return fRet;
}
