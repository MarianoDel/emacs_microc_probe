//-------------------------------------------------------
// #### SSD1306 OLED Display I2C Lib ####################
// ##
// ## @Author: Derk Steggewentz, 3/2015 
// ## @Modifications: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### SSD1306_DISPLAY.H ###############################
//-------------------------------------------------------
#ifndef _SSD1306_DISPLAY_H
#define _SSD1306_DISPLAY_H

#include <stdint.h>


// Exported Types Constants and Macros -----------------------------------------
typedef enum{
    SCROLL_RIGHT = 0x26,
    SCROLL_LEFT = 0x2A
}SCROLL_DIR;

typedef enum{
    SCROLL_SPEED_0 = 0x03,  // slowest
    SCROLL_SPEED_1 = 0x02,
    SCROLL_SPEED_2 = 0x01,
    SCROLL_SPEED_3 = 0x06,
    SCROLL_SPEED_4 = 0x00,
    SCROLL_SPEED_5 = 0x05,
    SCROLL_SPEED_6 = 0x04,
    SCROLL_SPEED_7 = 0x07   // fastest
}SCROLL_SPEED;

typedef enum{
    SCROLL_PAGE_0 = 0,
    SCROLL_PAGE_1,
    SCROLL_PAGE_2,
    SCROLL_PAGE_3,
    SCROLL_PAGE_4,
    SCROLL_PAGE_5,
    SCROLL_PAGE_6,
    SCROLL_PAGE_7   
}SCROLL_AREA;



// Exported Functions ----------------------------------------------------------

///////////////////////////////////////////////////////
// DISPLAY FUNCTIONS THAT SEND COMMANDS THROUGHT I2C //
///////////////////////////////////////////////////////
void display_init( uint8_t i2caddr );
void display_off( uint8_t i2caddr );
void display_contrast( uint8_t contrast );
void display_invert( uint8_t invert );
void display_stopscroll(void);
void display_scroll( SCROLL_AREA start, SCROLL_AREA end, SCROLL_DIR dir, SCROLL_SPEED speed );
void display_update_int_state_machine (void);
void display_update(void);
unsigned char display_is_free (void);
unsigned char display_update_int_contrast (unsigned char);

////////////////////////////////////////////////
// DISPLAY FUNCTIONS THAT ONLY UPDATES MEMORY //
////////////////////////////////////////////////
void display_clear(void);
//display_setPixel private
//display_line private


#endif    /* _SSD1306_DISPLAY_H	*/

//--- end of file ---//


