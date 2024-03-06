//---------------------------------------------
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    TEST PLATFORM FOR FIRMWARE
// ##
// #### TESTS_MANAGER.C #######################
//---------------------------------------------

// Includes Modules for tests --------------------------------------------------
#include "manager.h"
#include "comms.h"

//helper modules
#include "tests_ok.h"


#include <stdio.h>
#include <string.h>


// Externals - Globals on Module to Test ---------------------------------------
extern volatile unsigned short timer_mngr;
extern int mngr_state;
extern volatile unsigned char timer_1sec_mngr;


// Globals - Externals on Module to Test ---------------------------------------
unsigned short adc_ch[1] = { 0 };


// Globals ---------------------------------------------------------------------
int usart_answer = 0;
unsigned char usart1_have_data = 0;
char usart1_last_buff [100] = { 0 };



// Module Auxialiary Functions -------------------------------------------------


// Module Mocked Functions -----------------------------------------------------


// Module Functions for testing ------------------------------------------------
void Test_Manager (void);


// Module Functions ------------------------------------------------------------
int main(int argc, char *argv[])
{

    Test_Manager ();

    return 0;
}


void Test_Manager (void)
{
    int error = 0;
    adc_ch[0] = 1105;
    
    printf("Test Manager\n");
    char s_antenna [] = { "ant4,010.10,128.10,003.50,065.00\r\n" };
    char s_name [] = { "name:Tunnel 10 inches\r\n" };

    if (!error)
    {
        printf("-- manager to TX_SERIE2_NC\n");
        usart_answer = 0;
        for (int i = 0; i < 5; i++)
        {
            Manager (s_name, s_antenna);
            // timer_mngr = 0;
        }
    
        printf(" mngr_state: %d\n", mngr_state);
        if (mngr_state != 5)
            error = 1;
    }

    if (!error)
    {
        printf("-- manager to STAND_BY\n");
        timer_mngr = 0;
        usart_answer = 0;

        for (int i = 0; i < 5; i++)
        {
            Manager (s_name, s_antenna);
            // timer_mngr = 0;
        }

        printf(" mngr_state: %d\n", mngr_state);
        if (mngr_state != 0)
            error = 1;
    }

    if (!error)
    {
        printf("-- manager in STAND_BY -- keepalive error\n");
        timer_mngr = 0;
        usart_answer = 0;

        usart1_have_data = 1;
        strcpy(usart1_last_buff, "keep live");        
        for (int i = 0; i < 5; i++)
        {
            Manager (s_name, s_antenna);
            timer_mngr = 0;
        }

        printf(" mngr_state: %d\n", mngr_state);
        if (mngr_state != 0)
            error = 1;
    }
    
    if (!error)
    {
        printf("-- manager to CONNECT\n");
        timer_mngr = 0;
        usart_answer = 1;

        for (int i = 0; i < 5; i++)
        {
            Manager (s_name, s_antenna);
            timer_mngr = 0;
        }

        printf(" mngr_state: %d\n", mngr_state);
        if (mngr_state != 1)
            error = 1;
    }

    if (!error)
    {
        printf("-- manager in CONNECT - get_temp\n");
        timer_mngr = 0;
        usart_answer = 1;

        usart1_have_data = 1;
        strcpy(usart1_last_buff, "get_temp");
        
        for (int i = 0; i < 5; i++)
        {
            Manager (s_name, s_antenna);
            timer_mngr = 0;
        }

        printf(" mngr_state: %d\n", mngr_state);
        if (mngr_state != 1)
            error = 1;
    }

    if (!error)
    {
        printf("-- manager in CONNECT - get_name\n");
        timer_mngr = 0;
        usart_answer = 1;

        usart1_have_data = 1;
        strcpy(usart1_last_buff, "get_name");
        
        for (int i = 0; i < 5; i++)
        {
            Manager (s_name, s_antenna);
            timer_mngr = 0;
        }

        printf(" mngr_state: %d\n", mngr_state);
        if (mngr_state != 1)
            error = 1;
    }

    if (!error)
    {
        printf("-- manager in CONNECT - get_params\n");
        timer_mngr = 0;
        usart_answer = 1;

        usart1_have_data = 1;
        strcpy(usart1_last_buff, "get_params");
        
        for (int i = 0; i < 5; i++)
        {
            Manager (s_name, s_antenna);
            timer_mngr = 0;
        }

        printf(" mngr_state: %d\n", mngr_state);
        if (mngr_state != 1)
            error = 1;
    }

    if (!error)
    {
        printf("-- manager in CONNECT - keepalive\n");
        timer_mngr = 0;
        usart_answer = 1;

        usart1_have_data = 1;
        strcpy(usart1_last_buff, "keepalive");
        
        for (int i = 0; i < 5; i++)
        {
            Manager (s_name, s_antenna);
            timer_mngr = 0;
        }

        printf(" mngr_state: %d\n", mngr_state);
        if (mngr_state != 1)
            error = 1;
    }

    if (!error)
    {
        printf("-- manager in CONNECT - keepalive error\n");
        timer_mngr = 0;
        usart_answer = 1;

        usart1_have_data = 1;
        strcpy(usart1_last_buff, "keep live");
        
        for (int i = 0; i < 5; i++)
        {
            Manager (s_name, s_antenna);
            timer_mngr = 0;
        }

        printf(" mngr_state: %d\n", mngr_state);
        if (mngr_state != 1)
            error = 1;
    }

    if (!error)
    {
        printf("-- manager to STAND_BY - 10 secs\n");
        timer_mngr = 10;
        usart_answer = 0;

        timer_1sec_mngr = 0;
        for (int i = 0; i < 5; i++)
        {
            Manager (s_name, s_antenna);
            // timer_mngr = 0;
        }

        printf(" mngr_state: %d\n", mngr_state);
        if (mngr_state != 0)
            error = 1;
    }
    
    // if (!error)
    // {
    //     strcpy (s_test, "get_temp");
    //     printf(" test %s\n", s_test);
    //     resp = Comms_CheckMsg (s_test);
    //     if (resp != GET_TEMP)
    //         error = 1;
    // }

    // if (!error)
    // {
    //     strcpy (s_test, "get_params");
    //     printf(" test %s\n", s_test);
    //     resp = Comms_CheckMsg (s_test);
    //     if (resp != GET_PARAMS)
    //         error = 1;
    // }

    // if (!error)
    // {
    //     strcpy (s_test, "get_name");
    //     printf(" test %s\n", s_test);
    //     resp = Comms_CheckMsg (s_test);
    //     if (resp != GET_NAME)
    //         error = 1;
    // }


    // if (!error)
    // {
    //     strcpy (s_test, "get_namx");
    //     printf(" test %s error str\n", s_test);
    //     resp = Comms_CheckMsg (s_test);
    //     if (resp != ERROR)
    //         error = 1;
    // }
    
    printf("Result: ");
    if (error == 0)
        PrintOK();
    else
        PrintERR();
    
}




// Module Auxiliary Functions --------------------------------------------------



// Module Mocked Functions -----------------------------------------------------
void Led_On (void)
{
    printf("Led to on\n");
}


void Led_Off (void)
{
    printf("Led to off\n");
}


void Usart1RxEnable (void)
{
    printf("enable usart1 rx\n");
}


void Usart1RxDisable (void)
{
    printf("disable usart1 rx\n");
}


void Wait_ms (unsigned short ms)
{
    printf("wait for %dms\n", ms);
}


unsigned char Usart1EndOfTx (void)
{
    return 1;
}


short Temp_Sensor_GetTemp (unsigned short adc_data)
{
    return 30;
}


void Usart1Send (char * msg)
{
    printf("sending: %s", msg);

    if (usart_answer)
    {
        if (!strncmp(msg,
                     "ant4,010.10,128.10,003.50,065.00\r\n",
                     sizeof("ant4,010.10,128.10,003.50,065.00\r\n") - 1))
        {
            usart1_have_data = 1;
            strcpy(usart1_last_buff, "get_name");
        }
    }
}


unsigned char Usart1HaveData (void)
{
    return usart1_have_data;
}

void Usart1HaveDataReset (void)
{
    usart1_have_data = 0;
}


unsigned char Usart1ReadBuffer (char * s_msg)
{
    strcpy(s_msg, usart1_last_buff);

    return strlen(s_msg);
}
//--- end of file ---//


