//-------------------------------------------------------
// #### PROJECT: Magnet Antenna  G030 - Custom Board ####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### MANAGER.C #######################################
//-------------------------------------------------------

// Includes --------------------------------------------------------------------
#include "manager.h"
#include "adc.h"
#include "usart.h"
#include "tim.h"
#include "comms.h"
#include "temp_sensor.h"


#include <stdio.h>
#include <string.h>

// Module Private Types & Macros -----------------------------------------------
typedef enum {
    STAND_BY,
    CONNECT,
    TX_SERIE,
    TX_SERIE2,
    TX_SERIE_NC,
    TX_SERIE2_NC,
    RX_SERIE,
    TEMP_SENSE
    
} manager_states_e;


// Externals -------------------------------------------------------------------
extern volatile unsigned short adc_ch [];


// Globals ---------------------------------------------------------------------
manager_states_e mngr_state = STAND_BY;
volatile unsigned short timer_mngr = 0;
volatile unsigned char timer_1sec_mngr = 0;
volatile unsigned short millis = 0;
comms_answers_e answer = COMMS_ERROR;


// Module Private Functions ----------------------------------------------------


// Module Functions ------------------------------------------------------------
void Manager (char * ant_name, char * ant_params)
{
    char s_msg [100];
    
    switch (mngr_state)
    {
    case STAND_BY:
        // send antenna params on each second waiting for answers
        Led_Off();

        if (!timer_mngr)
        {
            Led_On();
            mngr_state = TX_SERIE_NC;
            Usart1RxDisable();
            Usart1Send(ant_params);
        }

        if (Usart1HaveData())
        {
            mngr_state = RX_SERIE;
        }
        break;

    case CONNECT:
        //cuando se agota timer_1_seg salgo a STAND_BY
        //me fijo si debo contestar algo
        Led_On();
        if (answer == COMMS_KEEPALIVE)
        {
            Wait_ms(5);
            answer = COMMS_ERROR;
            Led_Off();
            mngr_state = TX_SERIE;
            //apago RX
            Usart1RxDisable();
            Usart1Send("ok\r\n");
        }

        if (answer == COMMS_GET_PARAMS)
        {
            Wait_ms(5);
            answer = COMMS_ERROR;
            Led_Off();
            mngr_state = TX_SERIE;
            //apago RX
            Usart1RxDisable();
            Usart1Send(ant_params);
        }

        if (answer == COMMS_GET_NAME)
        {
            Wait_ms(5);
            answer = COMMS_ERROR;
            Led_Off();
            mngr_state = TX_SERIE;
            //apago RX
            Usart1RxDisable();
            Usart1Send(ant_name);
        }

        if (answer == COMMS_GET_TEMP)
        {
            Wait_ms(5);
            Led_Off();

            short temp = Temp_Sensor_GetTemp (Temp_Channel);

            //reviso errores de conversion
            if ((temp >= 0) && (temp <= 85))
            {
                char s_temp [30] = { 0 };
                sprintf(s_temp, "temp,%03d.00\r\n", temp);
                //apago RX
                Usart1RxDisable();
                Usart1Send(s_temp);

                answer = COMMS_ERROR;
                mngr_state = TX_SERIE;
            }
        }

        if ((Usart1HaveData()) && (mngr_state == CONNECT))
        {
            mngr_state = RX_SERIE;
        }

        if (!timer_1sec_mngr)    // more than 10 secs without comms
        {
            mngr_state = STAND_BY;
        }
        break;

    case TX_SERIE:
        //espero terminar de transmitir
        if (Usart1EndOfTx())
        {
            mngr_state = TX_SERIE2;
            timer_mngr = 2;
        }
        break;

    case TX_SERIE2:
        if (!timer_mngr)
        {
            mngr_state = CONNECT;
            Led_On();
            Usart1RxEnable();
        }
        break;

    case TX_SERIE_NC:
        //espero terminar de transmitir
        if (Usart1EndOfTx())
        {
            mngr_state = TX_SERIE2_NC;
            timer_mngr = 2;
        }
        break;

    case TX_SERIE2_NC:
        if (!timer_mngr)
        {
            mngr_state = STAND_BY;
            timer_mngr = 1000;
            Led_Off();
            Usart1RxEnable();
        }
        break;

    case RX_SERIE:
        // check the rx msg, go to connect on goog msg
        if (Usart1HaveData())
        {
            Usart1ReadBuffer((unsigned char *)s_msg, sizeof(s_msg));
            Usart1HaveDataReset();

            comms_answers_e a = COMMS_ERROR;
            a = Comms_CheckMsg (s_msg);

            switch (a)
            {
            case COMMS_GET_PARAMS:
                answer = COMMS_GET_PARAMS;
                timer_1sec_mngr = 10;
                break;

            case COMMS_GET_TEMP:
                answer = COMMS_GET_TEMP;
                timer_1sec_mngr = 10;
                break;

            case COMMS_GET_NAME:
                answer = COMMS_GET_NAME;
                timer_1sec_mngr = 10;
                break;
                
            case COMMS_KEEPALIVE:
                answer = COMMS_KEEPALIVE;
                timer_1sec_mngr = 10;
                break;

            case COMMS_ERROR:
            default:
                break;
            }
        }
        
        if (timer_1sec_mngr == 0)
            mngr_state = STAND_BY;
        else
            mngr_state = CONNECT;

        break;

    case TEMP_SENSE:
        break;

    default:
        mngr_state = STAND_BY;
        break;
    }

    // save flash after configs
//     if ((need_to_save) && (!need_to_save_timer))
//     {
//         need_to_save = Flash_WriteConfigurations();

// #ifdef USART_DEBUG_MODE
//         if (need_to_save)
//             UsartDebug((char *) "Memory Saved OK!\n");
//         else
//             UsartDebug((char *) "Memory problems\n");
// #endif

//         need_to_save = 0;
//     }
        
}


void Manager_Timeouts (void)
{
    // if (need_to_save_timer)
    //     need_to_save_timer--;

    if (timer_mngr)
        timer_mngr--;    

    if (millis < 1000)
        millis++;
    else
    {
        millis = 0;
        if (timer_1sec_mngr)
            timer_1sec_mngr--;
    }
}


//--- end of file ---//

