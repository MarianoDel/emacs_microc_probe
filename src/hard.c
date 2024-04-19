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


// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------


// Module Private Functions ----------------------------------------------------



// Module Functions ------------------------------------------------------------
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


unsigned char Start_Btn (void)
{
    unsigned int temp;
    unsigned char last_led_status = 0;
    unsigned char pa13_input_state = 0;

    if (Led_Is_On())
        last_led_status = 1;

    Wait_ms (10);
    // Led_On();
    // Wait_ms (1);
    
    // PA13 to input
    temp = GPIOA->MODER;    // 2 bits por pin
    temp &= 0xF3FFFFFF;    // PA13 input
    temp |= 0x00000000;
    GPIOA->MODER = temp;

    Wait_ms(1);
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
    
    return pa13_input_state;
}


//--- end of file ---//


