//-------------------------------------------------------
// #### PROJECT: Magnet Antenna  G030 - Custom Board ####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### MAIN.C ##########################################
//-------------------------------------------------------

// Includes --------------------------------------------------------------------
#include "stm32g0xx.h"
#include "hard.h"
#include "gpio.h"
#include "comms.h"
#include "usart.h"
#include "tim.h"
// #include "adc.h"
// #include "dma.h"
#include "test_functions.h"
#include "manager.h"

#include "i2c.h"
#include "screen1.h"

#include <string.h>
#include <stdio.h>



// Externals -------------------------------------------------------------------
// -- Externals for the timer module -----------------------
volatile unsigned char timer_1seg = 0;
volatile unsigned short timer_standby = 0;


// Globals ---------------------------------------------------------------------
// Config Probes Names and Features
const char s_probe [] = { "Probe1" };

// const char s_probe [] = { "probe1,Std Double,Link\r\n" };




// Module Private Functions ----------------------------------------------------
void SysTickError (void);
void TimingDelay_Decrement (void);


//-------------------------------------------//
// @brief  Main program.
// @param  None
// @retval None
//------------------------------------------//
int main(void)
{
    //GPIO Configuration.
    GPIO_Config();

    //Start the SysTick Timer
#if (defined CLOCK_FREQ_64_MHZ)
    if (SysTick_Config(64000))
        SysTickError();
#elif (defined CLOCK_FREQ_16_MHZ)
    if (SysTick_Config(16000))
        SysTickError();
#else
#error "No Clock Frequency defined on hard.h"
#endif
    
    //--- Test Hardware Functions ---
    // TF_Hardware_Tests ();    
    //--- End of Test Hardware Functions ---    
    
    // Delay for pins alternative
    for (int i = 0; i < 3; i++)
    {
        LED_ON;
        Wait_ms(300);
        LED_OFF;
        Wait_ms(300);
    }

    // Start the usart1
    Usart1Config();

    // Start i2c and oled screen
    I2C2_Init();

    //primer pantalla
    LED_ON;
    SCREEN_Init();
    LED_OFF;
    
    SCREEN_Clear ();        
    SCREEN_Text2_Line1 ("Infinity  ");    
    SCREEN_Text2_Line2 ("  Clinics ");
    Wait_ms(1000);

    //--- Main loop ---//
    while(1)
    {
        // SCREEN_Text2_BlankLine1();
        // SCREEN_Text2_Line1 ("Probe1 ch1");
        
        // Production Program
        Manager ((char *) s_probe);
    }

    return 0;
}


// Module Auxiliary Functions --------------------------------------------------
void SysTickError (void)
{
    //Capture systick error...
    while (1)
    {
        if (LED)
            LED_OFF;
        else
            LED_ON;

        for (unsigned char i = 0; i < 255; i++)
        {
            asm ("nop \n\t"
                 "nop \n\t"
                 "nop \n\t" );
        }
    }
}


void TimingDelay_Decrement(void)
{
    TIM_Timeouts ();

    Manager_Timeouts ();
    
    if (timer_standby)
        timer_standby--;

}


//--- end of file ---//
