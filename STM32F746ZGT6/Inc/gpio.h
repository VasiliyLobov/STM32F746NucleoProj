
#ifndef __GPIO_H__
#define __GPIO_H__

#include "stm32f7xx.h"

void init_GPIO(void);
void init_GPIO_Output(GPIO_TypeDef * gpio, unsigned int pin);
void init_GPIO_AFunction(GPIO_TypeDef * gpio, unsigned int pin, unsigned int AF);

#endif
