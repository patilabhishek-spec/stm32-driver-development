/*
 * practicedriver.c
 *
 *  Created on: Mar 19, 2026
 *      Author: Abhishek Patil
 */


#include<stdio.h>
#include "stm32f446xx.h"

void delay()
{
	for(int i=0;i<50000;i++);

}

int main()
{
	GPIO_Handle_t  Gpioled,Gpiobutton;
	Gpioled.pGPIOx=GPIOA;
	Gpioled.GPIO_PinConfig.GPIO_PinNumber=GPIO_PIN_NO_5;
	Gpioled.GPIO_PinConfig.GPIO_PinMode= GPIO_MODE_OUT;
	Gpioled.GPIO_PinConfig.GPIO_PinSpeed=GPIO_SPEED_FAST;
	Gpioled.GPIO_PinConfig.GPIO_PinOPType=GPIO_OP_TYPE_PP;
	Gpioled.GPIO_PinConfig.GPIO_PinPuPdControl=GPIO_NO_PUPD;

	GPIO_PeriClockControl(GPIOA, ENABLE);
	GPIO_Init(&Gpioled);



	Gpiobutton.pGPIOx=GPIOC;
	Gpiobutton.GPIO_PinConfig.GPIO_PinNumber=GPIO_PIN_NO_13;
	Gpiobutton.GPIO_PinConfig.GPIO_PinMode=GPIO_MODE_IT_FT;
	Gpiobutton.GPIO_PinConfig.GPIO_PinSpeed=GPIO_SPEED_FAST;
	Gpiobutton.GPIO_PinConfig.GPIO_PinPuPdControl=GPIO_NO_PUPD;

	GPIO_PeriClockControl(GPIOC, ENABLE);
	GPIO_Init(&Gpiobutton);

	//syscnfig enable
	SYSCFG_PCLK_EN();


	//IRQ CONFIGURATION

	GPIO_IRQInterruptConfig(IRQ_NO_EXTI15_10, ENABLE);

	// configuring the irq priority

	GPIO_IRQPriorityConfig(IRQ_NO_EXTI15_10, NVIC_IRQ_PRI15);

	while(1);




}


void EXTI15_10_IRQHandler(void)
  {
delay();
	  GPIO_IRQHandling(GPIO_PIN_NO_13);    //clear the pending event from exti line

	if(  GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_NO_13)==0){

	  GPIO_ToggleOutputPin(GPIOA, GPIO_PIN_NO_5);
	  }
  }
