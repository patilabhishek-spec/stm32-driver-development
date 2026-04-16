/*
 * 002button_interrrupt.c
 *
 *  Created on: Feb 9, 2026
 *      Author: Abhishek Patil
 */

#include<string.h>
#include "stm32f446xx.h"

#define HIGH  1
#define LOW  0

//this will introduce ~200ms delay  when system clock is 16mhz
  void delay (void)
  {
	  for(uint32_t i=0; i<500000; i++);

  }

int main(void)
{
	GPIO_Handle_t GpioLed,GpioButton;
	memset(&GpioLed,0,sizeof(GpioLed));
	memset(&GpioButton,0,sizeof(GpioButton));


	GpioLed.pGPIOx=GPIOB;
	GpioLed.GPIO_PinConfig.GPIO_PinNumber=GPIO_PIN_NO_0;
	GpioLed.GPIO_PinConfig.GPIO_PinMode=GPIO_MODE_OUT;
	GpioLed.GPIO_PinConfig.GPIO_PinSpeed=GPIO_SPEED_FAST;
	GpioLed.GPIO_PinConfig.GPIO_PinOPType=GPIO_OP_TYPE_PP;
	GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl=GPIO_NO_PUPD;

	GPIO_PeriClockControl(GPIOB, ENABLE);

	GPIO_Init(&GpioLed);
	SYSCFG_PCLK_EN();


		GpioButton.pGPIOx=GPIOC;
		GpioButton.GPIO_PinConfig.GPIO_PinNumber=GPIO_PIN_NO_13;
		GpioButton.GPIO_PinConfig.GPIO_PinMode=GPIO_MODE_IT_FT;
		GpioButton.GPIO_PinConfig.GPIO_PinSpeed=GPIO_SPEED_FAST;
		GpioButton.GPIO_PinConfig.GPIO_PinPuPdControl=GPIO_NO_PUPD;
		GPIO_PeriClockControl(GPIOC, ENABLE);
		GPIO_Init(&GpioButton);


		//IRQ configurations

		GPIO_IRQInterruptConfig(IRQ_NO_EXTI15_10, ENABLE);

		//CONGIGURE IRQ PRORITY

		GPIO_IRQPriorityConfig(IRQ_NO_EXTI15_10, NVIC_IRQ_PRI15);



while(1);

}

  void EXTI15_10_IRQHandler(void)
  {
	  GPIO_IRQHandling(GPIO_PIN_NO_13);    //clear the pending event from exti line
	  GPIO_ToggleOutputPin(GPIOB, GPIO_PIN_NO_0);
  }

