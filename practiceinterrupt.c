/*
 * practiceinterrupt.c
 *
 *  Created on: Mar 19, 2026
 *      Author: Abhishek Patil
 */


#include<stdint.h>
#include<stdio.h>
#define USART3_IRQNO   39

int main(void)
{
	//1.Manually pend the pending bit for the USART3 IRQ number in NVIC
	uint32_t *pISPR1 =(uint32_t*)0xE000E204;

	*pISPR1 |=(1 << (USART3_IRQNO % 32));

	//2.Enable the USART3 IRQ number in NVIC
	uint32_t *ISER1=(uint32_t*)0xE000E104;

	*ISER1 |= ( 1 <<( USART3_IRQNO % 32));



	for(;;);

}
void USART3_IRQHandler(void)
{
	printf("in USART3 isr\n");

	uint32_t *pICER1 =(uint32_t*)0XE000E184;

		*pICER1 |=(1 << (USART3_IRQNO % 32));


}
