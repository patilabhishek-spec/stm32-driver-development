/*
 * stm32f446xx_rcc_driver.c
 *
 *  Created on: Mar 30, 2026
 *      Author: Abhishek Patil
 */


#include "stm32f446xx_rcc_driver.h"

uint32_t RCC_GetPLLOutputClock(void);

/************************************************************
 * @var        AHB_Prescaler
 *
 * @brief      Lookup table containing division factors for
 *             the AHB bus prescaler.
 *
 * @details    This table is used to determine the division
 *             factor applied to the system clock when
 *             calculating the AHB clock (HCLK).
 *
 *             The prescaler values correspond to the HPRE
 *             bits in the RCC_CFGR register.
 *
 *             When the HPRE field value is greater than or
 *             equal to 8, the system clock is divided by
 *             the corresponding factor from this table.
 *
 * @division_factors
 *             Index | Division Factor
 *             ------|----------------
 *               0   | 2
 *               1   | 4
 *               2   | 8
 *               3   | 16
 *               4   | 64
 *               5   | 128
 *               6   | 256
 *               7   | 512
 *
 * @use_case
 *             Used in clock calculation functions such as
 *             RCC_GetPCLK1Value() to determine the AHB clock.
 *
 * @note       The final AHB clock (HCLK) is calculated as:
 *
 *             HCLK = SystemClock / AHB_Prescaler
 *
 ************************************************************/
uint16_t AHB_Prescaler[8] = { 2, 4, 8, 16, 64, 128, 256, 512 };

/************************************************************
 * @var        APB1_Prescaler
 *
 * @brief      Lookup table containing division factors for
 *             the APB1 bus prescaler.
 *
 * @details    This table is used to determine the division
 *             factor applied to the AHB clock when
 *             calculating the APB1 peripheral clock (PCLK1).
 *
 *             The prescaler values correspond to the PPRE1
 *             bits in the RCC_CFGR register.
 *
 *             When the PPRE1 field value is greater than or
 *             equal to 4, the AHB clock is divided by the
 *             corresponding factor from this table.
 *
 * @division_factors
 *             Index | Division Factor
 *             ------|----------------
 *               0   | 2
 *               1   | 4
 *               2   | 8
 *               3   | 16
 *
 * @use_case
 *             Used in clock calculation functions such as
 *             RCC_GetPCLK1Value() to determine the APB1 clock
 *             frequency used by peripherals like:
 *
 *             • I2C
 *             • I2C2 / I2C3
 *             • USART2 / USART3
 *             • Timers
 *
 * @note       The final APB1 clock (PCLK1) is calculated as:
 *
 *             PCLK1 = HCLK / APB1_Prescaler
 *
 ************************************************************/

uint8_t APB1_Prescaler[4] = { 2, 4, 8, 16 };



/************************************************************
 * @fn         RCC_GetPCLK1Value
 *
 * @brief      Returns the frequency of the APB1 peripheral
 *             clock (PCLK1).
 *
 * @param[in]  None
 *
 * @return     APB1 clock frequency in Hz
 *
 * @details    This function determines the clock frequency
 *             supplied to peripherals connected to the APB1
 *             bus. The calculation involves:
 *
 *             1. Identifying the system clock source
 *                (HSI, HSE, or PLL).
 *
 *             2. Determining the AHB prescaler value.
 *
 *             3. Determining the APB1 prescaler value.
 *
 *             The final APB1 clock is calculated as:
 *
 *             PCLK1 = SystemClock / AHB_Prescaler / APB1_Prescaler
 *
 * @use_case
 *             - Required when configuring peripherals connected
 *               to the APB1 bus.
 *
 *             Examples of APB1 peripherals:
 *                 • I2C1, I2C2, I2C3
 *                 • USART2, USART3
 *                 • I2C2, I2C3
 *                 • Timers (TIM2, TIM3, TIM4, etc.)
 *
 *             Many peripherals use the APB1 clock to generate
 *             communication timings. For example:
 *
 *                 • I2C SCL clock generation
 *                 • UART baud rate calculation
 *                 • I2C clock generation
 *
 * @note       Accurate clock calculation is essential because
 *             incorrect clock values can cause communication
 *             failures or incorrect timing.
 *
 ************************************************************/

uint8_t APB2_Prescaler[4] = { 2, 4, 8, 16 };







uint32_t RCC_GetPCLK1Value(void) {
	uint32_t pClk1, SystemClk;

	uint8_t Clksrc, temp;
	uint32_t ahbp, apb1p;
	Clksrc = ((RCC->CFGR >> 2) & 0x03); //rcc clock configuration register system clock switch status

	if (Clksrc == 0) {
		SystemClk = 16000000;   //HSI
	} else if (Clksrc == 1) {
		SystemClk = 8000000;    //HSE
	} else if (Clksrc == 2 || Clksrc == 3) {
		SystemClk = RCC_GetPLLOutputClock();
	}
	//AHB
	temp = ((RCC->CFGR >> 4) & 0xF);   //// Reading bits [12:10] from RCC_CFGR register to know current APB1 prescaler setting

	if (temp < 8) {
		ahbp = 1;

	} else {
		ahbp = AHB_Prescaler[temp - 8];
	}
	//APB1

	temp = ((RCC->CFGR >> 10) & 0x07);//// Reading bits [12:10] from RCC_CFGR register to know current APB1 prescaler setting

	if (temp < 4) {
		apb1p = 1;

	} else {
		apb1p = APB1_Prescaler[temp - 4];
	}

	pClk1 = (SystemClk / ahbp) / apb1p;
	return pClk1;
}




uint32_t RCC_GetPCLK2Value(void)
{
	uint32_t pClk2, SystemClk;

		uint8_t Clksrc, temp;
		uint32_t ahbp, apb2p;
		Clksrc = ((RCC->CFGR >> 2) & 0x03); //rcc clock configuration register system clock switch status

		if (Clksrc == 0) {
			SystemClk = 16000000;  //HSI
		} else if (Clksrc == 1) {
			SystemClk = 8000000; //HSE
		} else if (Clksrc == 2 || Clksrc == 3) {
			SystemClk = RCC_GetPLLOutputClock();
		}
		//AHB
		temp = ((RCC->CFGR >> 4) & 0xF);   //// Reading bits [15:13] from RCC_CFGR register to know current APB1 prescaler setting

		if (temp < 8) {
			ahbp = 1;

		} else {
			ahbp = AHB_Prescaler[temp - 8];
		}
		//APB2

		temp = ((RCC->CFGR >> 13) & 0x07);//// Reading bits [15:13] from RCC_CFGR register to know current APB1 prescaler setting

		if (temp < 4) {
			apb2p = 1;

		} else {
			apb2p = APB2_Prescaler[temp - 4];
		}

		pClk2 = (SystemClk / ahbp) / apb2p;
		return pClk2;


}


uint32_t RCC_GetPLLOutputClock(void)
{
    uint32_t pllclk, pllm, plln, pllp;
    uint32_t pll_src, pllsrcclk;

    // 1. Get PLL source (HSI or HSE)
    pll_src = (RCC->PLLCFGR >> 22) & 0x1;

    if(pll_src == 0)
    {
        pllsrcclk = 16000000; // HSI
    }
    else
    {
        pllsrcclk = 8000000;  // HSE
    }

    // 2. Get PLLM
    pllm = RCC->PLLCFGR & 0x3F;

    // 3. Get PLLN
    plln = (RCC->PLLCFGR >> 6) & 0x1FF;

    // 4. Get PLLP
    pllp = ((RCC->PLLCFGR >> 16) & 0x3);
    pllp = (pllp + 1) * 2;   // Convert to 2,4,6,8

    // 5. Calculate PLL clock
    pllclk = (pllsrcclk / pllm) * plln;
    pllclk = pllclk / pllp;

    return pllclk;
}
