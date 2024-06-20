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
#include "hard.h"
#include "usart.h"
#include "tim.h"
#include "comms.h"
#include "screen1.h"


#include <stdio.h>
#include <string.h>

// Module Private Types & Macros -----------------------------------------------
typedef enum {
    INIT,
    STAND_BY,
    CONNECT,
    TX_SERIE,
    TX_SERIE2,
    RX_SERIE
    
} manager_states_e;


// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------
manager_states_e mngr_state = INIT;
manager_states_e mngr_call_state = STAND_BY;
volatile unsigned short timer_mngr = 0;
volatile unsigned short timer_start = 0;
volatile unsigned char timer_1sec_mngr = 0;
volatile unsigned short millis = 0;
comms_answers_e answer = COMMS_ERROR;


// Module Private Functions ----------------------------------------------------


// Module Functions ------------------------------------------------------------
void Manager_Timeouts (void)
{
    if (timer_mngr)
        timer_mngr--;    

    if (timer_start)
        timer_start--;
    
    if (millis < 1000)
        millis++;
    else
    {
        millis = 0;
        if (timer_1sec_mngr)
            timer_1sec_mngr--;
    }
}


void Manager (char * probe_name)
{
    static unsigned char start_sended = 0;
    static unsigned short start_cnt = 0;
    char s_msg [100];
    
    switch (mngr_state)
    {
    case INIT:
        // show oled info
        SCREEN_Text2_BlankLine1 ();
        SCREEN_Text2_BlankLine2 ();

        // SCREEN_Text2_Line1 ("Infinity  ");        
        SCREEN_Text2_Line1 ("        NC");
        SCREEN_Text2_Line1 (probe_name);
        start_cnt = 0;
        mngr_state++;
        break;
        
    case STAND_BY:
        // send probe info every second waiting for answers        
        Led_Off();

        if (!timer_mngr)
        {
            Led_On();
            mngr_state = TX_SERIE;
            mngr_call_state = STAND_BY;
            Usart1RxDisable();
            sprintf(s_msg, "name %s\r\n", probe_name);
            Usart1Send(s_msg);
        }

        if (Usart1HaveData())
        {
            mngr_state = RX_SERIE;
            mngr_call_state = STAND_BY;            
        }

        if (!timer_start)
        {
            if (start_sended)
            {
                start_sended = 0;
                SCREEN_Text2_BlankLine2 ();
            }
            else if (Start_Btn())
            {
                Led_On();
                mngr_state = TX_SERIE;
                mngr_call_state = STAND_BY;
                Usart1RxDisable();
                Usart1Send("start\r\n");

                if (start_cnt < 9999)
                    start_cnt++;
                else
                    start_cnt = 0;

                timer_start = 2000;
                start_sended = 1;
                sprintf(s_msg, "Start %4d", start_cnt);
                SCREEN_Text2_BlankLine2 ();            
                SCREEN_Text2_Line2 (s_msg);
            }
        }


        if ((answer == COMMS_KEEPALIVE) ||
            (answer == COMMS_GET_NAME))
        {
            // // first ok answer
            // Wait_ms(5);
            // answer = COMMS_ERROR;
            // mngr_state = TX_SERIE;
            // mngr_call_state = CONNECT;            
            // Usart1RxDisable();
            // Usart1Send("ok\r\n");
            
            // show oled info -- 90ms delay
            SCREEN_Text2_BlankLine1 ();
            SCREEN_Text2_Line1 ("        Cn");
            SCREEN_Text2_Line1 (probe_name);
            
            mngr_state = CONNECT;
        }        
        break;
        
    case CONNECT:
        //cuando se agota timer_1_seg salgo a STAND_BY
        //me fijo si debo contestar algo
        Led_On();

        if (answer == COMMS_KEEPALIVE)
        {
            Led_Off();
            Wait_ms(5);
            answer = COMMS_ERROR;
            mngr_state = TX_SERIE;
            mngr_call_state = CONNECT;            
            Usart1RxDisable();
            Usart1Send("ok\r\n");
        }

        if (answer == COMMS_GET_NAME)
        {
            Led_Off();
            Wait_ms(5);
            answer = COMMS_ERROR;
            mngr_state = TX_SERIE;
            mngr_call_state = CONNECT;
            Usart1RxDisable();
            sprintf(s_msg, "name %s\r\n", probe_name);
            Usart1Send(s_msg);
        }


        if ((Usart1HaveData()) && (mngr_state == CONNECT))
        {            
            mngr_state = RX_SERIE;
            mngr_call_state = CONNECT;
        }

        if (!timer_1sec_mngr)    // more than 10 secs without comms
        {
            // show oled info
            SCREEN_Text2_BlankLine1 ();
            SCREEN_Text2_Line1 ("        NC");
            SCREEN_Text2_Line1 (probe_name);
            
            mngr_state = STAND_BY;
        }

        if (!timer_start)
        {
            if (start_sended)
            {
                start_sended = 0;
                SCREEN_Text2_BlankLine2 ();
            }
            else if (Start_Btn())
            {
                Led_Off();
                mngr_state = TX_SERIE;
                mngr_call_state = CONNECT;
                Usart1RxDisable();
                Usart1Send("start\r\n");

                if (start_cnt < 9999)
                    start_cnt++;
                else
                    start_cnt = 0;

                timer_start = 2000;
                start_sended = 1;
                sprintf(s_msg, "Start %4d", start_cnt);
                SCREEN_Text2_BlankLine2 ();            
                SCREEN_Text2_Line2 (s_msg);
            }
        }
        break;

    case TX_SERIE:
        // wait end of transmission        
        if (Usart1EndOfTx())
        {
            mngr_state = TX_SERIE2;
            timer_mngr = 2;
        }
        break;

    case TX_SERIE2:
        if (!timer_mngr)
        {
            mngr_state = mngr_call_state;

            // if (Led_Is_On())
            //     Led_Off();
            // else
            //     Led_On();
            
            Usart1RxEnable();

            timer_mngr = 1000;
            
        }
        break;

    case RX_SERIE:
        // check the rx msg, go to called state
        if (Usart1HaveData())
        {
            Usart1ReadBuffer((unsigned char *)s_msg, sizeof(s_msg));
            Usart1HaveDataReset();

            comms_answers_e a = COMMS_ERROR;
            a = Comms_CheckMsg (s_msg);

            switch (a)
            {
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
        
        mngr_state = mngr_call_state;
        break;

    default:
        mngr_state = STAND_BY;
        break;
    }
}


//--- end of file ---//

