/*
 * stm32f446xx_gpio_driver.c
 *
 *  Created on: Feb 4, 2026
 *      Author: Abhishek Patil
 */


#include "stm32f446xx_gpio_driver.h"





/************************************************************************

 @fn                  -   GPIO_PeriClockControl


  @Brief              -  this function enable or disable peripheral clock for given GPIO port


@param[in]            - base address of  the gpio
@param[in]            - enable or disable macros
@param[in]            -


@return               - none



@note                 - none

//peripheral clock setup


*/
void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx,uint8_t EnorDi)
{
	if(EnorDi==ENABLE)
	{
		if(pGPIOx==GPIOA)
		{
			GPIOA_PCLK_EN();
		} else if(pGPIOx==GPIOB)
		{
			GPIOB_PCLK_EN();
		}else if(pGPIOx==GPIOC)
		{
			GPIOC_PCLK_EN();
		}else if(pGPIOx==GPIOD)
		{
			GPIOD_PCLK_EN();
		}else if(pGPIOx==GPIOE)
		{
			GPIOE_PCLK_EN();
		}else if(pGPIOx==GPIOF)
		{
			GPIOF_PCLK_EN();
		}else if(pGPIOx==GPIOG)
		{
			GPIOG_PCLK_EN();
		}else if(pGPIOx==GPIOH)
		{
			GPIOH_PCLK_EN();
		}

	}
	else
	{
		if(pGPIOx==GPIOA)
				{
					GPIOA_PCLK_DI();
				} else if(pGPIOx==GPIOB)
				{
					GPIOB_PCLK_DI();
				}else if(pGPIOx==GPIOC)
				{
					GPIOC_PCLK_DI();
				}else if(pGPIOx==GPIOD)
				{
					GPIOD_PCLK_DI();
				}else if(pGPIOx==GPIOE)
				{
					GPIOE_PCLK_DI();
				}else if(pGPIOx==GPIOF)
				{
					GPIOF_PCLK_DI();
				}else if(pGPIOx==GPIOG)
				{
					GPIOG_PCLK_DI();
				}else if(pGPIOx==GPIOH)
				{
					GPIOH_PCLK_DI();
				}

	}

}



//init and de-init


/**
 *
 * This handle contains:

Which port (GPIOA, GPIOB, …)

Which pin

How the pin should behave
 *
 *
 * @fn                  GPIO_Init
 *
 * @brief               Initializes and configures the GPIO pin according to
 *                      the parameters specified in the GPIO handle
 *
 * @param[in]            pGPIOHandle   Pointer to GPIO handle structure which
 *                                     contains GPIO port base address and
 *                                     pin configuration information
 *
 * @return               None
 *
 * @note                 Before calling this function, the peripheral clock
 *                       for the corresponding GPIO port must be enabled
 *                       using GPIO_PeriClockControl().
 */

void GPIO_Init(GPIO_Handle_t *pGPIOHandle)
{
	uint32_t temp=0;
	GPIO_PeriClockControl(pGPIOHandle->pGPIOx, ENABLE);

	//ENABLE THE PERIPHERAL CLOCK FOR GPIO


	if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_ANALOG)
	{
		//the non interrupt mode//this is for the mode register  multiply * 2 beacuse each bit of 2 pins
		temp=(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <<(2* pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
		pGPIOHandle->pGPIOx->MODER &= ~(0x3 << (2* pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber) ); //clearing
		pGPIOHandle->pGPIOx->MODER|=temp; //setting


	}else
	{

		//this part will be code later (interrupt mode)

		if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode==GPIO_MODE_IT_FT)
		{
			//1 configure the falling edge trigger register

			EXTI->FTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber );

			//clear the RTSR
			EXTI->RTSR &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber );




		}
		else if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode==GPIO_MODE_IT_RT)
		{

			//1 configure the rising edge trigger register
			EXTI->RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber );


			//CLEAR THE FTSR
			EXTI->FTSR &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber );


		}
		else if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode==GPIO_MODE_IT_RFT)
		{
			//consider both rising edge and falling edge

			EXTI->FTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber );
			EXTI->RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber );


		}

		//2 configure the GPIO port selection in  SYSCFG_EXTICR

		/*
		 * EXTI PORT MAPPING USING SYSCFG EXTICR
		 *
		 * Purpose:
		 * STM32 EXTI lines (0–15) are shared across all GPIO ports.
		 * Example: EXTI0 can come from PA0, PB0, PC0, etc.
		 * So we must tell MCU WHICH PORT is connected to that EXTI line.
		 * This mapping is done using SYSCFG->EXTICR registers.
		 *
		 * EXTICR Layout:
		 * EXTICR[0] → EXTI0–3
		 * EXTICR[1] → EXTI4–7
		 * EXTICR[2] → EXTI8–11
		 * EXTICR[3] → EXTI12–15
		 *
		 * Each EXTI line uses 4 bits to store port code:
		 * GPIOA = 0, GPIOB = 1, GPIOC = 2, etc.
		 *
		 * temp1 = pin / 4  → selects which EXTICR register
		 * temp2 = pin % 4  → selects position inside that register
		 *
		 * We shift by (temp2 * 4) because each EXTI line occupies 4 bits.
		 *
		 * Example:
		 * For PB6 interrupt:
		 * pin = 6
		 * temp1 = 1 → EXTICR[1]
		 * temp2 = 2 → shift by 8 bits
		 * portcode = 1 (for GPIOB)
		 * So PB6 gets mapped to EXTI6.
		 *
		 * Without this mapping, EXTI interrupt will NOT trigger,
		 * because EXTI won't know which GPIO port generated the interrupt.
		 */



		uint8_t temp1=pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber /4;
		uint8_t temp2=pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber %4;
		uint8_t portcode=GPIO_BASEADDR_TO_CODE(pGPIOHandle->pGPIOx);
		SYSCFG_PCLK_EN();
        SYSCFG->EXTICR[temp1]=portcode << (temp2 * 4);

		//3. enable the exti interrupt delivery using interrupt mask register
           EXTI->IMR |= 1 <<pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber;

	}


	temp=0;

	/*2.configure the speed register
	Why speed matters?

Used for:
EMI control
High-frequency signals
Power consumption
Examples:
LED → low speed
SPI/UART → high speed
*/
	temp=(pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed << (2* pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->OSPEEDR &= ~(0x3 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber );  //clearing
	pGPIOHandle->pGPIOx->OSPEEDR |=temp;  //setting

	temp=0;


	//3.CONFIGURE THE PUPD SETTINGS

	temp=(pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdControl << (2* pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->PUPDR &= ~(0x3 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber );  //clearing
	pGPIOHandle->pGPIOx->PUPDR |=temp; //setting

		temp=0;


		//configure the output type

		temp=(pGPIOHandle->GPIO_PinConfig.GPIO_PinOPType <<  pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		pGPIOHandle->pGPIOx->OTYPER &= ~(0x1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber );  //clearing
		pGPIOHandle->pGPIOx->OTYPER |=temp;




				//configure the alternate functanily  mode


		/*
		 * GPIO ALTERNATE FUNCTION CONFIGURATION (AFR REGISTER)
		 *
		 * Purpose:
		 * Used when a GPIO pin must work as a peripheral pin
		 * (UART, SPI, I2C, TIM, CAN, etc.) instead of normal GPIO.
		 *
		 * STM32 Requirement:
		 * 1) Set MODER = Alternate mode
		 * 2) Select WHICH peripheral using AFR register
		 *
		 * AFR Layout:
		 * GPIOx_AFR[0] → Pins 0–7
		 * GPIOx_AFR[1] → Pins 8–15
		 *
		 * Each pin uses 4 bits because AF numbers range from AF0–AF15.
		 *
		 * temp1 = pin / 8
		 * → Selects which AFR register to use
		 *   0 → AFR[0] (pins 0–7)
		 *   1 → AFR[1] (pins 8–15)
		 *
		 * temp2 = pin % 8
		 * → Position of the pin inside that AFR register
		 *
		 * We shift by (4 * temp2) because:
		 * Each pin occupies 4 bits in AFR.
		 *
		 * Why 0xF?
		 * 0xF = 1111 (4-bit mask)
		 * Used to clear the 4-bit AF field before writing new AF value.
		 *
		 * Steps:
		 * 1) Clear old AF bits
		 *    AFR[temp1] &= ~(0xF << (4 * temp2));
		 *
		 * 2) Set new alternate function number
		 *    AFR[temp1] |= (AltFunMode << (4 * temp2));
		 *
		 * Example:
		 * Configure PA2 as USART2_TX (AF7)
		 * pin = 2
		 * temp1 = 2 / 8 = 0  → AFR[0]
		 * temp2 = 2 % 8 = 2  → shift by 8 bits
		 * Write AF7 into bits [11:8]
		 *
		 * Without AFR configuration, peripheral will NOT control the pin
		 * even if MODER is set to alternate mode.
		 */




			if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode==GPIO_MODE_ALTFN)
			{

				//CONFIGURE THE ALTERNATE FUNCTANILITY REGISTER
				uint8_t temp1,temp2;


				temp1=pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 8;
				temp2=pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 8;
				pGPIOHandle->pGPIOx->AFR[temp1] &= ~(0xF << (4* temp2 ) );  //clearing
				pGPIOHandle->pGPIOx->AFR[temp1] |= (pGPIOHandle->GPIO_PinConfig.GPIO_PinAltFunMode << (4* temp2));

			}


}



/**
 * @fn                  GPIO_DeInit
 *
 * @brief               Resets the GPIO peripheral registers of the given
 *                      GPIO port to their default reset values
 *
 * @param[in]            pGPIOx   Base address of the GPIO port to be reset
 *
 * @return               None
 *
 * @note                 This function uses the RCC peripheral reset
 *                       mechanism to reset the GPIO registers.
 *                       After reset, the GPIO port must be re-initialized
 *                       before further use.
 */
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx)                  //rst the register
{

	if(pGPIOx==GPIOA)
			{
				GPIOA_REG_RESET();
			} else if(pGPIOx==GPIOB)
			{
				GPIOB_REG_RESET();
			}else if(pGPIOx==GPIOC)
			{
				GPIOC_REG_RESET();
			}else if(pGPIOx==GPIOD)
			{
				GPIOD_REG_RESET();
			}else if(pGPIOx==GPIOE)
			{
				GPIOE_REG_RESET();
			}else if(pGPIOx==GPIOF)
			{
				GPIOF_REG_RESET();
			}else if(pGPIOx==GPIOG)
			{
				GPIOG_REG_RESET();
			}else if(pGPIOx==GPIOH)
			{
				GPIOH_REG_RESET();
			}

}


//data read and write


/**
 * @fn                  GPIO_ReadFromInputPin
 *
 * @brief               Reads the logic level present on a specific GPIO input pin
 *
 * @param[in]            pGPIOx      Base address of the GPIO port
 * @param[in]            PinNumber  GPIO pin number to be read
 *
 * @return               uint8_t    Returns the logic level of the pin
 *                                  (0 = LOW, 1 = HIGH)
 *
 * @note                 This function reads the value from the GPIO input
 *                       data register (IDR). The pin must be configured
 *                       in input mode before calling this function.
 */

uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx,uint8_t PinNumber)
{

	uint8_t value;

	value= (uint8_t) ((pGPIOx->IDR >> PinNumber) & 0x00000001 );
return value;

}



/**
 * @fn                  GPIO_ReadFromInputPort
 *
 * @brief               Reads the current logic levels of all pins of a GPIO port
 *
 * @param[in]            pGPIOx   Base address of the GPIO port
 *
 * @return               uint16_t Returns the 16-bit value of the GPIO input
 *                                data register (IDR)
 *
 * @note                 Each bit in the returned value corresponds to the
 *                       logic level of the respective GPIO pin.
 *                       Pins must be configured in input mode before
 *                       calling this function.
 */
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx)
{

	uint16_t value;

		value=(uint16_t)pGPIOx->IDR ;

	    return value;

}


/**
 * @fn                  GPIO_WriteToOutputPin
 *
 * @brief               Writes a logic value to a specific GPIO output pin
 *
 * @param[in]            pGPIOx      Base address of the GPIO port
 * @param[in]            PinNumber  GPIO pin number to be written
 * @param[in]            value      Logic value to be written to the pin
 *                                  (0 = LOW, 1 = HIGH)
 *
 * @return               None
 *
 * @note                 The GPIO pin must be configured in output mode
 *                       before calling this function.
 */
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx,uint8_t PinNumber,uint8_t value )
{
	if(value ==GPIO_PIN_SET )
	  {
		  //write 1 to the output data register at the bit field corresponoding to the pin
		  pGPIOx->ODR |=(1 << PinNumber);
	  }else
	  {
		  pGPIOx->ODR &= ~(1 << PinNumber);

	  }

}


/**
 * @fn                  GPIO_WriteToOutputPort
 *
 * @brief               Writes a 16-bit value to the GPIO output data register
 *                      of the given GPIO port
 *
 * @param[in]            pGPIOx   Base address of the GPIO port
 * @param[in]            value    16-bit value to be written to the output
 *                                data register (ODR)
 *
 * @return               None
 *
 * @note                 Each bit in the value corresponds to a GPIO pin
 *                       of the port. The GPIO pins must be configured
 *                       in output mode before calling this function.
 */

void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t value)
{

pGPIOx->ODR=value;

}



/**
 * @fn                  GPIO_ToggleOutputPin
 *
 * @brief               Toggles the current logic level of a specific GPIO
 *                      output pin
 *
 * @param[in]            pGPIOx      Base address of the GPIO port
 * @param[in]            PinNumber  GPIO pin number to be toggled
 *
 * @return               None
 *
 * @note                 This function reads the current state of the output
 *                       data register (ODR) and inverts the bit corresponding
 *                       to the specified GPIO pin. The pin must be configured
 *                       in output mode before calling this function.
 */
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx,uint8_t PinNumber)
{

	pGPIOx->ODR ^= (1 << PinNumber);

}



// IRQ CONFIGURATION AND ISR HANDLING



/**
 * @fn                  GPIO_IRQConfig
 *
 * @brief               Configures the GPIO interrupt in the NVIC by enabling
 *                      or disabling the IRQ number and setting its priority
 *
 * @param[in]            IRQNumber     Interrupt request number to be configured
 * @param[in]            IRQPriority   Priority level to be assigned to the IRQ
 * @param[in]            EnorDi        ENABLE or DISABLE macros
 *
 * @return               None
 *
 * @note                 This function configures the NVIC for the given IRQ.
 *                       GPIO pin interrupt configuration at the EXTI level
 *                       must be done separately before calling this function.
 */



void GPIO_IRQInterruptConfig(uint8_t IRQNumber,uint8_t EnorDi)
{

     if(EnorDi == ENABLE)
     {
    	 if(IRQNumber <= 31 )
    	 {
    		 //Program For ISRE0 Register
                *NVIC_ISER0 |=(1 << IRQNumber );


     }else if(IRQNumber >31 && IRQNumber < 64)
     {

    	 //Program For ISER1 Register (32 to 63 )
         *NVIC_ISER1 |=(1 << (IRQNumber % 32) );


     }else if(IRQNumber >=64 && IRQNumber < 96)
     {
    	 //Program For ISRE2 Register  (64 to 95 )
         *NVIC_ISER2 |=(1 << (IRQNumber % 64) );

     }
     }else
     {

    	 if(IRQNumber <= 31 )
    	    	 {
    	    		 //Program For ICER0) Register
             *NVIC_ICER0 |=(1 << IRQNumber );



    	     }else if(IRQNumber >31 && IRQNumber < 64)
    	     {

    	    	 //Program For ICER1 Register (32 to 63 )
                 *NVIC_ICER1 |=(1 << (IRQNumber % 32) );


    	     }else if(IRQNumber >=64 && IRQNumber < 96)
    	     {
    	    	 //Program For ICER2 Register  (64 to 95 )
                 *NVIC_ICER2 |=(1 << (IRQNumber % 64) );


    	     }
     }



}


void GPIO_IRQPriorityConfig( uint8_t IRQNumber,uint32_t IRQPriority)
{
	//1st find out the ipr register

	uint8_t iprx=IRQNumber /4;
	uint8_t iprx_section=IRQNumber % 4;
    uint8_t shift_amount=(8 * iprx_section) + (8 - NO_PR_BITS_IMPLEMENTED );
	// it jump to that ipr register address  :here 1st we are finding which ipr address is this and  then we are jumping to that register address by adding the (ipsr *4 ) (4 beacuse reg is of 32 bit) value to the base  address
	*(NVIC_PR_BASE_ADDR + iprx  ) = (IRQPriority << shift_amount);


}

/**
 * @fn                  GPIO_IRQHandling
 *
 * @brief               Handles the GPIO interrupt for a specific pin by
 *                      clearing the interrupt pending bit
 *
 * @param[in]            PinNumber   GPIO pin number for which the interrupt
 *                                   has occurred
 *
 * @return               None
 *
 * @note                 This function must be called inside the corresponding
 *                       EXTI interrupt service routine (ISR). Clearing the
 *                       pending bit is necessary to prevent repeated
 *                       interrupts for the same event.
 */

void GPIO_IRQHandling(uint8_t PinNumber)
{
//CLEAR THE EXTI PR REGISTER CORRESPONDING TO THE PIN NUMBER

	if(EXTI->PR &  (1 << PinNumber))
	{
		//clear

		EXTI->PR |= (1 << PinNumber);
	}

}




