
// Смотри файл stm32f476xx.h для настройки RCC и PLL, а также STM32CubeMX, так как в нем удобно изображена структура настройки PLL
#include "stm32f7xx.h"

#include "rcc.h"

void init_RCC(void)
{
    // Сброс периферии.
    RCC->AHB1RSTR = 0xFFFFFFFF; // Сброс
    RCC->AHB1RSTR = 0x00000000; // Установка

    RCC->AHB2RSTR = 0xFFFFFFFF; // Сброс
    RCC->AHB2RSTR = 0x00000000; // Установка

    RCC->APB1RSTR = 0xFFFFFFFF; // Сброс
    RCC->APB1RSTR = 0x00000000; // Установка

    RCC->APB2RSTR = 0xFFFFFFFF; // Сброс
    RCC->APB2RSTR = 0x00000000; // Установка

    // Обновление переменной с частотой тактирования.
//    SystemCoreClockUpdate(); // Функция CMSIS для обновления тактирования

//  return;

    RCC->APB1ENR |= RCC_APB1ENR_PWREN; // Power interface clock enable

    // Включение внешнего тактового генератора.
    RCC->CR |= RCC_CR_HSEBYP; // Включение именно генератора внешнего, а не осцилятора внешнего
    RCC->CR |= RCC_CR_HSEON; // Включение работы от внешнего генератора
    while(!(RCC->CR & RCC_CR_HSERDY)); // Ожидание флага, что внешний генератор готов

    // Выключение PLL.
    RCC->CR &= ~RCC_CR_PLLON; // Выключение PLL
    while(RCC->CR & RCC_CR_PLLRDY); // Ожидание флага выключения PLL

    // Настройка предделителей для шин тактирования периферии.
    // 1: ни на что не делим, 2: делим на 4(PCLK1 = 54 MГц), 3: делим на 2(PCLK2 = 108 MГц)
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1 | RCC_CFGR_PPRE1_DIV4 | RCC_CFGR_PPRE2_DIV2;

    // Настройка предделителей PLL.
    uint32_t pllcfgr = 0;

    pllcfgr |= RCC_PLLCFGR_PLLSRC_HSE; // Внешний резонатор используется для PLL
    pllcfgr |= 25 << RCC_PLLCFGR_PLLM_Pos; // Деление на 25 для PLLM(сдвиг на маску заданную, в данном случае деление на M)
    pllcfgr |= 432 << RCC_PLLCFGR_PLLN_Pos; // Умножение на 432 для PLLM(сдвиг на маску заданную, в данном случае умножение на N)
    pllcfgr |= 0 << RCC_PLLCFGR_PLLP_Pos; // Деление на 2 для PLLM(сдвиг на маску заданную, в данном случае деление на P), смотри в Reference Manual

    /*Осуществляем запись в регистр конфигурации PLL не напрямую, так как заданное значение не равно нулю, данный регистр изанчально настроен
     * на 16МГц, поэтому получится каша, если напрямую будем записывать значения делителей, для этого создали отдельную переменную, чтобы
     * записать нужное значение в регистр конфигурации PLL */
    RCC->PLLCFGR = pllcfgr; // Запись в регистр конфигурации PLL

    // Включение PLL.
    RCC->CR |= RCC_CR_PLLON; // Включение PLL
    while(!(RCC->CR & RCC_CR_PLLRDY)); // Ожидание флага включения PLL

    /* Смотри даташит и reference manual на STM32F746ZGT
     * Over-drive mode: This mode allows the CPU and the core logic to operate at a
     * higher frequency than the normal mode for the voltage scaling scale 1 and scale 2 */
    // Для рыботы на частоте 216Мгц надо включить режим Over-Drive, иначе не сможем работать на максимальной частоте
    PWR->CR1 |= PWR_CR1_ODEN; // Включаем овердрайв
    while(!(PWR->CSR1 & PWR_CSR1_ODRDY)); // Ожидаем флаг включения овердрайва
    PWR->CR1 |= PWR_CR1_ODSWEN; // Переключаем стабилизатор напряжения из нормального в режим овердрайв
    while(!(PWR->CSR1 & PWR_CSR1_ODSWRDY)); // Ожидаем флаг переключения стабилизатора напряжения

    /* Для настройки Latency смотри в Reference Manual Table 5. Number of wait states according to CPU clock (HCLK) frequency*/
    // Настраиваем Latency на 7 для 216 МГц.
    FLASH->ACR |= (7 << FLASH_ACR_LATENCY_Pos); // Настройка времени ожидания при чтении из Flash

    while(!(RCC->CR & RCC_CR_PLLRDY)); // Ожидание флага включения PLL

    // Выбор PLL как основного источника тактирования.
    RCC->CFGR |= RCC_CFGR_SW_PLL;

    SystemCoreClockUpdate();
}
