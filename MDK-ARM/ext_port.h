#ifndef _EXT_PORT_H_
#define _EXT_PORT_H_

#include "JDefs.h"

void sendChar(char ch);
HAL_StatusTypeDef sendStr(const uint8_t *iStr);

#endif
