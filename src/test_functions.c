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
#include "adc.h"
#include "dma.h"
#include "tim.h"
// #include "flash_program.h"
#include "temp_sensor.h"


#include <stdio.h>
#include <string.h>



// Externals -------------------------------------------------------------------

// - for ADC
extern volatile unsigned short adc_ch [];

// Globals ---------------------------------------------------------------------


// Module Private Types & Macros -----------------------------------------------


// Module Private Functions ----------------------------------------------------
void TF_Led (void);
void TF_Led_Usart1_Tx (void);
void TF_Led_Usart1_TxRx (void);
void TF_Temp_Channel (void);
void TF_ts_cal1 (void);
void TF_Temperature (void);



// Module Functions ------------------------------------------------------------
void TF_Hardware_Tests (void)
{
    // TF_Led ();    //simple led functionality
    
    // TF_Led_Usart1_Tx ();

    // TF_Led_Usart1_TxRx ();

    // TF_ts_cal1 ();

    // TF_Temp_Channel ();

    TF_Temperature ();
    
}


void TF_Led (void)
{
    while (1)
    {
        if (LED)
            LED_OFF;
        else
            LED_ON;

        Wait_ms(300);
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


void TF_ts_cal1 (void)
{
    char s_temp [40] = { 0 };

    // factory calibration of temp sensor
    unsigned short ts_cal1 = *((uint16_t*)0x1FFF75A8);
    
    Wait_ms (2000);
    Usart1Config();
    
    while (1)
    {
        Wait_ms (1900);
        Led_On();
        sprintf(s_temp, "ts_cal1: %d\n", ts_cal1);

        Usart1Send(s_temp);
        Wait_ms(100);
        Led_Off();
    }
}


void TF_Temp_Channel (void)
{
    char s_temp [40] = { 0 };

    // factory calibration of temp sensor
    unsigned short ts_cal1 = *((uint16_t*)0x1FFF75A8);
    
    // Delay for pins alternative and start the usart1
    Wait_ms(2000);
    Usart1Config();

    // Init ADC and DMA
    AdcConfig();
    DMAConfig();
    DMA1_Channel1->CCR |= DMA_CCR_EN;
    ADC1->CR |= ADC_CR_ADSTART;
    
    while (1)
    {
        Wait_ms (1900);
        Led_On();
        sprintf(s_temp, "ts_cal1: %d temp_ch: %d\n",
                ts_cal1,
                Temp_Channel);

        Usart1Send(s_temp);
        Wait_ms(100);
        Led_Off();
    }
}


void TF_Temperature (void)
{
    char s_temp [40] = { 0 };

    // factory calibration of temp sensor
    unsigned short ts_cal1 = *((uint16_t*)0x1FFF75A8);
    
    // Delay for pins alternative and start the usart1
    Wait_ms(2000);
    Usart1Config();

    // Init ADC and DMA
    AdcConfig();
    DMAConfig();
    DMA1_Channel1->CCR |= DMA_CCR_EN;
    ADC1->CR |= ADC_CR_ADSTART;

    // convert ref_30 from 3V to 3.3V
    int calc = ts_cal1 * 30;
    calc = calc / 33;
    Temp_Sensor_SetRef((unsigned short) calc);

    Temp_Sensor_Init((unsigned short) calc);
    
    while (1)
    {
        Wait_ms (1900);
        Led_On();
        calc = Temp_Sensor_GetTemp (Temp_Channel);

        sprintf(s_temp, "temperature: %dC\n", calc);

        Usart1Send(s_temp);
        Wait_ms(100);
        Led_Off();
    }
}

//--- end of file ---//
