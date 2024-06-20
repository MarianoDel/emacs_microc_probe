//-------------------------------------------------
// #### DEXEL 6CH BIDIRECTIONAL - Custom Board ####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### SLAVE_MENU.C ##############################
//-------------------------------------------------

// Includes --------------------------------------------------------------------
#include "probe_menu.h"
#include "display_utils1.h"

#include <string.h>
#include <stdio.h>


// Module Private Types Constants and Macros -----------------------------------
typedef enum {
    PROBE_MENU_INIT = 0,
    PROBE_MENU_WAIT_COMMANDS,
    PROBE_MENU_CONNECT,
    PROBE_MENU_DISCONNECT,
    PROBE_MENU_INFINITY,    
    PROBE_MENU_START,
    PROBE_MENU_START_WAIT_FINISH
    
} probe_menu_state_e;


// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------
volatile unsigned short probe_menu_timer = 0;
probe_menu_state_e probe_menu_state = PROBE_MENU_INIT;


// Module Private Functions ----------------------------------------------------


// Module Funtions -------------------------------------------------------------
void ProbeMenu_UpdateTimer (void)
{
    if (probe_menu_timer)
        probe_menu_timer--;
}


void ProbeMenu_Reset (void)
{
    probe_menu_state = PROBE_MENU_INIT;
}


extern void display_update (void);
void ProbeMenu (probe_actions_e action)
{
    unsigned char probe_menu_need_display_update = 0;
    
    switch (probe_menu_state)
    {
    case PROBE_MENU_INIT:
        Display_StartLines ();
        // Display_ClearLines();

        Display_BlankLine3();
        Display_BlankLine4();
        
        probe_menu_need_display_update = 1;
        probe_menu_state++;
        break;

    case PROBE_MENU_WAIT_COMMANDS:
        if (action == SHOW_CONNECT)
            probe_menu_state = PROBE_MENU_CONNECT;

        if (action == SHOW_DISCONNECT)
            probe_menu_state = PROBE_MENU_DISCONNECT;
        
        if (action == SHOW_START)
            probe_menu_state = PROBE_MENU_START;

        if (action == SHOW_INIT)
            probe_menu_state = PROBE_MENU_INFINITY;
        
        break;

    case PROBE_MENU_CONNECT:
        Display_BlankLine3();
        // Display_BlankLine4();

        Display_SetLine3("Connected to Channel");

        probe_menu_need_display_update = 1;
        probe_menu_state = PROBE_MENU_WAIT_COMMANDS;
        break;

    case PROBE_MENU_DISCONNECT:
        Display_BlankLine3();
        // Display_BlankLine4();

        Display_SetLine3(" Disconnected");        

        probe_menu_need_display_update = 1;
        probe_menu_state = PROBE_MENU_WAIT_COMMANDS;
        break;        

    case PROBE_MENU_INFINITY:
        Display_BlankLine1();
        Display_BlankLine2();

        Display_SetLine1("   Infinity");
        Display_SetLine2("         Clinics");        

        probe_menu_need_display_update = 1;
        probe_menu_state = PROBE_MENU_WAIT_COMMANDS;
        break;

    case PROBE_MENU_START:
        Display_BlankLine4();

        Display_SetLine4("               Start");

        probe_menu_timer = 2000;
        probe_menu_need_display_update = 1;
        probe_menu_state = PROBE_MENU_START_WAIT_FINISH;
        break;
        
    case PROBE_MENU_START_WAIT_FINISH:
        if (!probe_menu_timer)
        {
            Display_BlankLine4();

            probe_menu_need_display_update = 1;
            probe_menu_state = PROBE_MENU_WAIT_COMMANDS;
        }
        break;

    default:
        probe_menu_state = PROBE_MENU_INIT;
        break;
    }

    if (probe_menu_need_display_update)
    {
        display_update();
        probe_menu_need_display_update = 0;
    }
}

//--- end of file ---//
