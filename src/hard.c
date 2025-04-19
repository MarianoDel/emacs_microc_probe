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
volatile unsigned short hard_button_start_timer = 0;


// Module Private Functions ----------------------------------------------------
void Hard_Delay_us (void);


// Module Functions ------------------------------------------------------------
void Hard_Timeouts (void)
{
    if (hard_button_start_timer)
	hard_button_start_timer--;
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


unsigned char Start_Btn (void)
{
    unsigned int temp;
    unsigned char last_led_status = 0;
    unsigned char pa13_input_state = 0;
        
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

    return pa13_input_state;
}


unsigned short hard_start_cnt = 0;
unsigned short hard_change_cnt = 0;
unsigned char hard_start_last_edge = 0;
unsigned char hard_button_start = 0;
unsigned char hard_button_change =0;
void Start_Btn_Check_Update (void)
{
    if (hard_button_start_timer)
	return;

    hard_button_start_timer = 10;
    if (Start_Btn())
    {
	if (hard_start_cnt < 5)
	    hard_start_cnt++;

	if (hard_start_cnt > 2)
	    hard_start_last_edge = 1;

	if (hard_change_cnt < 300)
	    hard_change_cnt++;
	else
	{
	    hard_start_cnt = 0;
	    hard_start_last_edge = 0;
	    hard_change_cnt = 0;
	    hard_button_change = 1;
	}	
    }
    else
    {
	if (hard_start_cnt)
	    hard_start_cnt--;

	if (!hard_start_cnt)
	{
	    if (hard_start_last_edge)
		hard_button_start = 1;
	    else
		hard_button_start = 0;

	    hard_start_last_edge = 0;
	}

	if (hard_change_cnt < 250)
	    hard_button_change = 0;

	if (hard_change_cnt > 5)
	    hard_change_cnt -= 5;
	else
	    hard_change_cnt = 0;
	
    }
}


unsigned char Start_Btn_Check_Start (void)
{
    return hard_button_start;
}


unsigned char Start_Btn_Check_Change (void)
{
    return hard_button_change;
}


void Start_Btn_Check_Change_Reset (void)
{
    hard_start_cnt = 0;
    hard_button_start = 0;
    hard_change_cnt = 0;
    hard_button_change = 0;
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


