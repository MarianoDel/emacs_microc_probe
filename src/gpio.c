//---------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    STM32F030
// ##
// #### GPIO.C ################################
//---------------------------------------------

// Includes --------------------------------------------------------------------
#include "gpio.h"
#include "stm32g0xx.h"
#include "hard.h"

// Ports Configs ---------------------------------------------------------------
#define GPIOA_ENABLE
#define GPIOB_ENABLE
//#define GPIOF_ENABLE

// #define WITH_EXTI

// - Ports Clocks
#define GPIOA_CLK    (RCC->IOPENR & 0x00000001)
#define GPIOA_CLK_ON    (RCC->IOPENR |= 0x00000001)
#define GPIOA_CLK_OFF    (RCC->IOPENR &= ~0x00000001)

#define GPIOB_CLK    (RCC->IOPENR & 0x00000002)
#define GPIOB_CLK_ON    (RCC->IOPENR |= 0x00000002)
#define GPIOB_CLK_OFF    (RCC->IOPENR &= ~0x00000002)

#define GPIOC_CLK    (RCC->IOPENR & 0x00000004)
#define GPIOC_CLK_ON    (RCC->IOPENR |= 0x00000004)
#define GPIOC_CLK_OFF    (RCC->IOPENR &= ~0x00000004)

#define GPIOD_CLK    (RCC->IOPENR & 0x00000008)
#define GPIOD_CLK_ON    (RCC->IOPENR |= 0x00000008)
#define GPIOD_CLK_OFF    (RCC->IOPENR &= ~0x00000008)

#define GPIOF_CLK    (RCC->IOPENR & 0x00000020)
#define GPIOF_CLK_ON    (RCC->IOPENR |= 0x00000020)
#define GPIOF_CLK_OFF    (RCC->IOPENR &= ~0x00000020)

// #define SYSCFG_CLK    (RCC->APBENR2 & 0x00000001)
// #define SYSCFG_CLK_ON    (RCC->APBENR2 |= 0x00000001)
// #define SYSCFG_CLK_OFF    (RCC->APBENR2 &= ~0x00000001)


// Module Functions ------------------------------------------------------------
void GPIO_Config (void)
{
    //--- MODER ---//
    //00: Input mode 
    //01: General purpose output mode
    //10: Alternate function mode
    //11: Analog mode (reset state)

    //--- OTYPER ---//
    //These bits are written by software to configure the I/O output type.
    //0: Output push-pull (reset state)
    //1: Output open-drain

    //--- ORSPEEDR ---//
    //These bits are written by software to configure the I/O output speed.
    //x0: Low speed.
    //01: Medium speed.
    //11: High speed.
    //Note: Refer to the device datasheet for the frequency.

    //--- PUPDR ---//
    //These bits are written by software to configure the I/O pull-up or pull-down
    //00: No pull-up, pull-down
    //01: Pull-up
    //10: Pull-down
    //11: Reserved

    unsigned long temp;

    //--- GPIO A ---//
#ifdef GPIOA_ENABLE    
    if (!GPIOA_CLK)
        GPIOA_CLK_ON;
    
    temp = GPIOA->MODER;    // 2 bits por pin
    temp &= 0xF03FFFFF;    // PA13 output    
    temp |= 0x06800000;    // PA11 PA12 alternative
    GPIOA->MODER = temp;

    temp = GPIOA->OTYPER;    //1 bit por pin
    temp &= 0xFFFFC7FF;    // PA13 PA12 PA11 open drain
    temp |= 0x00003800;
    GPIOA->OTYPER = temp;
    
    temp = GPIOA->OSPEEDR;    // 2 bits por pin
    temp &= 0xFFFFFFFF;    // PA0 speed
    temp |= 0x00000000;
    GPIOA->OSPEEDR = temp;

    temp = GPIOA->PUPDR;	//2 bits por pin
    temp &= 0xFC3FFFFF;    // PA11 PA12 pullup
    temp |= 0x01400000;
    GPIOA->PUPDR = temp;
#endif    //GPIOA_ENABLE
    
    //--- GPIO B ---//
#ifdef GPIOB_ENABLE    
    if (!GPIOB_CLK)
        GPIOB_CLK_ON;
    
    temp = GPIOB->MODER;    //2 bits por pin
    temp &= 0xFFFFFFFF;    //PB0 output; PB6 PB7 alternative on the driver
    temp |= 0x00000000;        
    // temp &= 0xFFFF0FFC;    //PB0 output; PB6 PB7 alternative
    // temp |= 0x0000A001;
    GPIOB->MODER = temp;

    temp = GPIOB->OTYPER;	//1 bit por pin
    temp &= 0xFFFFFFFF;
    temp |= 0x00000000;
    GPIOB->OTYPER = temp;

    temp = GPIOB->OSPEEDR;	//2 bits por pin
    temp &= 0xFFFFFFFC;        //PB0 & PB6 low speed
    temp |= 0x00000000;
    GPIOB->OSPEEDR = temp;

    temp = GPIOB->PUPDR;	//2 bits por pin
    temp &= 0xFFFFFFFF;
    temp |= 0x00000000;
    // temp &= 0xFFFFF03F;    //PB3 PB4 PB5 pullup
    // temp |= 0x00000540;
    GPIOB->PUPDR = temp;
#endif    //GPIOB_ENABLE
    
    //--- GPIO C ---//
#ifdef GPIOC_ENABLE
    if (!GPIOC_CLK)
        GPIOC_CLK_ON;

    temp = GPIOC->MODER;	//2 bits por pin
    temp &= 0xFFFF0F3C;		//PB0 analog input; PB3 input PB6 PB7 alternative
    temp |= 0x0000A003;
    GPIOC->MODER = temp;

    temp = GPIOC->OTYPER;	//1 bit por pin
    temp &= 0xFFFFFFFF;
    temp |= 0x00000000;
    GPIOC->OTYPER = temp;

    temp = GPIOC->OSPEEDR;	//2 bits por pin
    temp &= 0xFFFF0FFF;
    temp |= 0x00000000;		//low speed
    GPIOC->OSPEEDR = temp;

    temp = GPIOC->PUPDR;	//2 bits por pin
    temp &= 0xFFFFFFFF;
    temp |= 0x00000000;
    GPIOC->PUPDR = temp;
#endif    // GPIOC_ENABLE

    
#ifdef WITH_EXTI
    //Interrupt on PA8
    // if (!SYSCFG_CLK)
    //     SYSCFG_CLK_ON;

    // SYSCFG->EXTICR[0] = 0x00000000; //Select Port A
    // SYSCFG->EXTICR[1] = 0x00000000; //Select Port A
    EXTI->IMR1 |= 0x0100;    //Corresponding mask bit for interrupts PA8
    EXTI->EMR1 |= 0x0000;    //Corresponding mask bit for events
    EXTI->RTSR1 |= 0x0100;    //Pin Interrupt line on rising edge PA8
    EXTI->FTSR1 |= 0x0100;    //Pin Interrupt line on falling edge PA8

    NVIC_EnableIRQ(EXTI4_15_IRQn);
    NVIC_SetPriority(EXTI4_15_IRQn, 3);
#endif
}


inline void EXTIOff (void)
{
    EXTI->IMR1 &= ~0x00000100;
}


inline void EXTIOn (void)
{
    EXTI->IMR1 |= 0x00000100;
}

//--- end of file ---//
