/*
 * stm32f446xx_spi_driver.c
 *
 *  Created on: Feb 12, 2026
 *      Author: Abhishek Patil
 */




#include "stm32f446xx_gpio_driver.h"

#include "stm32f446xx_spi_driver.h"





 static void spi_txe_interrupt_handle(SPI_Handle_t *pSPIHandle);

 static void spi_rxne_interrupt_handle(SPI_Handle_t *pSPIHandle);

 static void spi_ovr_error_interrupt_handle(SPI_Handle_t *pSPIHandle);










/************************************************************
************************************************************
 * @fn         SPI_PeriClockControl
 *
 * @brief      Enables or disables the peripheral clock for the given SPI port.
 *
 * @param[in]  pSPIx  : Base address of the SPI peripheral
 *                      (e.g., SPI1, SPI2, SPI3)
 *
 * @param[in]  EnorDi : Enable or Disable macro
 *                      ENABLE  - Enables SPI peripheral clock
 *                      DISABLE - Disables SPI peripheral clock
 *
 * @return     None
 *
 * @note       This function configures the corresponding SPI clock
 *             in the RCC (Reset and Clock Control) register.
 *             The SPI peripheral must have its clock enabled
 *             before any configuration or data transmission.
 *
 ************************************************************/

void SPI_PeriClockControl(SPI_RegDef_t *pSPIx,uint8_t EnorDi)
{
	if(EnorDi==ENABLE)
		{
			if(pSPIx==SPI1)
			{
				SPI1_PCLK_EN();
			} else if(pSPIx==SPI2)
			{
				SPI2_PCLK_EN();
			}else if(pSPIx==SPI3)
			{
				SPI3_PCLK_EN();
			}else if(pSPIx==SPI4)
			{
				SPI4_PCLK_EN();
			}

		}
		else
		{
			if(pSPIx==SPI1)
					{
						SPI1_PCLK_DI();
					} else if(pSPIx==SPI2)
					{
						SPI2_PCLK_DI();
					}else if(pSPIx==SPI3)
					{
						SPI3_PCLK_DI();
					}else if(pSPIx==SPI4)
					{
						SPI4_PCLK_DI();
					}
	}

}




//INIT AND DEINIT


/************************************************************
 * @fn         SPI_Init
 *
 * @brief      Initializes the SPI peripheral with the specified
 *             configuration settings.
 *
 * @param[in]  pSPIHandle : Pointer to the SPI handle structure
 *                          which contains:
 *                          - SPI peripheral base address
 *                          - SPI configuration parameters
 *                            (Device mode, Bus config, SCLK speed,
 *                             DFF, CPOL, CPHA, SSM, etc.)
 *
 * @return     None
 *
 * @note       1. The SPI peripheral clock must be enabled
 *                before calling this function.
 *             2. This function configures the SPI control
 *                registers (CR1, CR2) as per user settings.
 *             3. SPI peripheral remains disabled after
 *                initialization. Use SPI_PeripheralControl()
 *                to enable it.
 *
 ************************************************************/
void SPI_Init(SPI_Handle_t *pSPIHandle)
{

	//ENABLE THE CLOCK
	SPI_PeriClockControl(pSPIHandle->pSPIx, ENABLE);

	//1st let configure the SPI_CR1 register

uint32_t tempreg=0;

//configure the device mode
tempreg |= pSPIHandle->SPIConfig.SPI_DeviceMode << 2;

//config the bus config

if(pSPIHandle->SPIConfig.SPI_BusConfig ==SPI_BUS_CONFIG_FD )
{
	//Bidi MODE SHUOLD BE CLEARED

	tempreg &= ~(1 << 15);
}else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_HD)
{
	//Bidi SHOULD BE SET
	tempreg |= (1 << 15);

}
else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_SIMPLEX_RXONLY)
{
	//bidi mode should be cleared
	tempreg &= ~(1 << 15);

	//rx only bit must be set

	tempreg |= (1 << 10 );

}

//3. configure the spi serial clock speed(baud rate)
tempreg |= pSPIHandle->SPIConfig.SPI_SclkSpeed << SPI_CR1_BR;


//4.configure the dff
tempreg |=pSPIHandle->SPIConfig.SPI_DFF << SPI_CR1_DFF;

//5.configure the cpol

tempreg |=pSPIHandle->SPIConfig.SPI_CPOL << SPI_CR1_CPOL;

//6.configure the cpha

tempreg |=pSPIHandle->SPIConfig.SPI_CPHA << SPI_CR1_CPHA;

//7. configure SSM
tempreg |= pSPIHandle->SPIConfig.SPI_SSM << SPI_CR1_SSM;


pSPIHandle->pSPIx->SPI_CR1 =tempreg;
}









/************************************************************
 * @fn         SPI_DeInit
 *
 * @brief      Resets the given SPI peripheral to its default
 *             reset state.
 *
 * @param[in]  pSPIx : Base address of the SPI peripheral
 *                     (e.g., SPI1, SPI2, SPI3)
 *
 * @return     None
 *
 * @note       1. This function forces a reset of the selected
 *                SPI peripheral using RCC reset registers.
 *             2. All SPI configuration registers are cleared
 *                and returned to their default reset values.
 *             3. After de-initialization, the SPI must be
 *                re-initialized before reuse.
 *
 ************************************************************/

void SPI_DeInit(SPI_RegDef_t *pSPIx)
{
	if(pSPIx==SPI1)
	{
		SPI1_REG_RESET();
	}
	else if(pSPIx==SPI2)
	{
		SPI2_REG_RESET();
	}
	else if(pSPIx==SPI3)
	{
		SPI3_REG_RESET();
	}
	else if(pSPIx==SPI4)
	{
		SPI4_REG_RESET();
	}
}




uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx,uint32_t FlagName)
{
	if(pSPIx->SPI_SR & FlagName)
	{
		return FLAG_SET;
	}


	return FLAG_RESET;
}




















//DATA SEND AND RECEIVE


/************************************************************
 * @fn         SPI_SendData
 *
 * @brief      Sends data over SPI in blocking mode.
 *
 * @param[in]  pSPIx      : Base address of the SPI peripheral
 *                          (e.g., SPI1, SPI2, SPI3)
 *
 * @param[in]  pTxBuffer  : Pointer to the transmit buffer
 *                          containing data to be sent
 *
 * @param[in]  Len        : Length of data to be transmitted
 *                          (number of bytes)
 *
 * @return     None
 *
 * @note       1. This is a blocking (polling-based) API.
 *                It waits until TXE (Transmit buffer empty)
 *                flag is set before sending next data.
 *             2. It checks the DFF bit to determine whether
 *                data frame format is 8-bit or 16-bit.
 *             3. SPI peripheral must be initialized and enabled
 *                before calling this function.
 *             4. In full-duplex mode, data will also be received
 *                during transmission (dummy data if not required).
 *
 ************************************************************/

void SPI_SendData(SPI_RegDef_t *pSPIx,uint8_t *pTxBuffer,uint32_t Len)
{
	while(Len >0)
	{
		//1. wait untill TXE is set
		while(SPI_GetFlagStatus(pSPIx,SPI_TXE_FLAG) == FLAG_RESET );

      //2. CHEAK THE DFF BIT
		if(pSPIx->SPI_CR1 & (1 << SPI_CR1_DFF))
		{
			//16 bit dff
			// 1. load the data in to the data register
			pSPIx->SPI_DR = *((uint16_t*)pTxBuffer);
			Len -= 2;
			pTxBuffer += 2;

		}else
		{    //8 bit data
			//load the data into the data register
			pSPIx->SPI_DR = *pTxBuffer;
						Len--;
						pTxBuffer++;
		}

	}
}







/************************************************************
 * @fn         SPI_SendData
 *
 * @brief      Sends data over SPI in blocking mode.
 *
 * @param[in]  pSPIx      : Base address of the SPI peripheral
 *                          (e.g., SPI1, SPI2, SPI3)
 *
 * @param[in]  pTxBuffer  : Pointer to the transmit buffer
 *                          containing data to be sent
 *
 * @param[in]  Len        : Length of data to be transmitted
 *                          (number of bytes)
 *
 * @return     None
 *
 * @note       1. This is a blocking (polling-based) API.
 *                It waits until TXE (Transmit buffer empty)
 *                flag is set before sending next data.
 *             2. It checks the DFF bit to determine whether
 *                data frame format is 8-bit or 16-bit.
 *             3. SPI peripheral must be initialized and enabled
 *                before calling this function.
 *             4. In full-duplex mode, data will also be received
 *                during transmission (dummy data if not required).
 *
 ************************************************************/


void SPI_ReceiveData(SPI_RegDef_t *pSPIx,uint8_t *pRxBuffer,uint32_t Len)
{
	while(Len >0)
		{
			//1. wait untill TXE is set
			while(SPI_GetFlagStatus(pSPIx,SPI_RXNE_FLAG) == FLAG_RESET );

	      //2. CHEAK THE DFF BIT
			if(pSPIx->SPI_CR1 & (1 << SPI_CR1_DFF))
			{
				//16 bit dff
				// 1. load the data FROM DR TO RX BUFFER address which is given by the user
				 *((uint16_t*)pRxBuffer) = pSPIx->SPI_DR  ;
				Len -= 2;
				pRxBuffer += 2;

			}else
			{    //8 bit data
				//load the data into the data register
				*pRxBuffer = (uint8_t)pSPIx->SPI_DR;
							Len--;
							pRxBuffer++;
			}

		}
}


//IRQ CONFIGURATION AND ISR HANDLING


/************************************************************
 * @fn         SPI_IRQInterruptConfig
 *
 * @brief      Enables or disables the given SPI IRQ number
 *             in the NVIC (Nested Vectored Interrupt Controller).
 *
 * @param[in]  IRQNumber : IRQ number of the SPI peripheral
 *                         (e.g., SPI1_IRQn, SPI2_IRQn)
 *
 * @param[in]  EnorDi    : Enable or Disable macro
 *                         ENABLE  - Enables the IRQ in NVIC
 *                         DISABLE - Disables the IRQ in NVIC
 *
 * @return     None
 *
 * @note       1. This function configures the NVIC ISER/ICER
 *                registers to enable or disable interrupts.
 *             2. This does NOT configure SPI interrupt sources
 *                (TXEIE, RXNEIE, ERRIE). Those must be enabled
 *                separately in SPI control registers.
 *             3. Used in interrupt-based SPI communication.
 *
 ************************************************************/

void SPI_IRQInterruptConfig(uint8_t IRQNumber,uint8_t EnorDi)
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







/************************************************************
 * @fn         SPI_IRQPriorityConfig
 *
 * @brief      Configures the priority level of the given SPI IRQ
 *             in the NVIC priority register.
 *
 * @param[in]  IRQNumber   : IRQ number of the SPI peripheral
 *                           (e.g., SPI1_IRQn, SPI2_IRQn)
 *
 * @param[in]  IRQPriority : Priority value to be assigned
 *                           (lower value = higher priority)
 *
 * @return     None
 *
 * @note       1. This function configures the NVIC IPR
 *                (Interrupt Priority Register).
 *             2. The number of priority bits implemented
 *                depends on the MCU (e.g., 4 bits in STM32).
 *             3. Only the upper bits of the 8-bit priority
 *                field are used.
 *             4. Must be called after enabling the IRQ.
 *
 ************************************************************/
void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority)
{
	uint8_t iprx=IRQNumber /4;
		uint8_t iprx_section=IRQNumber % 4;
	    uint8_t shift_amount=(8 * iprx_section) + (8 - NO_PR_BITS_IMPLEMENTED );
		// it jump to that ipr register address  :here 1st we are finding which ipr address is this and  then we are jumping to that register address by adding the (ipsr *4 ) (4 beacuse reg is of 32 bit) value to the base  address
		*(NVIC_PR_BASE_ADDR + iprx  ) |= (IRQPriority << shift_amount);

}




/************************************************************
 * @fn         SPI_IRQHandling
 *
 * @brief      Handles SPI interrupt events such as TXE, RXNE,
 *             and error conditions.
 *
 * @param[in]  pHandle : Pointer to SPI handle structure
 *                       containing SPI peripheral base address
 *                       and transfer-related information.
 *
 * @return     None
 *
 * @note       1. This function must be called inside the
 *                corresponding SPI ISR (e.g., SPI1_IRQHandler).
 *
 *             2. It checks the interrupt flags in the SPI
 *                status register (SR) such as:
 *                   - TXE   (Transmit buffer empty)
 *                   - RXNE  (Receive buffer not empty)
 *                   - OVR   (Overrun error)
 *
 *             3. It also verifies whether the corresponding
 *                interrupt is enabled in CR2 register.
 *
 *             4. Appropriate helper functions are called
 *                to handle transmission, reception, or
 *                error clearing.
 *
 ************************************************************/





void SPI_IRQHandling(SPI_Handle_t *pHandle)
{
	uint8_t temp1,temp2;
	//1.first lets cheak for the txe

	temp1=pHandle->pSPIx->SPI_SR & (1 << SPI_SR_TXE);
	temp2=pHandle->pSPIx->SPI_CR2 & (1 << SPI_CR2_TXEIE);

	if(temp1 && temp2)
	{
		//handle TXE
		spi_txe_interrupt_handle(pHandle);

	}
	//2.cheak for the RXNE

	temp1=pHandle->pSPIx->SPI_SR & (1 << SPI_SR_RXNE);
	temp2=pHandle->pSPIx->SPI_CR2 & (1 << SPI_CR2_RXNEIE);

	if(temp1 && temp2)
		{
			//handle RXNE
			spi_rxne_interrupt_handle(pHandle);

		}
	//3.cheak for ovr flag

	temp1=pHandle->pSPIx->SPI_SR & (1 << SPI_SR_OVR);
	temp2=pHandle->pSPIx->SPI_CR2 & (1 << SPI_CR2_ERRIE);

	if(temp1 && temp2)
			{
				//handle OVR
				spi_ovr_error_interrupt_handle(pHandle);

			}

}



















/**
 * @fn         SPI_PeripheralControl
 *
 * @brief      Enables or disables the SPI peripheral.
 *
 * @param[in]  pSPIx   : Pointer to SPI peripheral base address
 *                       (e.g., SPI1, SPI2, SPI3).
 *
 * @param[in]  EnorDi  : Enable or Disable control.
 *                       Possible values:
 *                         - ENABLE  : Enables the SPI peripheral
 *                         - DISABLE : Disables the SPI peripheral
 *
 * @return     None
 *
 * @note       1. This function sets or clears the SPE bit
 *                (SPI Enable bit) in the SPI_CR1 register.
 *
 *             2. Enabling SPI:
 *                - Allows data transmission/reception.
 *                - Required before any SPI communication.
 *
 *             3. Disabling SPI:
 *                - Stops ongoing communication.
 *                - Should be done only after BSY flag = 0
 *                  to avoid data corruption.
 *
 *             4. Typically used during:
 *                - SPI initialization
 *                - Before starting transmission
 *                - Power-saving or reconfiguration
 */


void SPI_PeripheralControl(SPI_RegDef_t *pSPIx,uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		pSPIx->SPI_CR1 |= (1 << SPI_CR1_SPE);
	}else
	{
		pSPIx->SPI_CR1 &= ~(1 << SPI_CR1_SPE);

	}
}



/**
 * @fn         SPI_SSIConfig
 *
 * @brief      Configures the SSI (Internal Slave Select) bit
 *             in the SPI_CR1 register.
 *
 * @param[in]  pSPIx   : Pointer to SPI peripheral base address
 *                       (e.g., SPI1, SPI2, SPI3).
 *
 * @param[in]  EnorDi  : Enable or Disable control.
 *                       Possible values:
 *                         - ENABLE  : Sets SSI bit
 *                         - DISABLE : Clears SSI bit
 *
 * @return     None
 *
 * @note       1. SSI (Software Slave Management Input) is used
 *                when SSM (Software Slave Management) is enabled.
 *
 *             2. If SSM = 1:
 *                - SSI = 1 → Prevents MODF (Mode Fault) error
 *                            when SPI is configured as Master.
 *                - SSI = 0 → May trigger MODF if NSS is internally low.
 *
 *             3. This function is mainly used in:
 *                - Master mode with software NSS management.
 *                - Full software-controlled SPI setups.
 *
 *             4. Typical usage:
 *                SPI_SSIConfig(SPI1, ENABLE);
 *                → Sets NSS internally HIGH.
 *
 *             5. Important:
 *                Always set SSI = 1 before enabling SPI
 *                when using SSM in master mode to avoid MODF error.
 */



void SPI_SSIConfig(SPI_RegDef_t *pSPIx,uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
		{
			pSPIx->SPI_CR1 |= (1 << SPI_CR1_SSI);
		}else
		{
			pSPIx->SPI_CR1 &= ~(1 << SPI_CR1_SSI);

		}
}








/**
 * @fn         SPI_SSOEConfig
 *
 * @brief      Enables or disables automatic NSS output
 *             in hardware (SSOE control).
 *
 * @param[in]  pSPIx   : Pointer to SPI peripheral base address
 *                       (e.g., SPI1, SPI2, SPI3).
 *
 * @param[in]  EnorDi  : Enable or Disable control.
 *                       Possible values:
 *                         - ENABLE  : NSS output enabled
 *                         - DISABLE : NSS output disabled
 *
 * @return     None
 *
 * @note       1. SSOE (Slave Select Output Enable) is located
 *                in the SPI_CR2 register.
 *
 *             2. When enabled in MASTER mode:
 *                - Hardware automatically drives NSS LOW
 *                  when SPI is enabled.
 *                - NSS goes HIGH when SPI is disabled.
 *
 *             3. Used when:
 *                - Hardware NSS management is required.
 *                - Avoiding manual GPIO control of NSS pin.
 *
 *             4. Works only when:
 *                - SPI is configured as MASTER.
 *                - SSM = 0 (hardware slave management).
 *
 *             5. If SSM = 1:
 *                - SSOE has no effect.
 *
 *             6. Common use case:
 *                Multi-slave SPI systems using hardware NSS.
 */



void SPI_SSOEConfig(SPI_RegDef_t *pSPIx,uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
		{
			pSPIx->SPI_CR2 |= (1 << SPI_CR2_SSOE);
		}else
		{
			pSPIx->SPI_CR2 &= ~(1 << SPI_CR2_SSOE);

		}
}





/**
 * @fn         SPI_SendDataIT
 *
 * @brief      Transmits data over SPI using interrupt mode.
 *
 * @param[in]  pSPIHandle : Pointer to SPI handle structure
 *                         containing SPI peripheral base address
 *                         and transmission state info.
 *
 * @param[in]  pTxBuffer  : Pointer to transmit buffer.
 *
 * @param[in]  Len        : Length of data to transmit in bytes.
 *
 * @return     Status of transmission:
 *               - SPI_READY  : Transmission started successfully
 *               - SPI_BUSY_TX: SPI already busy in transmission
 *
 * @note       1. This function:
 *                - Stores Tx buffer and length in handle.
 *                - Sets SPI state to BUSY_TX.
 *                - Enables TXE interrupt in CR2.
 *
 *             2. Actual transmission happens inside:
 *                SPI_IRQHandling() → TXE handler.
 *
 *             3. Non-blocking API:
 *                CPU can execute other tasks while data transmits.
 *
 *             4. Once transmission completes:
 *                - TXE interrupt is disabled
 *                - State is reset to READY
 *                - Application callback is triggered
 *
 *             5. Callback example:
 *                SPI_ApplicationEventCallback(..., SPI_EVENT_TX_CMPLT);
 */
uint8_t SPI_SendDataIT(SPI_Handle_t *pSPIHandle,uint8_t *pTxBuffer,uint32_t Len)
{
	uint8_t state =pSPIHandle->TxState;

	if(state != SPI_BUSY_IN_TX)

	{


	//1.save the tx buffer address and len information in some global variables
	pSPIHandle->pTxBuffer = pTxBuffer;
	pSPIHandle->TxLen =    Len;

	//2. mark the state as busy in transmission so that
	// no other code can take over some spi peripheral untill transmissin is over

	pSPIHandle->TxState = SPI_BUSY_IN_TX;

	pSPIHandle->pSPIx->SPI_CR2 |= ( 1 << SPI_CR2_TXEIE);

	//4. Data Transmission will be handled by the isr code

}

	return state;
}









/**
 * @fn         SPI_ReceiveDataIT
 *
 * @brief      Receives data over SPI using interrupt mode.
 *
 * @param[in]  pSPIHandle : Pointer to SPI handle structure
 *                         containing SPI peripheral base address
 *                         and reception state info.
 *
 * @param[out] pRxBuffer  : Pointer to receive buffer.
 *
 * @param[in]  Len        : Length of data to receive in bytes.
 *
 * @return     Status of reception:
 *               - SPI_READY   : Reception started successfully
 *               - SPI_BUSY_RX : SPI already busy in reception
 *
 * @note       1. This function:
 *                - Stores Rx buffer and length in handle.
 *                - Sets SPI state to BUSY_RX.
 *                - Enables RXNE interrupt in CR2.
 *
 *             2. Actual reception occurs in:
 *                SPI_IRQHandling() → RXNE handler.
 *
 *             3. Non-blocking API:
 *                CPU can perform other tasks during reception.
 *
 *             4. Once reception completes:
 *                - RXNE interrupt is disabled
 *                - State reset to READY
 *                - Application callback triggered
 *
 *             5. Application must ensure:
 *                - Clock generation (Master mode)
 *                - Dummy writes if required for full-duplex SPI.
 */
uint8_t SPI_ReceiveDataIT(SPI_Handle_t *pSPIHandle,uint8_t *pRxBuffer,uint32_t Len)
{

	uint8_t state =pSPIHandle->RxState;

		if(state != SPI_BUSY_IN_RX)

		{


		//1.save the tx buffer address and len information in some global variables
		pSPIHandle->pRxBuffer = pRxBuffer;
		pSPIHandle->RxLen =    Len;

		//2. mark the state as busy in transmission so that
		// no other code can take over some spi peripheral untill transmissin is over

		pSPIHandle->RxState = SPI_BUSY_IN_RX;

		pSPIHandle->pSPIx->SPI_CR2 |= ( 1 << SPI_CR2_RXNEIE);

		//4. Data Transmission will be handled by the isr code

	}

		return state;


}


//some helper function implementation

static void spi_txe_interrupt_handle(SPI_Handle_t *pSPIHandle)
{
	//2. CHEAK THE DFF BIT
			if(pSPIHandle->pSPIx->SPI_CR1 & (1 << SPI_CR1_DFF))
			{
				//16 bit dff
				// 1. load the data in to the data register
				pSPIHandle->pSPIx->SPI_DR = *((uint16_t*)pSPIHandle->pTxBuffer);
				pSPIHandle->TxLen -= 2;
				pSPIHandle->pTxBuffer += 2;

			}else
			{    //8 bit data
				//load the data into the data register
				pSPIHandle->pSPIx->SPI_DR = *pSPIHandle->pTxBuffer;
								pSPIHandle->TxLen --;
								pSPIHandle->pTxBuffer ++;
			}

			if( ! pSPIHandle->TxLen)
			{
				//TX len is zero,so close the spi transmission and inform the application
				//tx is over

				// this prevents interrupts from setting up to txe flag
				 SPI_CloseTransmission(pSPIHandle);
				SPI_ApplicationEventCallback(pSPIHandle,SPI_EVENT_TX_CMPLT);
			}


}

 static void spi_rxne_interrupt_handle(SPI_Handle_t *pSPIHandle)
 {
	 //2. CHEAK THE DFF BIT
	 			if(pSPIHandle->pSPIx->SPI_CR1 & (1 << SPI_CR1_DFF))
	 			{
	 				//16 bit dff
	 				// 1. load the data in to the data register
	 				*((uint16_t*)pSPIHandle->pRxBuffer) = (uint16_t) pSPIHandle->pSPIx->SPI_DR;
	 				pSPIHandle->RxLen -= 2;
	 				pSPIHandle->pRxBuffer += 2;

	 			}else
	 			{    //8 bit data
	 				//load the data into the data register
	 				(*pSPIHandle->pRxBuffer) = pSPIHandle->pSPIx->SPI_DR ;
	 								pSPIHandle->RxLen --;
	 								pSPIHandle->pRxBuffer ++;
	 			}

	 			if( ! pSPIHandle->RxLen)
	 			{
	 				//TX len is zero,so close the spi transmission and inform the application
	 				//tx is over

	 				// turn off the rxneie interrrupt
	 				 SPI_CloseReception(pSPIHandle);
	 				SPI_ApplicationEventCallback(pSPIHandle,SPI_EVENT_RX_CMPLT);
	 			}





 }

 static void spi_ovr_error_interrupt_handle(SPI_Handle_t *pSPIHandle)
 {
	 uint8_t temp;
	 //clear the ovr flag
	 if(pSPIHandle->TxState != SPI_BUSY_IN_TX)
	 {
		 temp=pSPIHandle->pSPIx->SPI_DR;
		 temp=pSPIHandle->pSPIx->SPI_SR;
		 (void)temp;
		 //2.inform the application
		SPI_ApplicationEventCallback(pSPIHandle,SPI_EVENT_OVR_ERR);


	 }
 }




 void SPI_CloseTransmission(SPI_Handle_t *pSPIHandle)
 {
	       pSPIHandle->pSPIx->SPI_CR2 &= ~( 1 << SPI_CR2_TXEIE);
	    	pSPIHandle->pTxBuffer = NULL;
	 		pSPIHandle->TxLen =0;
	 		pSPIHandle->TxState = SPI_READY;
 }
 void SPI_CloseReception(SPI_Handle_t *pSPIHandle)
 {
	           pSPIHandle->pSPIx->SPI_CR2 &= ~( 1 << SPI_CR2_RXNEIE);
	            pSPIHandle->pRxBuffer = NULL;
	 	 		pSPIHandle->RxLen =0;
	 	 		pSPIHandle->RxState = SPI_READY;
 }


 void SPI_ClearOVRFlag(SPI_RegDef_t *pSPIx)
 {
	 uint8_t temp;
	 temp=pSPIx->SPI_DR;
	 temp=pSPIx->SPI_SR;
	 (void)temp;
 }



 weak void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle,uint8_t AppEv)
 {
     // This is a weak implementation
 }

