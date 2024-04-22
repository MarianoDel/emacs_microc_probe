//---------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    STM32G030
// ##
// #### HARD.C ################################
//---------------------------------------------

// Includes --------------------------------------------------------------------
#include "hard.h"
#include "stm32g0xx.h"
#include "tim.h"


// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------
volatile unsigned short pa13_input_timer = 0;


// Module Private Functions ----------------------------------------------------
void Hard_Delay_us (void);


// Module Functions ------------------------------------------------------------
void Hard_Timeouts (void)
{
    if (pa13_input_timer)
        pa13_input_timer--;
}


void Led_On (void)
{
    LED_ON;
}


void Led_Off (void)
{
    LED_OFF;
}


unsigned char Led_Is_On (void)
{
    return LED;
}


unsigned char Pa13_Input (void)
{
    return PA13_INPUT;
}


unsigned char last_pa13_input_state = 0;
unsigned char Start_Btn (void)
{
    unsigned int temp;
    unsigned char last_led_status = 0;
    unsigned char pa13_input_state = 0;
    
    if (pa13_input_timer)
        return last_pa13_input_state;

    pa13_input_timer = 10;    // check start button every 10ms
    
    if (Led_Is_On())
        last_led_status = 1;

    // PA13 to input
    temp = GPIOA->MODER;    // 2 bits por pin
    temp &= 0xF3FFFFFF;    // PA13 input
    temp |= 0x00000000;
    GPIOA->MODER = temp;

    // Wait_ms(1);
    Hard_Delay_us ();
    pa13_input_state = Pa13_Input ();
    
    temp = GPIOA->MODER;    // 2 bits por pin
    temp &= 0xF3FFFFFF;    // PA13 output
    temp |= 0x04000000;
    GPIOA->MODER = temp;

    // get back led status
    if (last_led_status)
        Led_On ();
    else
        Led_Off ();

    last_pa13_input_state = pa13_input_state;
    return pa13_input_state;
}


void Hard_Delay_us (void)
{
    for (int i = 0; i < 1000; i++)
    {
        asm ("nop \n\t"
             "nop \n\t"
             "nop \n\t" );
    }
}
//--- end of file ---//


