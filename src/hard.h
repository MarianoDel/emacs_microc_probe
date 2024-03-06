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

//GPIOB pin0
#define LED    ((GPIOB->ODR & 0x0001) != 0)
#define LED_ON    (GPIOB->BSRR = 0x00000001)
#define LED_OFF    (GPIOB->BSRR = 0x00010000)


// Exported Functions -- Implemented on main -----------------------------------
void Led_On (void);
void Led_Off (void);


#endif /* HARD_H_ */
