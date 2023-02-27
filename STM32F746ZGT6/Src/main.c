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

int main(void)
{
	init_RCC();
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; // Enable GPIOB port clock
    GPIOB->MODER |= 1 << GPIO_MODER_MODER0_Pos; // Enable General purpose output mode for PB0

    /* Loop forever */
    for(;;)
    {
        for(int i = 0; i < 100000; i++);
        /* Shift by a given mask PB0 change the values of PB0 by XOR operation
         * ODR0 = 0, therefore 0 ^ 1 = 1 and etc*/
        GPIOB->ODR ^= 1 << GPIO_ODR_OD0_Pos;
    }
}
