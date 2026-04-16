/*
 * 001led_toggle.c
 *
 *  Created on: Feb 7, 2026
 *      Author: Abhishek Patil
 */


#include "stm32f446xx.h"
#define HIGH 0
#define BTN_PRESSED HIGH

void delay(void)
{

	for(uint32_t i=0;i<500000;i++);
}
int main(void)
{
    GPIO_LedInit();

    while(1)
    {
        GPIO_ToggleOutputPin(GPIOA, GPIO_PIN_NO_5);
        for(volatile uint32_t i = 0; i < 2000000; i++);
    }
}
