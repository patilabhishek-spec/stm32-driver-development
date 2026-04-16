/*
 * stm32f446xx_gpio_driver.h
 *
 *  Created on: Feb 4, 2026
 *      Author: Abhishek Patil
 */

#ifndef INC_STM32F446XX_GPIO_DRIVER_H_
#define INC_STM32F446XX_GPIO_DRIVER_H_


#include "stm32f446xx.h"




typedef struct{

	uint8_t GPIO_PinNumber;
	uint8_t GPIO_PinMode;       //POSSIBLE values from @GPIO_PIN_MODES
	uint8_t GPIO_PinSpeed;       //POSSIBLE values from @GPIO pin speed
	uint8_t GPIO_PinPuPdControl;    //POSSIBLE values from @GPIO pullpull down modes
	uint8_t GPIO_PinOPType;         //POSSIBLE values from @GPIO pin output types modes
	uint8_t GPIO_PinAltFunMode;
}GPIO_PinConfig_t;

typedef struct{
	GPIO_RegDef_t  *pGPIOx;            //this hold the base address of the gpio port to which the pin belongs
	GPIO_PinConfig_t GPIO_PinConfig;    //this holds the gpio pin configuration setting
}GPIO_Handle_t;



//GPIO_PIN_NUMBERS
#define GPIO_PIN_NO_0              0
#define GPIO_PIN_NO_1              1
#define GPIO_PIN_NO_2              2
#define GPIO_PIN_NO_3              3
#define GPIO_PIN_NO_4              4
#define GPIO_PIN_NO_5              5
#define GPIO_PIN_NO_6              6
#define GPIO_PIN_NO_7              7
#define GPIO_PIN_NO_8              8
#define GPIO_PIN_NO_9              9
#define GPIO_PIN_NO_10             10
#define GPIO_PIN_NO_11             11
#define GPIO_PIN_NO_12             12
#define GPIO_PIN_NO_13             13
#define GPIO_PIN_NO_14             14
#define GPIO_PIN_NO_15             15

//GPIO pin posible modes
//GPIO_PIN_MODES

#define GPIO_MODE_IN          0
#define GPIO_MODE_OUT         1
#define GPIO_MODE_ALTFN       2
#define GPIO_MODE_ANALOG      3
#define GPIO_MODE_IT_FT       4
#define GPIO_MODE_IT_RT       5
#define GPIO_MODE_IT_RFT      6


//GPIO pin Posible output types
//GPIO pin output types modes

#define GPIO_OP_TYPE_PP     0
#define GPIO_OP_TYPE_OD     1



//GPIO pin posible output speeds
//GPIO pin speed

#define GPIO_SPEED _LOW         0
#define GPIO_SPEED_MEDIUM       1
#define GPIO_SPEED_FAST         2
#define GPIO_SPEED_HIGH         3


//GPIO pullpull down configuration macros
//GPIO pullpull down modes

#define GPIO_NO_PUPD     0
#define GPIO_PIN_PU       1
#define GPIO_PIN_PD       2





//****************   API SUPPORTED BY THIS DRIVER************

//peripheral clock setup
void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx,uint8_t EnorDi);


//init and de-init

void GPIO_Init(GPIO_Handle_t *pGPIOHandle);
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx);                         //rst the register


//data read and write

uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx,uint8_t PinNumber);
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx);
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx,uint8_t PinNumber,uint8_t value );
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t value);
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx,uint8_t PinNumber);


// IRQ CONFIGURATION AND ISR HANDLING
void GPIO_IRQInterruptConfig(uint8_t IRQNumber,uint8_t EnorDi);
void GPIO_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);
void GPIO_IRQHandling(uint8_t PinNumber);








/* INC_STM32F446XX_GPIO_DRIVER_H_ */
#endif
