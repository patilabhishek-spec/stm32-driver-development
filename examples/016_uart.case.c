/*
 * 016_uart.case.c
 *
 *  Created on: Apr 1, 2026
 *      Author: Abhishek Patil
 */


#include<stdio.h>
#include<string.h>
#include "stm32f446xx.h"

//we have 3 different messeges that we are transmit to esp32
//you can by all means add more messeges

char *msg[3]={"hihihihihi123","hello how are you ?  .today is monday"};

//reply from esp stored here

char rx_buf[1024];

USART_Handle_t usart2_handle;

//this flag indicates reception completion

uint8_t rxCmplt;

uint8_t g_data=0;

extern void intialize_monitor_handle();

void USART2_Init(void)
{
	usart2_handle.pUSARTx=USART2;
		usart2_handle.USART_Config.USART_Baud=USART_STD_BAUD_2400;
		usart2_handle.USART_Config.USART_HWFlowControl=USART_HW_FLOW_CTRL_NONE;
		usart2_handle.USART_Config.USART_Mode=USART_MODE_ONLY_TX;
		usart2_handle.USART_Config.USART_NoOfStopBits=USART_STOPBITS_1;
		usart2_handle.USART_Config.USART_WordLength=USART_WORDLEN_8BITS;
		usart2_handle.USART_Config.USART_ParityControl=USART_PARITY_DISABLE;
		USART_Init(&usart2_handle);
}


void USART2_GPIO_Init(void)
{
	GPIO_Handle_t  Usart2pins;
	Usart2pins.pGPIOx=GPIOA;
	Usart2pins.GPIO_PinConfig.GPIO_PinMode=GPIO_MODE_ALTFN;
	Usart2pins.GPIO_PinConfig.GPIO_PinAltFunMode=7;
	Usart2pins.GPIO_PinConfig.GPIO_PinOPType=GPIO_OP_TYPE_PP;
	Usart2pins.GPIO_PinConfig.GPIO_PinPuPdControl=GPIO_PIN_PU;
	Usart2pins.GPIO_PinConfig.GPIO_PinSpeed=GPIO_SPEED_FAST;

	//USART2 RX
	Usart2pins.GPIO_PinConfig.GPIO_PinNumber=GPIO_PIN_NO_2;
	GPIO_Init(&Usart2pins);

	//USART2 RX
	Usart2pins.GPIO_PinConfig.GPIO_PinNumber=GPIO_PIN_NO_3;
	GPIO_Init(&Usart2pins);

}


void Button_GPIO_Init(void) {
    GPIO_Handle_t button,led;
    button.pGPIOx = GPIOC;
    button.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
    button.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
    button.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PD;  // or GPIO_PIN_PU depending on your button wiring
    GPIO_Init(&button);


    //this  is the led gpio configuration
    led.pGPIOx=GPIOA;
    led.GPIO_PinConfig.GPIO_PinNumber=GPIO_PIN_NO_5;
    led.GPIO_PinConfig.GPIO_PinMode=GPIO_MODE_OUT;
    led.GPIO_PinConfig.GPIO_PinSpeed=GPIO_SPEED_FAST;
    led.GPIO_PinConfig.GPIO_PinPuPdControl=GPIO_NO_PUPD;

    GPIO_PeriClockControl(GPIOA, ENABLE);
    GPIO_Init(&led);
}


void delay(void)
{
	for(uint32_t i=0;i<50000;i++);

}


int main(void)
{
	uint32_t cnt=0;

	intialise_monitor_handles();

	USART2_GPIO_Init();
	USART2_Init();

	USART_IRQInterruptConfig(IRQ_NO_USART2, ENABLE);

	USART_PeripheralControl(USART2, ENABLE);

	printf("Application is running\n");


	//do forever
	while(1)
	{

		//wait till button is pressed
		while(! GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_NO_13))
		{
			//wait
		}


		//to avoid button debouncing related issues 200 ms of the delay
		 delay();

		 //next msg index make suer that cnt value does'nt cross 2
		 cnt =cnt %3;



		//first lets enable the reception in interrupt mode
		 //this code enable the receive interrupt

		 while( USART_ReceiveDataIT(&usart2_handle, (uint8_t*)rx_buf, strlen(msg[cnt])) !=USART_READY);

		 //send the msg indexed by cnt in blocking mode
		 USART_SendData(&usart2_handle, (uint8_t*)msg[cnt], strlen(msg[cnt]));

		 printf("Transmitted: %s\n",msg[cnt]);

		 //now lets wait untill  all the bytes are received from the esp
		 //when all the bytes are received rxCmplt will be set in application callback
		 while(rxCmplt !=SET);

		 //just make sure that last byte should be null otherwise %s fails while printing
		 rx_buf[strlen(msg[cnt])+1]='\0';

		 //print what we received from the esp32
		 printf("Received  : %s\n",rx_buf);

		 //invalidate the flag

		 rxCmplt=RESET;

		 //move on next messege indexed in msg 1

		 cnt++;



	}


}



