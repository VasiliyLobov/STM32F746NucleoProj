/**
 ******************************************************************************
 * @file           : rcc.c
 * @author         : Vasiliy Lobov
 * @brief          : RCC clock setting
 ******************************************************************************
 *
 * In this file you can set frequency of MCU at 16, 32 and 216 MHz and configure the HCLK clock from both
 * the internal resonator and the external generator, you only need to comment out some lines, and uncomment others
 *
 * For MCU working at 16 MHz you need to uncomment next lines: 43, 45.
 *
 * For MCU working at 32 MHz you need to uncomment lines: 60-61, 75, 86-89, 123;
 * and comment lines: 49, 54-56, 70, 80-83, 106-110, 117, 119;
 *
 * For MCU working at 216 MHz you need to uncomment lines: 49, 54-56, 70, 80-83, 106-110, 117, 119;
 * and comment lines: 60-61, 75, 86-89, 123;
 *
 * See the stm32f476xx.h file for RCC and PLL setting, and STM32CubeMX as it conveniently depicts the PLL setting structure
 *
 ******************************************************************************
 */
#include "stm32f7xx.h"

#include "rcc.h"

void init_RCC(void)
{
    // Reset Peripherals.
    RCC->AHB1RSTR = 0xFFFFFFFF; // Reset AHB1 Peripherals
    RCC->AHB1RSTR = 0x00000000; // Set AHB1 Peripherals

    RCC->AHB2RSTR = 0xFFFFFFFF; // Reset AHB2 Peripherals
    RCC->AHB2RSTR = 0x00000000; // Set AHB2 Peripherals

    RCC->APB1RSTR = 0xFFFFFFFF; // Reset APB1 Peripherals
    RCC->APB1RSTR = 0x00000000; // Set APB1 Peripherals

    RCC->APB2RSTR = 0xFFFFFFFF; // Reset APB2 Peripherals
    RCC->APB2RSTR = 0x00000000; // Set APB2 Peripherals

    // Update variable SystemCoreClockUpdate with clock rate.
//  SystemCoreClockUpdate(); // CMSIS function for clock update

//  return;

/*
    // Enabling Power Clock to Enable Over-drive Mode
    RCC->APB1ENR |= RCC_APB1ENR_PWREN; // Power interface clock enable(for over-drive mode)
*/

/*
    // Enable external clock generator.
    RCC->CR |= RCC_CR_HSEBYP; // First enable HSE Clock BYPAS and then HSE Clock
    RCC->CR |= RCC_CR_HSEON; // Enable HSE clock
    while(!(RCC->CR & RCC_CR_HSERDY)); // Waiting for the flag that the BYPAS is ready
*/

    // Enable the internal resonator HSI
    RCC->CR |= RCC_CR_HSION; // Enable HSI clock
    while(!(RCC->CR & RCC_CR_HSIRDY)); // Waiting for the flag that the HSI Clock is ready

    // Reset PLL.
    RCC->CR &= ~RCC_CR_PLLON; // Reseting PLL
    while(RCC->CR & RCC_CR_PLLRDY); // Waiting for the flag PLLRDY that the PLL is unlocked

/*
    // Setting up prescalers for peripheral clock buses.
    // 1: divide by 1(AHBCLK = 216 MHz), 2: divide by 4(APBCLK1 = 54 MHz), 3: divide by 1(APBCLK2 = 104 MHz)
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1 | RCC_CFGR_PPRE1_DIV4 | RCC_CFGR_PPRE2_DIV2;
*/

    // Setting up prescalers for peripheral clock buses.
    // 1: divide by 1(AHBCLK = 32 MHz), 2: divide by 1(APBCLK1 = 32 MHz), 3: divide by 1(APBCLK2 = 32 MHz)
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1 | RCC_CFGR_PPRE1_DIV1 | RCC_CFGR_PPRE2_DIV1;

    // Setting up prescalers of PLL.
    uint32_t pllcfgr = 0;
/*
    pllcfgr |= RCC_PLLCFGR_PLLSRC_HSE; // External resonator used for PLL
    pllcfgr |= 25 << RCC_PLLCFGR_PLLM_Pos; // Divide by 25 for PLL(shift by the given mask, in this case division by M)
    pllcfgr |= 432 << RCC_PLLCFGR_PLLN_Pos; // Multiply by 432 for PLL(shift by the given mask, in this case multiplication by N)
    pllcfgr |= 0 << RCC_PLLCFGR_PLLP_Pos; // Divide by 2 for PLL(shift by the given mask, in this case division by P), see Reference Manual
*/

    pllcfgr |= RCC_PLLCFGR_PLLSRC_HSI; // Internal resonator used for PLL
    pllcfgr |= 8 << RCC_PLLCFGR_PLLM_Pos; // Divide by 8 for PLL(shift by the given mask, in this case division by M)
    pllcfgr |= 64 << RCC_PLLCFGR_PLLN_Pos; // Multiply by 64 for PLL(shift by the given mask, in this case multiplication by N)
    pllcfgr |= 1 << RCC_PLLCFGR_PLLP_Pos; // Divide by 4 for PLL(shift by the given mask, in this case division by P), see Reference Manual


    /*We write to the PLL configuration register not directly, since the reset value is not equal to zero, this register is initially set
     * to 16 MHz [the initial value is not equal to zero (Reset value)], so we get an incomprehensible value if we directly write the divisor
     * values, for this we created a separate variable uint32_t pllcfgr to write the desired value to the PLL configuration register */
    RCC->PLLCFGR = pllcfgr; // Writing to the PLL Configuration Register

    // Enable PLL.
    RCC->CR |= RCC_CR_PLLON; // Enable PLL
    while(!(RCC->CR & RCC_CR_PLLRDY)); // Waiting for the flag PLLRDY that the PLL is locked

    /* See datasheet and reference manual for STM32F746ZGT
     * Over-drive mode: This mode allows the CPU and the core logic to operate at a
     * higher frequency than the normal mode for the voltage scaling scale 1 and scale 2 */

/*
    // To work at a frequency of 216 MHz, you must enable the Over-drive mode, otherwise we will not be able to work at the maximum frequency
    PWR->CR1 |= PWR_CR1_ODEN; // enable the Over-drive mode
    while(!(PWR->CSR1 & PWR_CSR1_ODRDY)); // Waiting for over-drive enable flag
    PWR->CR1 |= PWR_CR1_ODSWEN; // Switching the voltage regulator to over-drive mode
    while(!(PWR->CSR1 & PWR_CSR1_ODSWRDY)); //Waiting for voltage regulator switch flag
*/

    /* For latency settings, see Reference Manual Table 5. Number of wait states according to CPU clock (HCLK) frequency*/

/*
    // Setting Latency on 7 for 216 MHz.
//    FLASH->ACR |= (7 << FLASH_ACR_LATENCY_Pos); // Setting the timeout when reading from Flash

//    while(!(RCC->CR & RCC_CR_PLLRDY)); // Waiting for the flag PLLRDY that the PLL is locked
*/

    // Setting Latency on 1 for 32 MHz.
    FLASH->ACR |= (1 << FLASH_ACR_LATENCY_Pos); // Setting the timeout when reading from Flash

    // Selecting PLL as system clock.
    RCC->CFGR |= RCC_CFGR_SW_PLL;

    SystemCoreClockUpdate();
}
