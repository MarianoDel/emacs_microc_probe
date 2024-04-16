//----------------------------------------------------
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### DISPLAY_UTILS.C ##############################
//----------------------------------------------------

// Includes --------------------------------------------------------------------
#include "display_utils1.h"
#include "ssd1306_gfx1.h"
#include "ssd1306_display1.h"

#include <stdio.h>
#include <string.h>


// Private Types Constants and Macros ------------------------------------------
#define SRT_X    0
#define END_X    127

#define SRT_L1_Y    0
#define SRT_L2_Y    8
#define SRT_L3_Y    16
#define SRT_L4_Y    24
#define SRT_L5_Y    32
#define SRT_L6_Y    40
#define SRT_L7_Y    48
#define SRT_L8_Y    56

#define WIDTH    128
#define LINE_HEIGHT    8


// Externals -------------------------------------------------------------------

// Globals ---------------------------------------------------------------------

// Module Private Functions ----------------------------------------------------

// Module Funtions -------------------------------------------------------------
void Display_StartLines (void)
{
    gfx_setTextWrap(0);
    gfx_setTextSize(1);
    gfx_setTextColor(1);    

}


void Display_ClearLines (void)
{
    display_clear();
}


void Display_BlankLine (unsigned char line)
{
    gfx_fillRect(SRT_X, LINE_HEIGHT * (line - 1), WIDTH, LINE_HEIGHT, 0);        
}


void Display_SetLine (unsigned char line, const char * s)
{
    gfx_setCursor(SRT_X, LINE_HEIGHT * (line - 1));
    gfx_print(s);
}


void Display_SetOptions (unsigned char line, const char * s)
{
    char pretty [ALL_LINE_LENGTH_NULL] = { 0 };
    unsigned char line_offset = LINE_HEIGHT * (line - 1);

    if (strlen(s) > (ALL_LINE_LENGTH - 2))
        return;

    //clean line
    gfx_fillRect(SRT_X, line_offset, WIDTH, LINE_HEIGHT, 0);

    //change text type
    gfx_setTextColor(0);
    gfx_setTextBg(1);

    //write prettify string
    gfx_setCursor(SRT_X, line_offset);    
    sprintf(pretty, " %s ", s);
    gfx_print(pretty);

    //back to normal text type
    gfx_setTextColor(1);
    gfx_setTextBg(0);
    
}


void Display_FloatingOptions (options_st * op)
{
    //change text type
    if (op->set_or_reset)
    {
        gfx_setTextColor(0);
        gfx_setTextBg(1);
    }

    //clean the box
    gfx_fillRect(op->startx, op->starty, op->box_width, op->box_height, 0);
    gfx_setCursor(op->startx, op->starty);
    gfx_print(op->s);

    //back to normal text type
    if (op->set_or_reset)
    {
        gfx_setTextColor(1);
        gfx_setTextBg(0);
    }
}

//--- end of file ---//
