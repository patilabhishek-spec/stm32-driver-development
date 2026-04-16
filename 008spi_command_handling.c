/*
 * 006spi_tx_testing.c
 *
 *  Created on: Feb 14, 2026
 *      Author: Abhishek Patil
 *

 */
#include<string.h>
#include "stm32f446xx.h"





 void delay (void)
  {
	  for(uint32_t i=0; i<500000; i++);

  }

/*
 * PB14 ---> SPI2_MISO
 * PB15----> SPI2_MOSI
 * PB13----> SPI2_SCLK
 * PB12----> SPI2_NSS
 * ALT FUNCTION MODE
 */




	//THIS FUNCTION IS USED TO INTIALIZE THE GPIO PINS TO BEHAVE AS SPI2 PINS
	 void SPI2_GPIOInits(void)
		{

		GPIO_Handle_t SPIPins;
		SPIPins.pGPIOx =GPIOB;
		SPIPins.GPIO_PinConfig.GPIO_PinMode =GPIO_MODE_ALTFN;
		SPIPins. GPIO_PinConfig.GPIO_PinAltFunMode=5;
		SPIPins.GPIO_PinConfig.GPIO_PinOPType=GPIO_OP_TYPE_PP;
		SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl=GPIO_NO_PUPD;
		SPIPins.GPIO_PinConfig.GPIO_PinSpeed=GPIO_SPEED_FAST;


		//SCLK
		SPIPins.GPIO_PinConfig.GPIO_PinNumber=GPIO_PIN_NO_13;
		GPIO_Init(&SPIPins);

		//MOSI
		SPIPins.GPIO_PinConfig.GPIO_PinNumber=GPIO_PIN_NO_15;
		GPIO_Init(&SPIPins);

		//MISO
		SPIPins.GPIO_PinConfig.GPIO_PinNumber=GPIO_PIN_NO_14;
		GPIO_Init(&SPIPins);

		//NSS
		SPIPins.GPIO_PinConfig.GPIO_PinNumber=GPIO_PIN_NO_12;
		GPIO_Init(&SPIPins);

		}

	 void SPI2_Inits(void)
	 {
		 SPI_Handle_t SPI2handle;
		 SPI2handle.pSPIx =SPI2;
		 SPI2handle.SPIConfig.SPI_BusConfig=SPI_BUS_CONFIG_FD;
		 SPI2handle.SPIConfig.SPI_DeviceMode=SPI_DEVICE_MODE_MASTER;
		 SPI2handle.SPIConfig.SPI_SclkSpeed= SPI_SCLK_SPEED_DIV128;//GENARATES THE CLOCK OF 8MHZ
		 SPI2handle.SPIConfig.SPI_DFF =SPI_DFF_8BITS;
		 SPI2handle.SPIConfig.SPI_CPOL =SPI_CPOL_LOW;
		 SPI2handle.SPIConfig.SPI_CPHA  =SPI_CPHA_LOW;
		 SPI2handle.SPIConfig.SPI_SSM = SPI_SSM_DI;   // software NSS

		 SPI_Init(&SPI2handle);


	 }


void GPIO_ButtonInit(void)
{
	GPIO_Handle_t GpioButton;
	GpioButton.pGPIOx=GPIOC;
			GpioButton.GPIO_PinConfig.GPIO_PinNumber=GPIO_PIN_NO_13;
			GpioButton.GPIO_PinConfig.GPIO_PinMode=GPIO_MODE_IN;
			GpioButton.GPIO_PinConfig.GPIO_PinSpeed=GPIO_SPEED_FAST;
			GpioButton.GPIO_PinConfig.GPIO_PinPuPdControl=GPIO_NO_PUPD;
			GPIO_Init(&GpioButton);

}

  uint8_t SPI_VerifyResponse(uint8_t ackbyte)
  {
	  if(ackbyte == 0xF5)
	  {
		  return 1;
	  }
	  return 0;
  }






		int main(void)
		{


			    printf("Application is running\n");

			char user_data[] = "Hello world";
           uint8_t  dummy_write =0xff;
           uint8_t  dummy_read =0xff;




			GPIO_ButtonInit();


			//THIS FUNCTION IS USED TO INIATILIZE THE GPIO PINS TO BEHAVE AS SPI2 PINS
			SPI2_GPIOInits();


			//this function is used to intialize the spi2 peripheral parameters
			SPI2_Inits();

	           printf("SPI INIt is done\n");


			SPI_SSOEConfig(SPI2, ENABLE);


			//ENABLE THE SPI2 PERIPHERAL
			SPI_PeripheralControl(SPI2, ENABLE);
			delay();


			while(1)
			{

				while(!GPIO_ReadFromInputPin(GPIOC, 13));

	            delay();




		     	//1.cmd_led_ctrl

		     	uint8_t commndcode =COMMAND_LED_CTRL;
		       uint8_t ackbyte;
		       uint8_t args[2];



		       //send command
		     	SPI_SendData(SPI2, &commndcode, 1);


		     	//do dummy read to clear off the rxne
                SPI_ReceiveData(SPI2, &dummy_read, 1);


		     	//send some dummy bits(byte) to fetch response from slave
		     	SPI_SendData(SPI2, &dummy_write, 1);

		     	//read the ack byte received
                SPI_ReceiveData(SPI2, &ackbyte, 1);


		     if	(SPI_VerifyResponse(ackbyte))
				{

		    	 args[0]=LED_PIN;
		    	 args[1]=LED_ON;

		    	 //send arguments
			     SPI_SendData(SPI2, args, 2);
		           printf("command control executed \n");


			     SPI_ReceiveData(SPI2, &dummy_read, 1);
			     SPI_ReceiveData(SPI2, &dummy_read, 1);

				}





		     	//1st send lenght information
		     	uint8_t dataLen =strlen(user_data);
				   SPI_SendData(SPI2,(uint8_t*)&dataLen,1);


               //TO SEND THE DATA
			   SPI_SendData(SPI2,(uint8_t*)user_data,strlen(user_data));

			   //LETS CONFIRM SPI IS NOT BUSY
			while ( SPI_GetFlagStatus(SPI2, SPI_BUSY_FLAG) )
			{

			}



			}


	return 0;
}
