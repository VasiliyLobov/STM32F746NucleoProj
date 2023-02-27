#include "gpio.h"

void init_GPIO(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN; // Enable GPIOC port clock for TIM8CH1
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; // Enable GPIOB port clock
	init_GPIO_Output(GPIOB, 0);
	init_GPIO_AFunction(GPIOC, 6, 3);
}

void init_GPIO_Output(GPIO_TypeDef * gpio, unsigned int pin)
{
	// General output mode of GPIOB pin PB0
	gpio->MODER |= 1 << (1*pin); // shift by chosen pin of chosen GPIO
}

void init_GPIO_AFunction(GPIO_TypeDef * gpio, unsigned int pin, unsigned int AF)
{

	/* For enabling TIM8CH1 we need to write 0011=AF3 in AFR6(6 - means pin of GPIOC, therefore it means PC6)
	 * AFLR[7:0][3:0]: Alternate function selection for port x I/O pin y (y = 7 to 0)
	 * These bits are written by software to configure alternate function I/Os.
	 * */
	if(pin < 8){
		gpio->AFR[0] |= AF << (4*pin);
	}
	else
	/* Numeration of AFHR starts from 8, we need to take this into account to count from zero of this pin,
	 * therefore we need to subtract 8 of our pin number, because we might end up out of pin number*/
		gpio->AFR[1] |= AF << (4*(pin-8));
	// Set AFunction of GPIOC pin C6 to choose TIM8CH1
	gpio->MODER |= 2 << (2*pin); // shift of GPIO mode register by chosen pin of chosen GPIO
	gpio->OSPEEDR |= 3 << (2*pin); // shift of GPIO speed register by chosen pin of chosen GPIO
}
