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
#include "adc.h"
#include "dma.h"
#include "temp_sensor.h"
#include "test_functions.h"
#include "manager.h"

#include <string.h>
#include <stdio.h>


// Select the Antenna to use here! ---------------------------------------------
// #define ANTENA0    // plana 125mm 23mm 1.3kg alambre 0.8mm dia -- Plannar 5 inches
// #define ANTENA1    //toroidal diametro mediana
// #define ANTENA1B    // plana 125mm 23mm 1.3kg -- Plannar 5 inches
// #define ANTENA2	//cilindrica chica tunel de munieca
// #define ANTENA3	//cilindrica mediana
// #define ANTENA4	//cilindrica grande
// #define ANTENA4B	//cilindrica grande, fabricada 18-06-2021
// #define ANTENA4C    //tunnel 10", fabricada 23-08-2021
// #define ANTENA4D    // tunnel 10", fabricada 22-09-2022
// #define ANTENA4E    // tunnel 10", fabricada 10-01-2023
// #define ANTENA4F    // tunnel 10" head, fabricada 10-01-2023
// #define ANTENA4G    // tunnel 8", fabricada 10-01-2023
// #define ANTENA4H    // tunnel 5", fabricada 10-01-2023
// #define ANTENA4I    //tunnel 12", oval fabricada 25-09-2023, 10.4ohms 138mHy
//#define ANTENA5	//cilindrica muy chica OJOS
//#define ANTENA6	//cilindrica vieja de madera
// #define ANTENA7 //pencil tunel
#define ANTENA7B    // tunnel 4" (wrist tunnel) fabricada 11-12-2023
//#define ANTENA8 //pencil plato doble
//#define ANTENAA1	//Ernesto viejas
//#define ANTENAA2	//Ernesto viejas
//#define ANTENAA3	//Ernesto viejas
// #define ANTENAA5	//Ernesto viejas
//#define ANTENAA4	//Ernesto viejas
//#define ANTENAA6 //antenas Ernesto tunel grande 350mm dia
//#define ANTENAA7 //antenas Ernesto tunel mediano 240mm dia
//#define ANTENAA1 //antenas Ernesto tunel chico 150mm dia
//#define ANTENAA9	//Nuevas Luis
//#define ANTENAB0	//antenas Ernesto tunel 9" dia
//#define ANTENAB1	//antenas Ernesto tunel 7" dia
//#define ANTENAB2	//antenas Ernesto tunel 6" dia
// #define ANTENAB3	//antenas Ernesto tunel 6" dia
// #define ANTENAB4	//antenas plato companiera pencil
// #define ANTENAB5	//antenas para ojos 2 bobinas en paralelo
// #define ANTENAB6	//eyes gogles 48G, the new ones
// #define ANTENAB7    //eyes gogles 48G, the new ones 12-09-2023 75ohms 87mHy individual





// #define ANTK_T10INCHES    // tunnel 10" 1mm 740T

// Externals -------------------------------------------------------------------
// -- Externals for the timer module -----------------------
volatile unsigned char timer_1seg = 0;
volatile unsigned short timer_standby = 0;


// -- Externals from or for Serial Port --------------------
// volatile unsigned char buffrx_ready = 0;
// volatile unsigned char *pbuffrx;
// volatile unsigned char *pbuffrx_cpy;

// const char s_ok [] = {"ok\r\n"};

// -- Externals for the adc --------------------------------
volatile unsigned short adc_ch[ADC_CHANNEL_QUANTITY];


// Globals ---------------------------------------------------------------------
// Each name has to have less than 8 chars
//antena, R [ohms], L [mHy], Imax [A], Tmax [ºC] todos 000.00
#ifdef ANTENA0 //toroidal diametro grande
const char s_antenna [] = { "ant0,012.27,087.90,001.80,065.50\r\n" };
const char s_name [] = { "name:Plannar 5 inches\r\n" };
#endif

#ifdef ANTENA1 //toroidal diametro mediana
const char s_antenna [] = { "ant1,023.85,141.60,001.30,065.50\r\n" };
const char s_name [] = { "name:Plannar 5 inches\r\n" };
#endif

#ifdef ANTENA1B // plana 125mm 23mm 1.3kg -- Plannar 5 inches
const char s_antenna [] = { "ant1,017.00,120.00,001.30,065.50\r\n" };
const char s_name [] = { "name:Plannar 5 inches\r\n" };
#endif

#ifdef ANTENA2 //cilindrica chica
const char s_antenna [] = { "ant2,005.70,011.10,002.80,065.50\r\n" };
const char s_name [] = { "name:Wrist Tunnel\r\n" };
#endif

#ifdef ANTENA3 //cilindrica mediana
const char s_antenna [] = { "ant3,003.50,019.00,003.50,065.50\r\n" };
const char s_name [] = { "name:Tunnel 6 inches\r\n" };
#endif

#ifdef ANTENA4 //cilindrica grande
const char s_antenna [] = { "ant4,004.00,022.60,003.50,065.50\r\n" };
const char s_name [] = { "name:Tunnel 8 inches\r\n" };
#endif

#ifdef ANTENA4B //cilindrica grande
const char s_antenna [] = { "ant4,002.50,021.00,003.50,065.50\r\n" };
const char s_name [] = { "name:Tunnel 10 inches\r\n" };
#endif

#ifdef ANTENA4C //cilindrica grande 3 capas 0.9, 4 capas 0.7
const char s_antenna [] = { "ant4,006.60,056.50,003.50,065.50\r\n" };
const char s_name [] = { "name:Tunnel 10 inches\r\n" };
#endif

#ifdef ANTENA4D    //tunnel 10" 6 capas 1.2mm
const char s_antenna [] = { "ant4,010.10,128.10,003.50,065.50\r\n" };
const char s_name [] = { "name:Tunnel 10 inches\r\n" };
#endif

#ifdef ANTENA4E    //tunnel 10" 4 capas 1.2mm 750T, 10-01-2023
const char s_antenna [] = { "ant4,008.50,100.00,003.50,065.50\r\n" };
const char s_name [] = { "name:Tunnel 10 inches\r\n" };
#endif

#ifdef ANTENA4F    //tunnel 10" 4 capas 1.2mm 750T, 10-01-2023
const char s_antenna [] = { "ant4,008.50,100.00,003.50,065.50\r\n" };
const char s_name [] = { "name:Head Tunnel 10\"\r\n" };
#endif

#ifdef ANTENA4G    //tunnel 8" 4 capas 1mm 1000T, 10-01-2023
const char s_antenna [] = { "ant4,015.30,155.00,003.50,065.50\r\n" };
const char s_name [] = { "name:Tunnel 8 inches\r\n" };
#endif

#ifdef ANTENA4H    //tunnel 5" 4 capas 0.75mm 1000T, 10-01-2023
const char s_antenna [] = { "ant4,014.30,083.00,002.00,065.50\r\n" };
const char s_name [] = { "name:Tunnel 5 inches\r\n" };
#endif

#ifdef ANTENA4I    //tunnel 12" 25-09-2023, 
const char s_antenna [] = { "ant4,010.40,138.00,003.50,065.50\r\n" };
const char s_name [] = { "name:Head Tunnel\r\n" };
#endif

#ifdef ANTENA5 //cilindrica muy chica OJOS
const char s_antenna [] = { "ant5,052.00,130.00,000.60,065.50\r\n" };
#endif

#ifdef ANTENA6 //cilindrica vieja de madera
const char s_antenna [] = { "ant6,010.60,078.20,002.00,065.50\r\n" };
#endif

#ifdef ANTENA7A //pencil tunel
const char s_antenna [] = { "ant7,012.70,064.80,002.00,065.50\r\n" };
#endif

#ifdef ANTENA7B    // tunnel 4" ajusto R de 15 a 13.5 para menor corriente
// const char s_antenna [] = { "ant7,015.00,061.00,002.50,065.50\r\n" };
const char s_antenna [] = { "ant7,013.50,061.00,002.50,065.50\r\n" };
const char s_name [] = { "name:Tunnel 4 inches\r\n" };
#endif

#ifdef ANTENA8 //pencil plato doble
const char s_antenna [] = { "ant8,022.60,157.20,001.10,065.50\r\n" };
#endif

//antena, R [ohms], L [mHy], Imax [A], Tmax [Â°C] todos 000.00
#ifdef ANTENAA9 //toroidal diametro mediana antena nueva DE=110 DI=45
const char s_antenna [] = { "anta,023.80,133.00,001.10,065.50\r\n" };
#endif

#ifdef ANTENAA1 //antenas Ernesto viejas CHICOS
const char s_antenna [] = { "anta,008.60,045.00,002.50,065.50\r\n" };
#endif

#ifdef ANTENAA2 //antenas Ernesto viejas CHICOS
const char s_antenna [] = { "anta,009.00,051.40,003.20,065.50\r\n" };
#endif

#ifdef ANTENAA3 //antenas Ernesto T02
const char s_antenna [] = { "anta,007.20,041.40,004.00,065.50\r\n" };
#endif

#ifdef ANTENAA4 //antenas Ernesto viejas CHICOS
const char s_antenna [] = { "anta,022.60,157.20,001.10,065.50\r\n" };
#endif

#ifdef ANTENAA5 //antenas Ernesto viejas CHICOS
const char s_antenna [] = { "anta,022.60,157.20,001.10,065.50\r\n" };
#endif

#ifdef ANTENAA6 //antenas Ernesto tunel grande 350mm dia
const char s_antenna [] = { "anta,009.00,051.00,003.50,065.50\r\n" };
#endif

#ifdef ANTENAA7 //antenas Ernesto tunel mediano 240mm dia
const char s_antenna [] = { "anta,009.00,045.00,003.50,065.50\r\n" };
#endif

#ifdef ANTENAA8 //antenas Ernesto tunel chico 150mm dia
const char s_antenna [] = { "anta,010.60,007.82,002.00,065.50\r\n" };
#endif

#ifdef ANTENAB0 //antenas Ernesto tunel 5,6ohm 15,4mHy 08-08-17
const char s_antenna [] = { "anta,005.60,015.40,003.00,065.50\r\n" };
#endif

#ifdef ANTENAB1 //antenas Ernesto tunel 08-08-17
const char s_antenna [] = { "anta,002.70,019.40,003.00,065.50\r\n" };
#endif

#ifdef ANTENAB2 //antenas Ernesto tunel 08-08-17
const char s_antenna [] = { "anta,003.50,023.25,003.00,065.50\r\n" };
#endif

#ifdef ANTENAB3 //antenas Ernesto tunel 08-08-17
const char s_antenna [] = { "anta,002.20,021.71,003.00,065.50\r\n" };
#endif

#ifdef ANTENAB4 //antenas plana companiera de pencil 26-04-18
// const char s_antenna [] = { "anta,031.10,220.00,001.10,065.50\r\n" };
const char s_antenna [] = { "anta,031.10,150.00,001.10,065.50\r\n" };    //lo baje para que me deje salr cuadrada en 60Hz
#endif

#ifdef ANTENAB5
const char s_antenna [] = { "anta,147.00,180.00,000.32,055.50\r\n" };
const char s_name [] = { "name:GT Googles 1\r\n" };
#endif

#ifdef ANTENAB6
const char s_antenna [] = { "anta,061.00,063.00,000.64,055.50\r\n" };
const char s_name [] = { "name:GT Googles\r\n" };
#endif

#ifdef ANTENAB7    // googles 12-09-2023
const char s_antenna [] = { "anta,035.00,045.00,000.64,055.50\r\n" };
const char s_name [] = { "name:Inf Googles\r\n" };
#endif


#ifdef ANTK_T10INCHES    //tunnel 10" 4 capas 1.0mm 185T x capa 740T, 30-08-2023
const char s_antenna [] = { "ant4,013.10,110.80,003.50,065.50\r\n" };
const char s_name [] = { "name:Tunnel 10 inches\r\n" };
#endif


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
    char str1 [40];

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
    
    //--- Funciones de Test Hardware ---
    // TF_Hardware_Tests ();    
    //--- Fin Funciones de Test Hardware ---    
    
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

    // convert ref_30 from 3V to 3.3V on Temp_Sensor
    int calc = ts_cal1 * 30;
    calc = calc / 33;
    Temp_Sensor_SetRef((unsigned short) calc);

    Temp_Sensor_Init((unsigned short) calc);

    Usart1RxDisable();
    sprintf(str1, "ts_cal1: %d adj: %d\r\n",
            ts_cal1,
            (unsigned short) calc);
    Usart1Send(str1);
    Wait_ms(100);
    Usart1RxEnable();

    //--- Main loop ---//
    while(1)
    {
        // Production Program
        Manager ((char *) s_name, (char *) s_antenna);    
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


void Led_On (void)
{
    LED_ON;
}


void Led_Off (void)
{
    LED_OFF;
}


//--- end of file ---//
