/*
 * 015usart_tx.c
 *
 *  Created on: Mar 30, 2026
 *      Author: Abhishek Patil
 */


#include "stm32f446xx.h"
#include<string.h>

void delay()
{
	for(int i=0;i<50000;i++);
}
char msg[1024] = "UART Tx is succesfull sfter 4 hrs...\n\r";

USART_Handle_t usart2_handle;
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
    GPIO_Handle_t button;
    button.pGPIOx = GPIOC;
    button.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
    button.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
    button.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PD;  // or GPIO_PIN_PU depending on your button wiring
    GPIO_Init(&button);
}





int main(void)
{
    USART_PeriClockControl(USART2,ENABLE);
    GPIO_PeriClockControl(GPIOA, ENABLE);

    USART2_GPIO_Init();
    Button_GPIO_Init();   // 🔥 ADD THIS

    USART2_Init();
    USART_PeripheralControl(USART2, ENABLE);

    while (1)
    {
        while (!GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_NO_13));

        delay();

        USART_SendData(&usart2_handle, (uint8_t*) msg, strlen(msg));
    }
}
