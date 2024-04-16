//------------------------------------------------
// ## Internal Test Functions Module
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### TEST_FUNCTIONS.C #########################
//------------------------------------------------

// Includes --------------------------------------------------------------------
#include "test_functions.h"
#include "stm32g0xx.h"
#include "hard.h"
#include "tim.h"
#include "usart.h"
// #include "adc.h"
#include "dma.h"
#include "tim.h"
#include "i2c.h"
// #include "flash_program.h"

#include "screen1.h"


#include <stdio.h>
#include <string.h>



// Externals -------------------------------------------------------------------
extern volatile unsigned short timer_standby;

// Globals ---------------------------------------------------------------------


// Module Private Types & Macros -----------------------------------------------


// Module Private Functions ----------------------------------------------------
void TF_Led (void);
void TF_Pa8 (void);
void TF_Led_Usart1_Tx (void);
void TF_Led_Usart1_TxRx (void);
void TF_Oled_Screen (void);


// Module Functions ------------------------------------------------------------
void TF_Hardware_Tests (void)
{
    // TF_Led ();    //simple led functionality

    // TF_Pa8 ();
    
    // TF_Led_Usart1_Tx ();

    // TF_Led_Usart1_TxRx ();

    TF_Oled_Screen ();

}


void TF_Led (void)
{
    Wait_ms (1000);

    LED_ON;
    while (1);
    // while (1)
    // {
    //     if (Led_Is_On())
    //         Led_Off();
    //     else
    //         Led_On();

    //     Wait_ms(300);
    // }
}


void TF_Pa8 (void)
{
    Wait_ms (1000);

    while (1)
    {
        // if (!timer_standby)
        // {
        //     if (PB0)
        //     {
        //         PB0_OFF;
        //         PA15_OFF;
        //     }
        //     else
        //     {
        //         PB0_ON;
        //         PA15_ON;
        //     }

        //     timer_standby = 300;
        // }

        // if (PA13_INPUT)
        //     PA15_ON;
        // else
        //     PA15_OFF;

        Wait_ms(300);
        PA13_ON;
        Wait_ms(300);
        PA13_OFF;
        
    }
}


void TF_Led_Usart1_Tx (void)
{
    Wait_ms(5000);
    Usart1Config();
    
    while (1)
    {
        LED_ON;
        Usart1Send("Prueba en usart1\n");
        Wait_ms(100);
        LED_OFF;

        Wait_ms(2000);
    }
}


extern volatile unsigned short timer_standby;
void TF_Led_Usart1_TxRx (void)
{
    char buff [30] = { 0 };

    Wait_ms(5000);    
    Usart1Config();
    
    while (1)
    {
        if (!timer_standby)
        {
            timer_standby = 2000;
            Usart1Send("Prueba en usart1\n");
        }

        if (Usart1HaveData())
        {
            Usart1ReadBuffer((unsigned char *) buff, 30);
            Usart1HaveDataReset();
            if (!strncmp(buff, "Prueba en usart1", sizeof("Prueba en usart1") - 1))
            {
                LED_ON;
                Wait_ms(100);
                LED_OFF;
            }
        }
    }
}


extern uint8_t SSD1306_buffer[];
void TF_Oled_Screen (void)
{
    for (int i = 0; i < 3; i++)
    {
        LED_ON;
        Wait_ms(300);
        LED_OFF;
        Wait_ms(300);
    }
    
    // OLED Init
    I2C2_Init();
    Wait_ms(2000);

    //primer pantalla
    LED_ON;
    SCREEN_Init();

    int j = 0;
    char lbuf[10] = { 0 };
    while (1)
    {
        // estas 3 juntas ok
        // SCREEN_Clear ();        
        // SCREEN_Text2_Line1 ("Probe012345");
        // SCREEN_Text2_Line2 ("0123456789");    // oka        

        // estas 3 juntas err en vuelta 3 o 4 (oka con Wait_ms)
        // SCREEN_Clear ();        
        // SCREEN_Text2_Line1 ("Probe01234");
        // SCREEN_Text2_Line2 ("0123456789");    // oka        
        
        // estas 3 juntas ok
        // SCREEN_Clear ();        
        // SCREEN_Text2_Line1 ("Probe0123");
        // SCREEN_Text2_Line2 ("012345678");    // oka
        
        if (j < 1000)
            j++;
        else
            j = 0;
        
        sprintf(lbuf, "cnt: %d", j);
        SCREEN_Clear ();
        SCREEN_Text2_Line1 ("Probe01234");
        SCREEN_Text2_Line2 (lbuf);    // err
                
        for (int i = 0; i < 30; i++)
        {
            if (LED)
                LED_OFF;
            else
                LED_ON;

            Wait_ms(200);
        }

        Wait_ms(1000);
        Wait_ms(1000);        
    }
    
}
//--- end of file ---//
