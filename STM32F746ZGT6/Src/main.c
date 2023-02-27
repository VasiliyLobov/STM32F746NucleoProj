/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Vasiliy Lobov
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
#include <stdint.h>
#include "stm32f7xx.h"
#include "rcc.h"
#include "gpio.h"
#include "timer.h"

int main(void)
{
	init_RCC();
    init_GPIO();
    init_TIM8();

    /* Loop forever */
    for(;;)
    {
        for(int i = 0; i < 100000; i++);
        /* Shift by a given mask PB0 change the values of PB0 by XOR operation
         * ODR0 = 0, therefore 0 ^ 1 = 1 and etc*/
        GPIOB->ODR ^= 1 << GPIO_ODR_OD0_Pos;
    }
}
