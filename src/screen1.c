//---------------------------------------------
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    STM32F303
// ##
// #### SCREEN.C ##############################
//---------------------------------------------

// Includes --------------------------------------------------------------------
#include "screen1.h"
#include "ssd1306_display1.h"
#include "ssd1306_gfx1.h"
#include "ssd1306_params1.h"

#include "i2c.h"    //TODO: MEJORAR ESTO PARA ADDR SLV


// Externals -------------------------------------------------------------------

// Globals ---------------------------------------------------------------------

// Module Private Functions ----------------------------------------------------

// Module Funtions -------------------------------------------------------------
void SCREEN_Init (void)
{
    display_init(I2C_ADDRESS_SLV);
    gfx_init( DISPLAYWIDTH, DISPLAYHEIGHT );
}


// recibe los 4 renglones a mostrar 9 caracteres maximo
void SCREEN_ShowText2 (char * line1, char * line2, char * line3, char * line4)
{
    gfx_setTextSize(2);
    gfx_setTextBg(0);
    gfx_setTextColor(1);
    display_clear();    //200us aprox.
    gfx_setCursor(0,0);

    gfx_println(line1);
    gfx_println(line2);
    gfx_println(line3);
    gfx_println(line4);

    display_update();    
}

void SCREEN_ShowText4 (char * line1, char * line2)
{
    gfx_setTextSize(4);
    gfx_setTextBg(0);
    gfx_setTextColor(1);
    display_clear();    //200us aprox.
    gfx_setCursor(0,0);

    gfx_println(line1);
    gfx_println(line2);

    display_update();    
}


// void SCREEN_Text4_Line1 (char * line1)
// {
//     gfx_setTextSize(4);
//     gfx_setTextColor(1);
//     gfx_setCursor(0,0);
//     gfx_print( line1 ); 
//     // gfx_println(line1);
//     display_update();    
// }


// void SCREEN_Text4_Line2 (char * line2)
// {
//     gfx_setTextSize(4);
//     gfx_setTextColor(1);
//     gfx_setCursor(0,32);
//     gfx_println(line2);
//     display_update();    
// }


void SCREEN_Text2_Line1 (char * line1)
{
    gfx_setTextSize(2);
    gfx_setTextColor(1);
    gfx_setCursor(0,0);
    gfx_print( line1 ); 
    // gfx_println(line1);
    display_update();    
}


void SCREEN_Text2_Line2 (char * line2)
{
    gfx_setTextSize(2);
    gfx_setTextColor(1);
    gfx_setCursor(0,16);
    gfx_print( line2 ); 
    // gfx_println(line1);
    display_update();    
}


void SCREEN_Text2_BlankLine1 (void)
{
    gfx_fillRect (0, 0, 128, 16, 0);
    display_update();    
}


void SCREEN_Text2_BlankLine2 (void)
{
    gfx_fillRect (0, 16, 128, 16, 0);
    display_update();    
}


void SCREEN_Clear (void)
{
    display_clear();    //200us aprox.
    display_update();    
}


//--- end of file ---//
