/*
 * 011i2c_master_rx_testing.c
 *
 *  Created on: Mar 20, 2026
 *      Author: Abhishek Patil
 */


#include"stm32f446xx.h"
#include<stdio.h>
#include<stdint.h>

uint8_t rxComplt =RESET;

//extern void intialize_monitor_handles();
#define MY_ADDR  0x61
#define SLAVE_ADDR  0x68  // to cheak the ack failure change this address and cheak



void delay(void)
{
   for(int i=0;i<50000;i++)	;
}


I2C_Handle_t I2C1Handle;

//rx buffer
uint8_t rcv_buf[32];

void I2C1_GPIOInit(void)
{
	GPIO_Handle_t I2CPins;
	I2CPins.pGPIOx=GPIOB;
	I2CPins.GPIO_PinConfig.GPIO_PinMode=GPIO_MODE_ALTFN;
	I2CPins.GPIO_PinConfig.GPIO_PinOPType=GPIO_OP_TYPE_OD;
	I2CPins.GPIO_PinConfig.GPIO_PinPuPdControl=GPIO_PIN_PU;
	I2CPins.GPIO_PinConfig.GPIO_PinSpeed=GPIO_SPEED_FAST;
	I2CPins.GPIO_PinConfig.GPIO_PinAltFunMode=4;

	//scl
	I2CPins.GPIO_PinConfig.GPIO_PinNumber=GPIO_PIN_NO_7;
	GPIO_Init(&I2CPins);

	//sda
	I2CPins.GPIO_PinConfig.GPIO_PinNumber=GPIO_PIN_NO_9;
	GPIO_Init(&I2CPins);

}



void I2C1_Init(void)
{
	I2C1Handle.pI2Cx=I2C1;
	I2C1Handle.I2C_Config.I2C_AckControl=I2C_ACK_ENABLE;
	I2C1Handle.I2C_Config.I2C_DeviceAddress=MY_ADDR;
	I2C1Handle.I2C_Config.I2C_FMDutyCycle=I2C_FM_DUTY_2;
	I2C1Handle.I2C_Config.I2C_SCL_Speed=I2C_SCL_SPEED_SM;

	I2C_Init(&I2C1Handle);
}


void I2C1_ButtonInit(void){

	GPIO_Handle_t GPIOButton;
	GPIOButton.pGPIOx=GPIOC;
	GPIOButton.GPIO_PinConfig.GPIO_PinNumber=GPIO_PIN_NO_13;
	GPIOButton.GPIO_PinConfig.GPIO_PinMode=GPIO_MODE_IN;
	GPIOButton.GPIO_PinConfig.GPIO_PinSpeed=GPIO_SPEED_FAST;
	GPIOButton.GPIO_PinConfig.GPIO_PinPuPdControl=GPIO_NO_PUPD;

	GPIO_Init(&GPIOButton);

	// this is led gpio configuration



}


int main()
{

	uint8_t commandcode;

	uint8_t len;

	//intialize_monitor_handles();

	printf("Application is runnig\n");



	I2C1_ButtonInit();
	I2C1_Init();

	//I2C IRQ configuration
	I2C_IRQInterruptConfig(IRQ_NO_I2C1_EV, ENABLE);
	I2C_IRQInterruptConfig(IRQ_NO_I2C1_ER, ENABLE);


	I2C1_GPIOInit();

	I2C_PeriClockControl(I2C1, ENABLE);

	//ACK is enable 1 after pe =1
	I2C_ManageAcking(I2C1, ENABLE);


	while(1)
	{
		//wait till button is pressed

		while(! GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_13));

		//to avoid the button debouncing
		delay();

		commandcode=0x51;

		while(I2C_MasterSendDataIT(&I2C1Handle, &commandcode, 1, SLAVE_ADDR,I2C_ENABLE_SR)!=I2C_READY);

		while(I2C_MasterReceiveDataIT(&I2C1Handle, &len, 1, SLAVE_ADDR,I2C_ENABLE_SR)!=I2C_READY);

		commandcode=0x52;

		while(I2C_MasterSendDataIT(&I2C1Handle, &commandcode, 1, SLAVE_ADDR,I2C_ENABLE_SR)!=I2C_READY);

		while(I2C_MasterReceiveDataIT(&I2C1Handle, rcv_buf, len, SLAVE_ADDR,I2C_DISABLE_SR)!=I2C_READY);

	      rxComplt=RESET;


		//wait till rx complete

		while(rxComplt != SET);

		rcv_buf[len+1]='\0';
      printf("data: %s ",rcv_buf);

      rxComplt=RESET;

	}


}



void I2C1_EV_IRQHandler (void)
{
	I2C_EV_IRQHandling(&I2C1Handle);
}


void I2C1_ER_IRQHandler(void)
{
	I2C_ER_IRQHandling(&I2C1Handle);
}


void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle, uint8_t AppEv)
{
	if(AppEv == I2C_EV_TX_CMPLT)
	{
		printf("Tx is completed\n");
	}else if(AppEv == I2C_EV_RX_CMPLT)
	{
		printf("Rx is completed\n");
		rxComplt=SET;
	}else if(AppEv == I2C_ERROR_AF)
	{
		printf("ERROR: Ack failure\n");
		//in master ack failure happens when slave fails to send ack for the byte
		//send from the master
		I2C_CloseSendData(pI2CHandle);

		//genrate the stop condition to release the bus
		 I2C_GenerateStopCondition(I2C1);

		 // hang in infinite loop
		 while(1);
	}

}
