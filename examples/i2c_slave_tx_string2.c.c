/*
 * 011i2c_master_rx_testing.c
 *
 *  Created on: Mar 20, 2026
 *      Author: Abhishek Patil
 */
  //this program is for the how we can fetch  more than 32 byte data from the slave or send to the slave

#include"stm32f446xx.h"
#include<stdio.h>
#include<stdint.h>
#include<string.h>

#define MY_ADDR   SLAVE_ADDR
//extern void intialize_monitor_handles();
#define SLAVE_ADDR  0x68  // to cheak the ack failure change this address and cheak


void delay(void)
{
   for(int i=0;i<50000;i++)	;
}


I2C_Handle_t I2C1Handle;

//rx buffer
uint8_t Tx_buf[32]= "STM32 Slave Mode Testing";

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



	//intialize_monitor_handles();

	printf("Application is runnig\n");
	I2C1_GPIOInit();



	I2C1_ButtonInit();
	I2C1_Init();

	//I2C IRQ configuration
	I2C_IRQInterruptConfig(IRQ_NO_I2C1_EV, ENABLE);
	I2C_IRQInterruptConfig(IRQ_NO_I2C1_ER, ENABLE);

	I2C_SlaveEnbleDisbleCallbackEvents(I2C1,ENABLE);

	I2C_PeriClockControl(I2C1, ENABLE);

	//ACK is enable 1 after pe =1
	I2C_ManageAcking(I2C1, ENABLE);

while(1);

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
	static uint8_t commandcode=0;//static variables are kind of global variables memory for theses varible is not allocate into the stack
	static uint8_t cnt=0;         // memory allocate in global space
	//static means private to the function
	if(AppEv == I2C_EV_DATA_REQ)
	{
	// master wants some data slave has to send it
		if(commandcode==0x51)
		{
			//send the length information to the master
			I2C_SlaveSendData(pI2CHandle->pI2Cx, strlen((char*)Tx_buf));
		}else if (commandcode == 0x52)
		{
			//send the content of tx buffer
			I2C_SlaveSendData(pI2CHandle->pI2Cx, Tx_buf[cnt++]);

		}

	}else if (AppEv==I2C_EV_DATA_RCV)
	{
		//data is waiting for the slave to read,slave has to read it
		commandcode=I2C_SlaveReceiveData(pI2CHandle->pI2Cx);

	}else if(AppEv == I2C_ERROR_AF)
	{
		// this happens during slave transmission
		//master has sent the nack so slave should understand that master doesnt need more data
		commandcode=0xff; //it is invalidating
		cnt =0;  //rst

	}else if(AppEv==I2C_EV_STOP)
	{
		//this happens only during slave reception
		//master has ended the I2C communication with slave
	}




}


