//---------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    STM32F103
// ##
// #### TIM.H ################################
//---------------------------------------------
#ifndef _TIM_H_
#define _TIM_H_

//--- Incuded for help defines ---//
#include "hard.h"
#include "stm32g0xx.h"


// Exported Types Constants and Macros -----------------------------------------


// Module Exported Functions ---------------------------------------------------
void Wait_ms (unsigned short wait);
void TIM_Timeouts (void);

void TIM_1_Init(void);
void Update_TIM1_CH1 (unsigned short);
void Update_TIM1_CH3 (unsigned short);

void TIM_3_Init(void);
void TIM3_IRQHandler (void);
void Update_TIM3_CH1 (unsigned short);
void Update_TIM3_CH2 (unsigned short);
void Update_TIM3_CH3 (unsigned short);
void Update_TIM3_CH4 (unsigned short);

//Only for C8 Devices
void TIM_6_Init (void);
void TIM6Enable (void);
void TIM6Disable (void);

void TIM_14_Init(void);
void TIM14_IRQHandler (void);

void TIM_16_Init(void);
void TIM16_IRQHandler (void);
void OneShootTIM16 (unsigned short);
void TIM16Enable (void);
void TIM16Disable (void);

void TIM_17_Init(void);
void TIM17_IRQHandler (void);
void TIM17Enable (void);
void TIM17Disable (void);

// void Wait_ms (unsigned short wait);

#endif    /* _TIM_H_ */

//--- end of file ---//

