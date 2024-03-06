//---------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    STM32F030
// ##
// #### TIM.C ################################
//---------------------------------------------

// Includes --------------------------------------------------------------------
#include "tim.h"
#include "stm32g0xx.h"


#define ENABLE_TIM1    (TIM1->CR1 |= TIM_CR1_CEN)
#define DISABLE_TIM1    (TIM1->CR1 &= ~TIM_CR1_CEN)

#define ENABLE_TIM3    (TIM3->CR1 |= TIM_CR1_CEN)
#define DISABLE_TIM3    (TIM3->CR1 &= ~TIM_CR1_CEN)


#define RCC_TIM1_CLK    (RCC->APB2ENR & 0x00000800)
#define RCC_TIM1_CLK_ON    (RCC->APB2ENR |= 0x00000800)
#define RCC_TIM1_CLK_OFF    (RCC->APB2ENR &= ~0x00000800)

#define RCC_TIM3_CLK    (RCC->APB1ENR & 0x00000002)
#define RCC_TIM3_CLK_ON    (RCC->APB1ENR |= 0x00000002)
#define RCC_TIM3_CLK_OFF    (RCC->APB1ENR &= ~0x00000002)

#define RCC_TIM6_CLK    (RCC->APB1ENR & 0x00000010)
#define RCC_TIM6_CLK_ON    (RCC->APB1ENR |= 0x00000010)
#define RCC_TIM6_CLK_OFF    (RCC->APB1ENR &= ~0x00000010)

#define RCC_TIM14_CLK    (RCC->APB1ENR & 0x00000100)
#define RCC_TIM14_CLK_ON    (RCC->APB1ENR |= 0x00000100)
#define RCC_TIM14_CLK_OFF    (RCC->APB1ENR &= ~0x00000100)

#define RCC_TIM15_CLK    (RCC->APB2ENR & 0x00010000)
#define RCC_TIM15_CLK_ON    (RCC->APB2ENR |= 0x00010000)
#define RCC_TIM15_CLK_OFF    (RCC->APB2ENR &= ~0x00010000)

#define RCC_TIM16_CLK    (RCC->APB2ENR & 0x00020000)
#define RCC_TIM16_CLK_ON    (RCC->APB2ENR |= 0x00020000)
#define RCC_TIM16_CLK_OFF    (RCC->APB2ENR &= ~0x00020000)

#define RCC_TIM17_CLK    (RCC->APB2ENR & 0x00040000)
#define RCC_TIM17_CLK_ON    (RCC->APB2ENR |= 0x00040000)
#define RCC_TIM17_CLK_OFF    (RCC->APB2ENR &= ~0x00040000)


#define EnablePreload_Mosfet_HighLeft    (TIM3->CCMR1 |= TIM_CCMR1_OC2PE)
#define DisablePreload_Mosfet_HighLeft    (TIM3->CCMR1 &= ~TIM_CCMR1_OC2PE)
#define EnablePreload_Mosfet_HighRight    (TIM3->CCMR2 |= TIM_CCMR2_OC4PE)
#define DisablePreload_Mosfet_HighRight    (TIM3->CCMR2 &= ~TIM_CCMR2_OC4PE)


#define LOW_LEFT(X)     Update_TIM3_CH1(X)
#define HIGH_LEFT(X)    Update_TIM3_CH2(X)
#define LOW_RIGHT(X)     Update_TIM3_CH3(X)
#define HIGH_RIGHT(X)    Update_TIM3_CH4(X)


// Externals -------------------------------------------------------------------
extern volatile unsigned char timer_1seg;
extern volatile unsigned short timer_led_comm;
extern volatile unsigned short timer_standby;


// Globals ---------------------------------------------------------------------
volatile unsigned short wait_ms_var = 0;
volatile unsigned short timer_1000 = 0;


// Module Private Functions ----------------------------------------------------


// Module Functions ------------------------------------------------------------
void Wait_ms (unsigned short wait)
{
    wait_ms_var = wait;

    while (wait_ms_var);
}


void TIM_Timeouts (void)
{
    if (wait_ms_var)
        wait_ms_var--;
    
}


// void TIM_3_Init (void)
// {
//     if (!RCC_TIM3_CLK)
//         RCC_TIM3_CLK_ON;

//     //Configuracion del timer.
//     TIM3->ARR = 100; //1m
//     TIM3->CNT = 0;
//     TIM3->PSC = 479;
//     TIM3->EGR = TIM_EGR_UG;

//     // Enable timer ver UDIS
//     TIM3->DIER |= TIM_DIER_UIE;
//     TIM3->CR1 |= TIM_CR1_CEN;

//     NVIC_EnableIRQ(TIM3_IRQn);
//     NVIC_SetPriority(TIM3_IRQn, 6);
    
// }


// void TIM3_IRQHandler (void)	
// {
//     if (timer_1seg)
//     {
//         if (timer_1000)
//             timer_1000--;
//         else
//         {
//             timer_1seg--;
//             timer_1000 = 1000;
//         }
//     }

//     if (wait_ms_var)
//         wait_ms_var--;

//     if (timer_led_comm)
//         timer_led_comm--;

//     if (timer_standby)
//         timer_standby--;

//     //bajar flag
//     if (TIM3->SR & 0x01)	//bajo el flag
//         TIM3->SR = 0x00;
// }


// void TIM_1_Init (void)
// {
//     if (!RCC_TIM1_CLK)
//         RCC_TIM1_CLK_ON;

//     //Configuracion del timer.
//     //TIM1->CR1 |= TIM_CR1_OPM;        //clk int / 1; upcounting; one pulse
//     TIM1->CR1 = 0x00;        //clk int / 1;
//     // TIM1->CR2 |= TIM_CR2_MMS_1;        //UEV -> TRG0
//     TIM1->CR2 = 0x00;
//     //TIM1->SMCR |= TIM_SMCR_MSM | TIM_SMCR_SMS_2 | TIM_SMCR_SMS_1 | TIM_SMCR_TS_1;    //link timer3
//     TIM1->SMCR = 0x0000;

//     TIM1->CCMR1 = 0x0060;            //CH1 output PWM mode 1 (channel active TIM1->CNT < TIM1->CCR1)    
//     TIM1->CCMR2 = 0x0000;            //
//     TIM1->CCER |= TIM_CCER_CC1E;
    
//     TIM1->BDTR |= TIM_BDTR_MOE;
//     // TIM1->ARR = 1023;                //cada tick 20.83ns
//     // TIM1->ARR = DUTY_100_PERCENT;                //cada tick 20.83ns

//     TIM1->CNT = 0;
//     TIM1->PSC = 0;

//     // unsigned int temp = 0;    
//     // temp = GPIOA->AFR[1];
//     // temp &= 0xFFFFFFF0;
//     // temp |= 0x00000002;    //PA8 -> AF2
//     // GPIOA->AFR[1] = temp;
    
//     // Enable timer ver UDIS
//     //TIM1->DIER |= TIM_DIER_UIE;
//     TIM1->CR1 |= TIM_CR1_CEN;

//     TIM1->CCR1 = 0;
//     TIM1->CCR3 = 0;    
// }




// void TIM_6_Init (void)
// {
//     if (!RCC_TIM6_CLK)
//         RCC_TIM6_CLK_ON;

//     //Configuracion del timer.
//     TIM6->CR1 = 0x00;		//clk int / 1; upcounting
//     TIM6->PSC = 47;			//tick cada 1us
//     TIM6->ARR = 0xFFFF;			//para que arranque
//     //TIM6->CR1 |= TIM_CR1_CEN;
// }

// void TIM6Enable (void)
// {
//     TIM6->CR1 |= TIM_CR1_CEN;
// }

// void TIM6Disable (void)
// {
//     TIM6->CR1 &= ~TIM_CR1_CEN;
// }

// void TIM14_IRQHandler (void)	//100uS
// {

//     if (TIM14->SR & 0x01)
//         //bajar flag
//         TIM14->SR = 0x00;
// }

// void TIM_14_Init (void)
// {
//     unsigned int temp;

//     if (!RCC_TIM14_CLK)
//         RCC_TIM14_CLK_ON;

//     TIM14->CCMR1 = 0x0060;            //CH1 output PWM mode 1
//     TIM14->CCER |= TIM_CCER_CC1E;    //CH1 enable on pin active high
//     //TIM3->CCER |= TIM_CCER_CC2E | TIM_CCER_CC2P;    //CH2 enable on pin active high
//     TIM14->PSC = 3;			//tick cada 83.33n
//     TIM14->ARR = 1023;    //freq 11.73KHz

//     //Configuracion del timer.
//     TIM14->EGR |= 0x0001;

//     //Configuracion Pin PB1
//     temp = GPIOB->AFR[0];
//     temp &= 0xFFFFFF0F;
//     temp |= 0x00000000;	//PB1 -> AF0
//     GPIOB->AFR[0] = temp;

//     TIM14->CR1 |= TIM_CR1_CEN;

// }

// void TIM16_IRQHandler (void)	//es one shoot
// {
//     //SendDMXPacket(PCKT_UPDATE);

//     if (TIM16->SR & 0x01)
//         //bajar flag
//         TIM16->SR = 0x00;
// }

// void TIM_16_Init (void)
// {
//     if (!RCC_TIM16_CLK)
//         RCC_TIM16_CLK_ON;

//     //Configuracion del timer.
//     TIM16->CR1 = 0x00;		//clk int / 1; upcounting; uev
//     TIM16->ARR = 0xFFFF;
//     TIM16->CNT = 0;
//     TIM16->PSC = 47;			//tick 1us
//     TIM16->EGR = TIM_EGR_UG;

// }


// void OneShootTIM16 (unsigned short a)
// {
//     TIM16->ARR = a;
//     TIM16->CR1 |= TIM_CR1_CEN;
// }


// void TIM16Enable (void)
// {
//     TIM16->CR1 |= TIM_CR1_CEN;
// }


// void TIM16Disable (void)
// {
//     TIM16->CR1 &= ~TIM_CR1_CEN;
// }


// void TIM_17_Init (void)
// {
//     if (!RCC_TIM17_CLK)
//         RCC_TIM17_CLK_ON;

//     //Configuracion del timer.
//     TIM17->ARR = 0xFFFF;
//     TIM17->CNT = 0;
//     TIM17->PSC = 47;

//     // Enable timer interrupt ver UDIS
//     TIM17->DIER |= TIM_DIER_UIE;
//     TIM17->CR1 |= TIM_CR1_URS;	//solo int cuando hay overflow y one shot

//     NVIC_EnableIRQ(TIM17_IRQn);
//     NVIC_SetPriority(TIM17_IRQn, 8);
// }


// void TIM17_IRQHandler (void)
// {
//     if (TIM17->SR & 0x01)
//     {
//         // SYNC_Zero_Crossing_Handler();
//         TIM17->SR = 0x00;    //flag down
//     }    
// }


// void TIM17Enable (void)
// {
//     TIM17->CR1 |= TIM_CR1_CEN;
// }


// void TIM17Disable (void)
// {
//     TIM17->CR1 &= ~TIM_CR1_CEN;
// }


//--- end of file ---//
