/*
 * interrupt_priority.c
 *
 *  Created on: Mar 19, 2026
 *      Author: Abhishek Patil
 */


#include<stdio.h>
#include<stdint.h>

#define IRQNO_TIMER   28
#define IRQNO_I2C1    31

uint32_t *pNVIC_ISPRBase =(uint32_t*)0xE000E200;
uint32_t *pNVIC_ISERBase=(uint32_t*)0xE000E100;
uint32_t *pNVIC_IPRBase=(uint32_t*)0xE000E400;
uint32_t *pNVIC_ICER =(uint32_t*)0XE000E184;




void configure_priority_for_irqs(uint8_t irq_no,uint8_t priority_value)
{
	//1. find out iprx

	uint8_t iprx = irq_no /4;
	uint32_t *ipr = pNVIC_IPRBase + iprx;

	//2.position in iprx

	uint8_t pos =(irq_no % 4) * 8;

	//3. configure the priority
	*ipr &=~(0xFF << pos);  //clear
	*ipr |=(priority_value << pos);  //Set



}




int main(void)
{

	//1.configure the priority for the peripherals
	configure_priority_for_irqs(IRQNO_TIMER,0x80);
	configure_priority_for_irqs(IRQNO_I2C1,0x70);


	//1.Manually pend the pending bit for the  IRQ number in NVIC

		*pNVIC_ISPRBase |=(1 << (IRQNO_TIMER % 32));

		*pNVIC_ISPRBase |=(1 << (IRQNO_I2C1 % 32));

		//2.Enable the  IRQ number in NVIC

		*pNVIC_ISERBase |= ( 1 <<( IRQNO_TIMER % 32));

		*pNVIC_ISERBase |= ( 1 <<( IRQNO_I2C1 % 32));



		for(;;);

	}
	void TIM2_IRQHandler(void)
	{
		printf("in USART3 isr\n");
		*pNVIC_ISERBase |= ( 1 <<( IRQNO_I2C1 % 32));
        while(1);


		/*uint32_t *pNVIC_ICER =(uint32_t*)0XE000E184;

			*pNVIC_ICER |=(1 << (IRQNO_TIMER % 32));*/


	}


	void I2C1_EV_IRQHandler(void)
		{
			printf("in USART3 isr\n");

			/*uint32_t *pNVIC_ICER =(uint32_t*)0XE000E184;

				*pNVIC_ICER |=(1 << (IRQNO_I2C1 % 32));
*/

		}


