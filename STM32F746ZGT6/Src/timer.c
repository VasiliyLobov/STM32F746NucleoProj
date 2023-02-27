
#include "timer.h"

void init_TIM8(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_TIM8EN; // Enable GPIOC port clock for TIM8CH1
	// Set prescaler of TIM8 equal to 1
	TIM8->PSC |= 0; // The counter clock frequency (CK_CNT) is equal to fCK_PSC / (PSC[15:0] + 1)
	/* Set the auto reload register to 1080 count, therefore we'll get frequency at 100 MHz
	 * we write 1080, because we doing PWM in center-aligned mode 2(triangle), therefore value 1080 is multiplied by 2*/
	TIM8->ARR |= 1080; // For TIM8 working at frequency 100 MHz we need to write in ARR value equal to 1080(216MHz/(1080*2)=100MHz)
	/* Set CMS1 in center-aligned mode 2 in channel 1 of TIM8 in timer control register 1
	 * 10: Center-aligned mode 2. The counter counts up and down alternatively.
	 * */
	TIM8->CR1 |= TIM_CR1_CMS_1; // Center-aligned mode 2 selection
	// Set compare output enable of channel 1 in TIM8 capture/compare enable register
	TIM8->CCER |= TIM_CCER_CC1E; // Enable Compare output at channel 1
	/* Set PWM mode of channel 1 in TIM8 capture/compare mode register 1
	 * 0110: PWM mode 1 - In upcounting, channel 1 is active as long as TIMx_CNT<TIMx_CCR1
	 * else inactive. In downcounting, channel 1 is inactive (OC1REF=‘0’) as long as
	 * TIMx_CNT>TIMx_CCR1 else active (OC1REF=’1’).
	 * Set PWM mode by setting 1 and 2 bit of OC1M
	 * */
	TIM8->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2;
	/* Set main output enable(MOE) function of channel 1 in TIM8 break and dead-time register,
	 * therefore we can see our PWM at output of PC6
	 */
	TIM8->BDTR |= TIM_BDTR_MOE;
	// Turn on channel 1 of TIM8 in timer control register 1 by set CEN bit
	TIM8->CR1 |= TIM_CR1_CEN;
}
