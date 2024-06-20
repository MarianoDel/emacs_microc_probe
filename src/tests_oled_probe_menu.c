//----------------------------------------------------
// Tests for gtk lib
// Implementation of tests_lcd_application.h functions
//----------------------------------------------------

#include <gtk/gtk.h>
#include <stdint.h>
#include "tests_oled_application.h"

// Application Includes needed for this test
#include "screen1.h"
#include "probe_menu.h"


// Module Types Constants and Macros -------------------------------------------


// Externals -- Access to the tested Module ------------------------------------


// Globals -- Externals for the tested Module ----------------------------------


// Globals ---------------------------------------------------------------------
static GMutex mutex;
unsigned int timer_standby = 0;
int encoder_actions = 0;


// Teting Functions ------------------------------------------------------------
void Test_Probe_Menu (void);


// Testing Function loop -------------------------------------------------------
gboolean Test_Main_Loop (gpointer user_data)
{
    
    Test_Probe_Menu ();
    
    return TRUE;
}


gboolean Test_Timeouts_Loop_1ms (gpointer user_data)
{
    if (timer_standby)
        timer_standby--;

    ProbeMenu_UpdateTimer ();
    
    return TRUE;
}


gboolean Test_Timeouts_Loop_1000ms (gpointer user_data)
{
    return TRUE;
}


// Module Implemantation of buttons functions
void cw_button_function (void)
{
    g_mutex_lock (&mutex);
    encoder_actions = 1;
    g_mutex_unlock (&mutex);
}


void ccw_button_function (void)
{
    g_mutex_lock (&mutex);
    encoder_actions = 2;
    g_mutex_unlock (&mutex);
}


void set_button_function (void)
{
    g_mutex_lock (&mutex);
    encoder_actions = 3;
    g_mutex_unlock (&mutex);
}


// Module Testing functions ----------------------------------------------------
int setup_done = 0;
void Test_Probe_Menu (void)
{
    //first screen
    if (setup_done == 0)
    {
        setup_done = 1;

        SCREEN_Init();

        SCREEN_ShowText2(
            "Infinity ",
            "  Clinics",
            "",
            ""
            );

        timer_standby = 1300;        
        
        ProbeMenu_Reset ();

        display_update_int_state_machine ();
    }


    if (setup_done == 1)
    {
        if (!timer_standby)
            setup_done = 2;
        else
            display_update_int_state_machine ();
    }

    if (setup_done == 2)
    {
        probe_actions_e action = DO_NOTHING;
        
        if (encoder_actions == 3)
            action = SHOW_START;

        if (encoder_actions == 2)
            action = SHOW_CONNECT;

        if (encoder_actions == 1)
            action = SHOW_INIT;
        
        ProbeMenu (action);

        encoder_actions = 0;

        display_update_int_state_machine ();

    }    
    
}



// Mocked Functions Needed for the tests ---------------------------------------


//--- end of file ---//
