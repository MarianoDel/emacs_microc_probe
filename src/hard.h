//----------------------------------------------------
// #### PROJECT: Magnet Antennas - Custom Board ####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### HARD.H #######################################
//----------------------------------------------------

// Define to prevent recursive inclusion ---------------------------------------
#ifndef HARD_H_
#define HARD_H_

// #include "stm32g0xx.h"

//-- Defines For Configuration -------------------
//---- Configuration for Hardware Versions -------
// #define VER_2_0		//juego de 2 placas chicas
// #define VER_1_1		//placa rectangular larga

//-------- Clock Frequency ------------------------------------
#define CLOCK_FREQ_64_MHZ    // Modify this also on system_stm32g0xx.c SystemInit
// #define CLOCK_FREQ_16_MHZ    // Modify this also on system_stm32g0xx.c SystemInit


//-- End Of Defines For Configuration ---------------

// Exported Types Constants & Macros  ------------------------------------------
//GPIOA pin0

//GPIOA pin1
// #define CTRL_BKL    ((GPIOA->ODR & 0x0002) != 0)
// #define CTRL_BKL_ON    (GPIOA->BSRR = 0x00000002)
// #define CTRL_BKL_OFF    (GPIOA->BSRR = 0x00020000)

//GPIOA pin2    Usart2 tx
//GPIOA pin3	Usart2 rx
//GPIOA pin2
//GPIOA pin3	tx - rx usart

//GPIOA pin4
//GPIOA pin5
//GPIOA pin6
//GPIOA pin7    Lcd interface

//GPIOA pin8
#define PA8    ((GPIOA->ODR & 0x0100) != 0)
#define PA8_ON    (GPIOA->BSRR = 0x00000100)
#define PA8_OFF    (GPIOA->BSRR = 0x01000000)

//GPIOA pin11
#define PA11    ((GPIOA->ODR & 0x0800) != 0)
#define PA11_ON    (GPIOA->BSRR = 0x00000800)
#define PA11_OFF    (GPIOA->BSRR = 0x08000000)

//GPIOA pin12
#define PA12    ((GPIOA->ODR & 0x1000) != 0)
#define PA12_ON    (GPIOA->BSRR = 0x00001000)
#define PA12_OFF    (GPIOA->BSRR = 0x10000000)

//GPIOA pin13
#define LED    (!(PA13))
#define LED_ON    PA13_OFF
#define LED_OFF    PA13_ON
#define PA13    ((GPIOA->ODR & 0x2000) != 0)
#define PA13_ON    (GPIOA->BSRR = 0x00002000)
#define PA13_OFF    (GPIOA->BSRR = 0x20000000)
#define PA13_INPUT    ((GPIOA->IDR & 0x2000) == 0)

//GPIOA pin15
#define PA15    ((GPIOA->ODR & 0x8000) != 0)
#define PA15_ON    (GPIOA->BSRR = 0x00008000)
#define PA15_OFF    (GPIOA->BSRR = 0x80000000)


//GPIOB pin0
#define PB0    ((GPIOB->ODR & 0x0001) != 0)
#define PB0_ON    (GPIOB->BSRR = 0x00000001)
#define PB0_OFF    (GPIOB->BSRR = 0x00010000)

//GPIOB pin5
#define PB5    ((GPIOB->ODR & 0x0020) != 0)
#define PB5_ON    (GPIOB->BSRR = 0x00000020)
#define PB5_OFF    (GPIOB->BSRR = 0x00200000)


// Exported Functions -- Implemented on main -----------------------------------
void Hard_Timeouts (void);
void Led_On (void);
void Led_Off (void);
unsigned char Led_Is_On (void);
    
unsigned char Start_Btn (void);

#endif /* HARD_H_ */
