#ifndef INC_STM32F446XX_H_
#define INC_STM32F446XX_H_

#include<stdint.h>
#include<stddef.h>

#define   weak     __attribute__((weak))

//*****************PROCESSOR SPECIFIC DETAILS*****************************

//ARM CORTEX M4 PROCESSOR NVIC ISERx REGISTERS  ADDRESSES
//NVIC is a hardware interrupt controller in ARM Cortex-M processors that manages, prioritizes, and handles interrupts
//efficiently with support for nesting and fast response.

#define NVIC_ISER0       ((__volatile uint32_t*)0xE000E100)
#define NVIC_ISER1       ((__volatile uint32_t*)0xE000E104)
#define NVIC_ISER2       ((__volatile uint32_t*)0xE000E108)
#define NVIC_ISER3       ((__volatile uint32_t*)0xE000E10C)

//ARM CORTEX M4 PROCESSOR NVIC ICERx REGISTERS  ADDRESSES

#define NVIC_ICER0       ((__volatile uint32_t*)0xE000E180)
#define NVIC_ICER1       ((__volatile uint32_t*)0xE000E184)
#define NVIC_ICER2       ((__volatile uint32_t*)0xE000E188)
#define NVIC_ICER3       ((__volatile uint32_t*)0xE000E18C)

//ARM CORTEX Mx PROCESSOR PRIORITY REGISTER ADDRESSES CALCULATION

#define NVIC_PR_BASE_ADDR   ((__volatile uint32_t*)0xE000E400)

#define NO_PR_BITS_IMPLEMENTED        4   //in st's case

//base addresses of flash and sram memories

#define FLASH_BASEADDR           0x80000000U
#define SRAM1_BASEADDR           0x20000000U
#define SRAM                     SRAM1_BASEADDR
#define SRAM2_BASEADDR           0x2001C000U
#define  ROM                     0x1FFF0000U

//base addresses of ahb and apb bus

#define AHB1_BASEADDR           0x40020000U
#define AHB2_BASEADDR           0x50000000U
#define AHB3_BASEADDR           0x60000000U
#define APB1_BASEADDR           0x40000000U
#define APB2_BASEADDR           0x40010000U

//BASE ADDRESSES OF THE PHERIPHERAL REGISTERS WHICH ARE HANGING ON THE AHB1 BUS

#define  GPIOA_BASEADDR      (AHB1_BASEADDR + 0x0000 )
#define  GPIOB_BASEADDR      (AHB1_BASEADDR + 0x0400 )
#define  GPIOC_BASEADDR      (AHB1_BASEADDR + 0x0800 )
#define  GPIOD_BASEADDR      (AHB1_BASEADDR + 0x0C00 )
#define  GPIOE_BASEADDR      (AHB1_BASEADDR + 0x1000 )
#define  GPIOF_BASEADDR      (AHB1_BASEADDR + 0x1400 )
#define  GPIOG_BASEADDR      (AHB1_BASEADDR + 0x1800  )
#define  GPIOH_BASEADDR      (AHB1_BASEADDR + 0x1C00  )
#define  RCC_BASEADDR        (AHB1_BASEADDR + 0x3800  )

//BASE ADDRESSES OF THE PHERIPHERAL REGISTERS WHICH ARE HANGING ON THE APB1 BUS

#define I2C1_BASEADDR        (APB1_BASEADDR + 0x5400)
#define I2C2_BASEADDR        (APB1_BASEADDR + 0x5800)
#define I2C3_BASEADDR        (APB1_BASEADDR + 0x5C00)

#define SPI2_BASEADDR        (APB1_BASEADDR + 0x3800 )
#define SPI3_BASEADDR        (APB1_BASEADDR + 0x3C00 )

#define USART2_BASEADDR       (APB1_BASEADDR + 0x4400 )
#define USART3_BASEADDR       (APB1_BASEADDR + 0x4800 )

#define UART4_BASEADDR       (APB1_BASEADDR + 0x4C00 )
#define UART5_BASEADDR       (APB1_BASEADDR + 0x5000 )

//BASE ADDRESSES OF THE PHERIPHERAL REGISTERS WHICH ARE HANGING ON THE APB2 BUS
#define EXTI_BASEADDR               (APB2_BASEADDR + 0x3C00 )
#define SPI1_BASEADDR               (APB2_BASEADDR + 0x3000 )
#define SPI4_BASEADDR               (APB2_BASEADDR + 0x3400 )
#define USART1_BASEADDR             (APB2_BASEADDR + 0x1000 )
#define USART6_BASEADDR             (APB2_BASEADDR + 0x1400 )
#define SYSCFG_BASEADDR             (APB2_BASEADDR + 0x3800 )

//GPIO PERIPHERAL REGISTER  we use strcuture here because of this we did not write for each gpio

typedef struct {

	volatile uint32_t MODER;         // GPIO PORT MODE REGISTER
	volatile uint32_t OTYPER;          //GPIO OUTPUT TYPE REGISTER
	volatile uint32_t OSPEEDR;        //GPIO OUTPUT SPEED REGISTER
	volatile uint32_t PUPDR;          //GPIO PULLUP PULLDOWN REGISTER
	volatile uint32_t IDR;           //GPIO INPUT DATA REGISTER
	volatile uint32_t ODR;          //GPIO OUTPUT DATA REGISTER
	volatile uint32_t BSRR;         //GPIO BIT SET RESET REGISTER
	volatile uint32_t LCKR;          //GPIO port configuration lock register
	volatile uint32_t AFR[2]; //AF[0]:ALTERNATE FUNCTION LOW REGISTER,AF[1]: GPIO ALTERNATE FUNCTION HiGH REGISTER

} GPIO_RegDef_t;

//PERIPHERAL DEFINATION STRCUTURE TO RCC

typedef struct {

	volatile uint32_t CR;
	volatile uint32_t PLLCFGR;
	volatile uint32_t CFGR;
	volatile uint32_t CIR;
	volatile uint32_t AHB1RSTR;
	volatile uint32_t AHB2RSTR;
	volatile uint32_t AHB3RSTR;
	volatile uint32_t RESERVED0;
	volatile uint32_t APB1RSTR;
	volatile uint32_t APB2RSTR;
	volatile uint32_t RESERVED1[2];
	volatile uint32_t AHB1ENR;
	volatile uint32_t AHB2ENR;
	volatile uint32_t AHB3ENR;
	volatile uint32_t RESERVED2;
	volatile uint32_t APB1ENR;
	volatile uint32_t APB2ENR;
	volatile uint32_t RESERVED3[2];
	volatile uint32_t AHB1LPENR;
	volatile uint32_t AHB2LPENR;
	volatile uint32_t RESERVED4;
	volatile uint32_t APB1LPENR;
	volatile uint32_t APB2LPENR;
	volatile uint32_t RESERVED5[2];
	volatile uint32_t BDCR;
	volatile uint32_t CSR;
	volatile uint32_t RESERVED6[2];
	volatile uint32_t SSCGR;
	volatile uint32_t PLLI2SCFGR;
	volatile uint32_t PLLSAICFGR;
	volatile uint32_t DCKCFGR;
	volatile uint32_t CKGATENR;
	volatile uint32_t DCKCFGR2;
} RCC_RegDef_t;

//PERIPHERAL DEFINATION STRCUTURE TO EXTI

typedef struct {
	__volatile uint32_t IMR;                             //Address Offset: 0x00;
	__volatile uint32_t EMR;                             //Address Offset: 0x04;
	__volatile uint32_t RTSR;                            //Address Offset: 0x08;
	__volatile uint32_t FTSR;                            //Address Offset: 0x0C;
	__volatile uint32_t SWIER;                           //Address Offset: 0x10;
	__volatile uint32_t PR;                              //Address Offset: 0x14;
} EXTI_RegDef_t;

//PERIPHERAL DEFINATION STRCUTURE TO EXTI

typedef struct {
	__volatile uint32_t MEMRMP;                          //Address Offset: 0x00;
	__volatile uint32_t PMC;                             //Address Offset: 0x04;
	__volatile uint32_t EXTICR[4];                  //Address Offset: 0x08-0x14;
	__volatile uint32_t RESERVED1[2];                    //Address Offset: 0x1C;
	__volatile uint32_t CMPCR;                           //Address Offset: 0x20;
	__volatile uint32_t RESERVED2[2];                   //Address Offset: 0x028;
	__volatile uint32_t CFGR;                            //Address Offset: 0x2C;

} SYSCFG_RegDef_t;

//peripheral register definatiion strcuture for spi

typedef struct {
	volatile uint32_t SPI_CR1;     //  SPI control register 1
	volatile uint32_t SPI_CR2;       //  SPI control register 2
	volatile uint32_t SPI_SR;      //  SPI status register
	volatile uint32_t SPI_DR;      // SPI data register
	volatile uint32_t SPI_CRCPR;      // SPI CRC polynomial register
	volatile uint32_t SPI_RXCRCR;      // SPI RX CRC register
	volatile uint32_t SPI_TXCRCR;      //SPI TX CRC register
	volatile uint32_t SPI_I2SCFGR;      //SPI_I2S configuration register
	volatile uint32_t SPI_I2SPR;      //SPI_I2S prescaler register
} SPI_RegDef_t;

//PERIPHERAL DEFINATION STRCUTURE TO I2C

typedef struct {
	volatile uint32_t I2C_CR1;          //  I2C control register 1
	volatile uint32_t I2C_CR2;          //  I2C control register 2
	volatile uint32_t I2C_OAR1;         //  I2C own address register 1
	volatile uint32_t I2C_OAR2;         //  I2C own address register 2
	volatile uint32_t I2C_DR;           //  I2C data register
	volatile uint32_t I2C_SR1;          //  I2C status register 1
	volatile uint32_t I2C_SR2;          //  I2C status register 2
	volatile uint32_t I2C_CCR;          //  I2C clock control register
	volatile uint32_t I2C_TRISE;        //  I2C TRISE register
	volatile uint32_t I2C_FLTR;         //  I2C FLTR register

} I2C_RegDef_t;

//PERIPHERAL DEFINATION STRCUTURE OF USART

typedef struct {
	volatile uint32_t USART_SR;          //  Status register
	volatile uint32_t USART_DR;          //  Data register
	volatile uint32_t USART_BRR;         //  Baud rate register
	volatile uint32_t USART_CR1;         // Control register 1
	volatile uint32_t USART_CR2;         //  Control register 2
	volatile uint32_t USART_CR3;         //  Control register 3
	volatile uint32_t USART_GTPR;        // Guard time and prescaler register
} USART_RegDef_t;

/*
 GPIOA Peripheral Macro Explanation

 #define GPIOA ((GPIO_RegDef_t*)GPIOA_BASEADDR)

 This macro converts the base address of GPIOA into a pointer of type
 GPIO_RegDef_t structure. The structure contains all GPIO register
 definitions such as MODER, OTYPER, OSPEEDR, PUPDR, IDR and ODR.

 By typecasting the base address to a structure pointer, we can access
 GPIO registers using the arrow operator (->) like:

 GPIOA->MODER
 GPIOA->ODR
 GPIOA->IDR

 This makes register access easier, readable and organized instead of
 directly using memory addresses.
 */

//peripheral defination   ( peripheral base addresses typecasted to xxx_RegDef_t)
#define GPIOA                   ((GPIO_RegDef_t*)GPIOA_BASEADDR)
#define GPIOB                   ((GPIO_RegDef_t*)GPIOB_BASEADDR)
#define GPIOC                   ((GPIO_RegDef_t*)GPIOC_BASEADDR)
#define GPIOD                   ((GPIO_RegDef_t*)GPIOD_BASEADDR)
#define GPIOE                   ((GPIO_RegDef_t*)GPIOE_BASEADDR)
#define GPIOF                   ((GPIO_RegDef_t*)GPIOF_BASEADDR)
#define GPIOG                   ((GPIO_RegDef_t*)GPIOG_BASEADDR)
#define GPIOH                   ((GPIO_RegDef_t*)GPIOH_BASEADDR)

#define RCC                  ((RCC_RegDef_t*)RCC_BASEADDR)
#define EXTI                 ((EXTI_RegDef_t*)EXTI_BASEADDR)
#define SYSCFG               ((SYSCFG_RegDef_t*)SYSCFG_BASEADDR)

//peripheral defination of spi

#define SPI1         ((SPI_RegDef_t*)SPI1_BASEADDR)
#define SPI2         ((SPI_RegDef_t*)SPI2_BASEADDR)
#define SPI3         ((SPI_RegDef_t*)SPI3_BASEADDR)
#define SPI4         ((SPI_RegDef_t*)SPI4_BASEADDR)

//PERIPHERAL DEFINATION MACROS FOR I2C

#define I2C1         ((I2C_RegDef_t*)I2C1_BASEADDR)
#define I2C2         ((I2C_RegDef_t*)I2C2_BASEADDR)
#define I2C3         ((I2C_RegDef_t*)I2C3_BASEADDR)

//PERIPHERAL DEFINATION MACROS FOR USARTx

#define USART1         ((USART_RegDef_t*)USART1_BASEADDR)
#define USART2         ((USART_RegDef_t*)USART2_BASEADDR)
#define USART3         ((USART_RegDef_t*)USART3_BASEADDR)
#define UART4         ((USART_RegDef_t*)UART4_BASEADDR)
#define UART5         ((USART_RegDef_t*)UART5_BASEADDR)
#define USART6         ((USART_RegDef_t*)USART6_BASEADDR)





//clock enable macros for gpiox peripheral

#define GPIOA_PCLK_EN()      (RCC->AHB1ENR) |=(1<<0)
#define GPIOB_PCLK_EN()      (RCC->AHB1ENR) |=(1<<1)
#define GPIOC_PCLK_EN()      (RCC->AHB1ENR) |=(1<<2)
#define GPIOD_PCLK_EN()      (RCC->AHB1ENR) |=(1<<3)
#define GPIOE_PCLK_EN()      (RCC->AHB1ENR) |=(1<<4)
#define GPIOF_PCLK_EN()      (RCC->AHB1ENR) |=(1<<5)
#define GPIOG_PCLK_EN()      (RCC->AHB1ENR) |=(1<<6)
#define GPIOH_PCLK_EN()      (RCC->AHB1ENR) |=(1<<7)

//CLOCK ENABLE MACORS FOR I2C PERIPHERAL
#define I2C1_PCLK_EN()      (RCC->APB1ENR)|=(1 << 21)
#define I2C2_PCLK_EN()      (RCC->APB1ENR)|=(1 << 22)
#define I2C3_PCLK_EN()      (RCC->APB1ENR)|=(1 << 23)

//CLOCK ENABLE MACROS FOR SPIX PERIPHERAL

#define SPI1_PCLK_EN()      (RCC->APB2ENR)|=(1 << 12)
#define SPI4_PCLK_EN()      (RCC->APB2ENR)|=(1 << 13)
#define SPI2_PCLK_EN()      (RCC->APB1ENR)|=(1 << 14)
#define SPI3_PCLK_EN()      (RCC->APB1ENR)|=(1 << 15)

//CLOCK ENABLE MACROS FOR USARTx PERIPHERALS

#define USART1_PCLK_EN()      (RCC->APB2ENR)|=(1 << 4)
#define USART2_PCLK_EN()      (RCC->APB1ENR)|=(1 << 17)
#define USART3_PCLK_EN()      (RCC->APB1ENR)|=(1 << 18)
#define UART4_PCLK_EN()       (RCC->APB1ENR)|=(1 << 19)
#define UART5_PCLK_EN()       (RCC->APB1ENR)|=(1 << 20)
#define USART6_PCLK_EN()       (RCC->APB2ENR)|=(1 << 5)

//CLOCK ENABLE MACROS FOR THE SYSCFG PERIPHERAL

#define SYSCFG_PCLK_EN()      (RCC->APB2ENR)|=(1 << 14)

//CLOCK DISABLE MACROS FOR THE GPIOx PERIPHERAL

#define GPIOA_PCLK_DI()      (RCC->AHB1ENR) &= ~( 1 << 0)
#define GPIOB_PCLK_DI()      (RCC->AHB1ENR) &= ~( 1 << 1)
#define GPIOC_PCLK_DI()      (RCC->AHB1ENR) &= ~( 1 << 2)
#define GPIOD_PCLK_DI()      (RCC->AHB1ENR) &= ~( 1 << 3)
#define GPIOE_PCLK_DI()      (RCC->AHB1ENR) &= ~( 1 << 4)
#define GPIOF_PCLK_DI()      (RCC->AHB1ENR) &= ~( 1 << 5)
#define GPIOG_PCLK_DI()      (RCC->AHB1ENR) &= ~( 1 << 6)
#define GPIOH_PCLK_DI()      (RCC->AHB1ENR) &= ~( 1 << 7)

//CLOCK DISABLE MACORS FOR I2C PERIPHERAL

#define I2C1_PCLK_DI()      (RCC->APB1ENR) &= ~(1 << 21)
#define I2C2_PCLK_DI()      (RCC->APB1ENR) &= ~(1 << 22)
#define I2C3_PCLK_DI()      (RCC->APB1ENR) &= ~(1 << 23)

//CLOCK DISABLE MACROS FOR SPIX PERIPHERAL

#define SPI1_PCLK_DI()      (RCC->APB2ENR) &= ~(1 << 12)
#define SPI4_PCLK_DI()      (RCC->APB2ENR) &= ~(1 << 13)
#define SPI2_PCLK_DI()      (RCC->APB1ENR) &= ~(1 << 14)
#define SPI3_PCLK_DI()      (RCC->APB1ENR) &= ~(1 << 15)

//CLOCK disable MACROS FOR USARTx PERIPHERALS

#define USART1_PCLK_DI()      (RCC->APB2ENR)&= ~(1 << 4)
#define USART2_PCLK_DI()      (RCC->APB1ENR)&= ~(1 << 17)
#define USART3_PCLK_DI()      (RCC->APB1ENR)&= ~(1 << 18)
#define UART4_PCLK_DI()       (RCC->APB1ENR)&= ~(1 << 19)
#define UART5_PCLK_DI()       (RCC->APB1ENR)&= ~(1 << 20)
#define USART6_PCLK_DI()      (RCC->APB2ENR)&= ~(1 << 5)

//CLOCK DISABLE MACROS FOR THE SYSCFG PERIPHERAL

#define SYSCFG_PCLK_DI()      (RCC->APB2ENR)&= ~(1 << 14)

//	Macros to reset GPIOx peripherals

/*
 GPIO Peripheral Reset Macros Explanation

 #define GPIOA_REG_RESET()  do{ ((RCC->AHB1RSTR) |= (1 << 0)); (RCC->AHB1RSTR) &= ~(1 << 0); }while(0)

 These macros are used to reset the GPIO peripherals using the RCC
 (Reset and Clock Control) reset register.

 AHB1RSTR = AHB1 Peripheral Reset Register.

 Each bit in AHB1RSTR corresponds to one peripheral:
 Bit 0 → GPIOA
 Bit 1 → GPIOB
 Bit 2 → GPIOC
 Bit 3 → GPIOD
 Bit 4 → GPIOE
 Bit 5 → GPIOF
 Bit 6 → GPIOG
 Bit 7 → GPIOH

 Reset operation works in two steps:
 1. Set the corresponding bit in RCC->AHB1RSTR to '1'
 → This puts the peripheral into reset state.

 2. Clear the bit back to '0'
 → This releases the peripheral from reset.

 Example:
 GPIOA_REG_RESET();

 This will reset the GPIOA peripheral and bring all its registers
 back to their default reset values.

 The do{ }while(0) construct ensures the macro behaves like a
 single statement when used inside if conditions or other blocks.
 */

#define GPIOA_REG_RESET()               do{ ((RCC->AHB1RSTR) |= (1 << 0));  (RCC->AHB1RSTR) &= ~(1 << 0); }while(0)
#define GPIOB_REG_RESET()               do{ ((RCC->AHB1RSTR) |= (1 << 1));  (RCC->AHB1RSTR) &= ~(1 << 1); }while(0)
#define GPIOC_REG_RESET()               do{ ((RCC->AHB1RSTR) |= (1 << 2));  (RCC->AHB1RSTR) &= ~(1 << 2); }while(0)
#define GPIOD_REG_RESET()               do{ ((RCC->AHB1RSTR) |= (1 << 3));  (RCC->AHB1RSTR) &= ~(1 << 3); }while(0)
#define GPIOE_REG_RESET()               do{ ((RCC->AHB1RSTR) |= (1 << 4));  (RCC->AHB1RSTR) &= ~(1 << 4); }while(0)
#define GPIOF_REG_RESET()               do{ ((RCC->AHB1RSTR) |= (1 << 5));  (RCC->AHB1RSTR) &= ~(1 << 5); }while(0)
#define GPIOG_REG_RESET()               do{ ((RCC->AHB1RSTR) |= (1 << 6));  (RCC->AHB1RSTR) &= ~(1 << 6); }while(0)
#define GPIOH_REG_RESET()               do{ ((RCC->AHB1RSTR) |= (1 << 7));  (RCC->AHB1RSTR) &= ~(1 << 7); }while(0)

//	Macros to reset SPIx peripherals

#define SPI1_REG_RESET()               do{ ((RCC->APB1RSTR) |= (1 << 12));  (RCC->APB1RSTR) &= ~(1 << 12); }while(0)
#define SPI2_REG_RESET()               do{ ((RCC->APB2RSTR) |= (1 << 14));  (RCC->APB2RSTR) &= ~(1 << 14); }while(0)
#define SPI3_REG_RESET()               do{ ((RCC->APB2RSTR) |= (1 << 15));  (RCC->APB2RSTR) &= ~(1 << 15); }while(0)
#define SPI4_REG_RESET()               do{ ((RCC->APB1RSTR) |= (1 << 13));  (RCC->APB1RSTR) &= ~(1 << 13); }while(0)

//	Macros to reset I2Cx peripherals

#define I2C1_REG_RESET()               do{ ((RCC->APB1RSTR) |= (1 << 21));  (RCC->APB1RSTR) &= ~(1 << 21); }while(0)
#define I2C2_REG_RESET()               do{ ((RCC->APB1RSTR) |= (1 << 22));  (RCC->APB2RSTR) &= ~(1 << 22); }while(0)
#define I2C3_REG_RESET()               do{ ((RCC->APB1RSTR) |= (1 << 23));  (RCC->APB2RSTR) &= ~(1 << 23); }while(0)

//Macros to reset usartx peripherals

#define USART1_REG_RESET()               do {   ((RCC->APB2RSTR)  |= (1 << 4));     (RCC->APB2RSTR)   &= ~(1 << 5);}  while(0)
#define USART2_REG_RESET()               do {   ((RCC->APB1RSTR)  |= ~(1 << 17);    (RCC->APB1RSTR)   &= ~(1 << 17);} while(0)
#define USART3_REG_RESET()               do {   ((RCC->APB1RSTR)  |= ~(1 << 18);    (RCC->APB1RSTR)   &= ~(1 << 18);} while(0)
#define UART4_REG_RESET()                do {   ((RCC->APB1RSTR)  |= ~(1 << 19);    (RCC->APB1RSTR)   &= ~(1 << 19);} while(0)
#define UART5_REG_RESET()                do {   ((RCC->APB1RSTR)  |= ~(1 << 20);    (RCC->APB1RSTR)   &= ~(1 << 20);} while(0)
#define USART6_REG_RESET()               do {   ((RCC->APB2RSTR)  |= ~(1 << 5);     (RCC->APB2RSTR)   &= ~(1 << 5 );} while(0)

//RETURN PORT CODE FOR GIVEN GPIOX BASE ADDR
#define GPIO_BASEADDR_TO_CODE(x)     ( (x==GPIOA) ? 0 :\
                                      (x==GPIOB) ? 1 :\
                                      (x==GPIOC) ? 2 :\
                                      (x==GPIOD) ? 3 :\
                              		  (x==GPIOE) ? 4 :\
                                   	  (x==GPIOF) ? 5 :\
                                      (x==GPIOG) ? 6 :\
                                      (x==GPIOH) ? 7 : 0)

//IRQ(interrupt request) Numbers of stm32f446re

#define IRQ_NO_EXTI0       6
#define IRQ_NO_EXTI1       7
#define IRQ_NO_EXTI2       8
#define IRQ_NO_EXTI3       9
#define IRQ_NO_EXTI4       10
#define IRQ_NO_EXTI9_5     23
#define IRQ_NO_EXTI15_10   40

//IRQ numbers for the spi

#define IRQ_NO_SPI1       35
#define IRQ_NO_SPI2       36
#define IRQ_NO_SPI3       51
#define IRQ_NO_SPI4       84

//IRQ Number for the I2C
#define IRQ_NO_I2C1_EV    31
#define IRQ_NO_I2C1_ER    32
#define IRQ_NO_I2C2_EV    33
#define IRQ_NO_I2C2_ER    34
#define IRQ_NO_I2C3_EV    72
#define IRQ_NO_I2C3_ER    73


//IRQ numbers for the USART

#define IRQ_NO_USART1   37
#define IRQ_NO_USART2   38
#define IRQ_NO_USART3   39
#define IRQ_NO_UART4    52
#define IRQ_NO_UART5    53
#define IRQ_NO_USART6   71



//IRQ PRIORITY NUMBERS

#define NVIC_IRQ_PRI0       0
#define NVIC_IRQ_PRI1       1
#define NVIC_IRQ_PRI2       2
#define NVIC_IRQ_PRI3       3
#define NVIC_IRQ_PRI4       4
#define NVIC_IRQ_PRI5       5
#define NVIC_IRQ_PRI6       6
#define NVIC_IRQ_PRI7       7
#define NVIC_IRQ_PRI8       8
#define NVIC_IRQ_PRI9       9
#define NVIC_IRQ_PRI10      10
#define NVIC_IRQ_PRI11      11
#define NVIC_IRQ_PRI12      12
#define NVIC_IRQ_PRI13      13
#define NVIC_IRQ_PRI14      14
#define NVIC_IRQ_PRI15      15



//some generic macros

#define                   ENABLE  1
#define                   DISABLE 0
#define SET               ENABLE
#define RESET             DISABLE
#define GPIO_PIN_SET      SET
#define GPIO_PIN_RESET    RESET
#define FLAG_RESET        RESET
#define FLAG_SET          SET

/************************************************************************
 *Bit Position Defination Of Spi Peripheral  CR1
 *************************************************************************/

#define SPI_CR1_CPHA         0
#define SPI_CR1_CPOL         1
#define SPI_CR1_MSTR         2
#define SPI_CR1_BR           3
#define SPI_CR1_SPE          6
#define SPI_CR1_LSBFIRST     7
#define SPI_CR1_SSI          8
#define SPI_CR1_SSM          9
#define SPI_CR1_RXONLY       10
#define SPI_CR1_DFF          11
#define SPI_CR1_CRC_NEXT     12
#define SPI_CR1_CRC_EN       13
#define SPI_CR1_BIDI_OE      14
#define SPI_CR1_BIDI_MODE    15

/************************************************************************
 *Bit Position Defination Of Spi Peripheral  CR2
 *************************************************************************/

#define SPI_CR2_RXDMAEN        0            //RECEIVE BUFFERR DMA ENABLE
#define SPI_CR2_TXDMAEN        1            //TRANSMIT BUFFERR DMA ENABLE
#define SPI_CR2_SSOE           2            // SS output enable
#define SPI_CR2_FRF            4            // FRAME FORMAT
#define SPI_CR2_ERRIE          5            // Error interrupt enable
#define SPI_CR2_RXNEIE         6            // RX buffer not empty interrupt enable
#define SPI_CR2_TXEIE          7            // Tx buffer empty interrupt enable

// BIT POSITION DEFINATION FOR SPI STATUS REGISTER

#define  SPI_SR_RXNE       0              // Receive buffer not empty IF SET TO 1
#define  SPI_SR_TXE        1              //  IF Transmit buffer IS empty THEN THIS IS SET
#define  SPI_SR_CHSIDE     2              // Channel side
#define  SPI_SR_UDR        3              // Underrun flag
#define  SPI_SR_CRCERR     4              // CRC error flag
#define  SPI_SR_MODF       5              // Mode fault
#define  SPI_SR_OVR        6             // Overrun flag
#define  SPI_SR_BSY        7              // Busy flag
#define  SPI_SR_FRE        8              // Frame Error

// BIT POSITION DEFINATION FOR SPI data REGISTER

#define  SPI_SR_RXNE       0              // Receive buffer not empty
#define  SPI_SR_TXE        1              // Transmit buffer empty
#define  SPI_SR_CHSIDE     2              // Channel side
#define  SPI_SR_UDR        3              // Underrun flag
#define  SPI_SR_CRCERR     4              // CRC error flag
#define  SPI_SR_MODF       5              // Mode fault
#define  SPI_SR_OVR        6             // Overrun flag
#define  SPI_SR_BSY        7              // Busy flag
#define  SPI_SR_FRE        8              // Frame Error

/************************************************************************
 *Bit Position Defination Of I2C Peripheral  CR1
 *************************************************************************/

#define I2C_CR1_PE                 0
#define I2C_CR1_NOSTRETCH          7
#define I2C_CR1_START              8
#define I2C_CR1_STOP               9
#define I2C_CR1_ACK                10
#define I2C_CR1_SWRST              15

/************************************************************************
 *Bit Position Defination Of I2C Peripheral  CR2
 *************************************************************************/

#define I2C_CR2_FREQ              0
#define I2C_CR2_ITERREN           8
#define I2C_CR2_ITEVTEN           9
#define I2C_CR2_ITBUFEN           10

/************************************************************************
 *Bit Position Defination Of I2C Peripheral  SR1
 *************************************************************************/
#define I2C_SR1_SB      0
#define I2C_SR1_ADDR    1
#define I2C_SR1_BTF     2
#define I2C_SR1_ADD10   3
#define I2C_SR1_STOPF   4
#define I2C_SR1_RxNE    6
#define I2C_SR1_TxE     7
#define I2C_SR1_BERR    8
#define I2C_SR1_ARLO    9
#define I2C_SR1_AF      10
#define I2C_SR1_OVR     11
#define I2C_SR1_TIMEOUT 12

/************************************************************************
 *Bit Position Defination Of I2C Peripheral  SR2
 *************************************************************************/

#define I2C_SR2_MSL      0
#define I2C_SR2_BUSY     1
#define I2C_SR2_TRA      2
#define I2C_SR2_GENCALL  4
#define I2C_SR2_DUALF    7

/************************************************************************
 *Bit Position Defination Of I2C Peripheral  CCR
 *************************************************************************/
#define  I2C_CCR_CCR        0
#define  I2C_CCR_DUTY      14
#define  I2C_CCR_FS        15

/************************************************************************
 *Bit Position Defination Of USART Peripheral  SR
 *************************************************************************/
#define USART_SR_PE         0
#define USART_SR_FE         1
#define USART_SR_NF         2
#define USART_SR_ORE        3
#define USART_SR_IDLE       4
#define USART_SR_RXNE       5
#define USART_SR_TC         6
#define USART_SR_TXE        7
#define USART_SR_LBD        8
#define USART_SR_CTS        9

/************************************************************************
 *Bit Position Defination Of USART Peripheral  CR1
 *************************************************************************/
#define USART_CR1_SBK           0
#define USART_CR1_RWU           1
#define USART_CR1_RE            2
#define USART_CR1_TE            3
#define USART_CR1_IDLEIE        4
#define USART_CR1_RXNEIE        5
#define USART_CR1_TCIE          6
#define USART_CR1_TXEIE         7
#define USART_CR1_PEIE          8
#define USART_CR1_PS            9
#define USART_CR1_PCE          10
#define USART_CR1_WAKE         11
#define USART_CR1_M            12
#define USART_CR1_UE           13
#define USART_CR1_OVER8        14

/************************************************************************
 *Bit Position Defination Of USART Peripheral  CR2
 *************************************************************************/

#define USART_CR2_LBDL       `        5
#define USART_CR2_LBDIE               6
#define USART_CR2_Res                7
#define USART_CR2_LBCL               8
#define USART_CR2_CPHA               9
#define USART_CR2_CPOL              10
#define USART_CR2_CLEKEN            11
#define USART_CR2_STOP              12
#define USART_CR2_LINEN             14



/************************************************************************
 *Bit Position Defination Of USART Peripheral  CR3
 *************************************************************************/
#define USART_CR3_EIE                 0
#define USART_CR3_IREN                1
#define USART_CR3_IRLP                2
#define USART_CR3_HDSEL               3
#define USART_CR3_SCEN                4
#define USART_CR3_DMAR       `        5
#define USART_CR3_DMAT                6
#define USART_CR3_RTSE                7
#define USART_CR3_CTSE                8
#define USART_CR3_CPHA                9
#define USART_CR3_CTSIE              10
#define USART_CR3_ONEBIT             11




#define COMMAND_LED_CTRL        0x50
#define COMMAND_SENSOR_READ     0x51
#define COMMAND_LED_READ        0x52
#define COMMAND_PRINT           0x53
#define COMMAND_ID_READ         0x54

#define LED_ON     1
#define LED_OFF    0

#define ANALOG_PIN0       0
#define ANALOG_PIN1       1
#define ANALOG_PIN2       2
#define ANALOG_PIN3       3
#define ANALOG_PIN4       4

#define COMMAND_LED_CTRL    0x50
#define LED_PIN       9
#define LED_ON        1
#define LED_OFF       0

#include "stm32f446xx_gpio_driver.h"
#include "stm32f446xx_spi_driver.h"
#include "stm32f446xx_i2c_driver.h"
#include "stm32f446xx_usart_driver.h"
#include "stm32f446xx_rcc_driver.h"


#endif /* INC_STM32F446XX_H_ */
