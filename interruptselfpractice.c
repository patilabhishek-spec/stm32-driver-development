/*
 * interruptselfpractice.c
 *
 *  Created on: Mar 19, 2026
 *      Author: Abhishek Patil
 */


#include<stdint.h>
#include<stdio.h>

#define SPI1_IRQNO    35
#define  USART1_IRQNO  37


uint32_t *pNVIC_ISERBase= (uint32_t*)0xE000E100;
uint32_t *pNVIC_ICERBase=(uint32_t*)0XE000E180;
uint32_t *pNVIC_ISPRBase=(uint32_t*)0XE000E200;
uint32_t *pNVIC_IPRBase=(uint32_t*)0xE000E400;


// NVIC ISER/ISPR logic:
// Each register handles 32 IRQs (1 bit per IRQ)
//
// (IRQNO / 32) → selects register (ISER0, ISER1, ...)
// (IRQNO % 32) → selects bit inside that register
//
// Example:
// IRQ 35 → ISER1 (35/32 = 1), bit 3 (35%32 = 3)
void interrupt_service(uint8_t irqno,uint8_t priority)
{
	uint32_t iprx=irqno/4;
	uint32_t *ipr=pNVIC_IPRBase+iprx;

	//2.position

	uint8_t pos=(irqno % 4) * 8;

	//3.configure the priority

	*ipr &=~(0xFF << pos);
	*ipr |=(priority << pos);






}



int main(void)
{
	interrupt_service(SPI1_IRQNO,0x80);
	interrupt_service(USART1_IRQNO,0x80);


	// NVIC ISER/ISPR logic:
	// Each register handles 32 IRQs (1 bit per IRQ)
	//
	// (IRQNO / 32) → selects register (ISER0, ISER1, ...)
	// (IRQNO % 32) → selects bit inside that register
	//
	// Example:
	// IRQ 35 → ISER1 (35/32 = 1), bit 3 (35%32 = 3)


	//1. manually pend the pending register
	*(pNVIC_ISPRBase + (USART1_IRQNO / 32)) |= (1 << (USART1_IRQNO % 32));
		*(pNVIC_ISPRBase + (SPI1_IRQNO / 32))  |= (1 << (SPI1_IRQNO % 32));



	*(pNVIC_ISERBase + (USART1_IRQNO / 32)) |= (1 << (USART1_IRQNO % 32));
	*(pNVIC_ISERBase + (SPI1_IRQNO / 32))  |= (1 << (SPI1_IRQNO % 32));
while(1);

}

	void SPI1_IRQHandler(void)
	{
		printf("SPI1");

	}



	void USART1_IRQHandler(void)
		{
			printf("usart1");

		}


