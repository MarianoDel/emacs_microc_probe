//-------------------------------------------------------
// #### SSD1306 OLED GFX Lib ############################
// ##
// ## @Author: Derk Steggewentz, 3/2015 
// ## @Modifications: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### SSD1306_GFX.H ###################################
//-------------------------------------------------------

#ifndef _SSD1306_GFX_H
#define _SSD1306_GFX_H

#include <stdint.h>


// Exported Types Constants and Macros -----------------------------------------
#define BLACK 0
#define WHITE 1
#define INVERSE 2   


// Exported Functions ----------------------------------------------------------

// ============================================================
// graphics library stuff

void gfx_init( int16_t width, int16_t height );
int16_t gfx_width(void);
int16_t gfx_height(void);
//rotation private
void gfx_setCursor( int16_t x, int16_t y );
void gfx_setTextSize( uint8_t size );
void gfx_setTextColor( uint16_t color );
void gfx_setTextBg( uint16_t background );
void gfx_setTextWrap( uint8_t w );
void gfx_setRotation( uint8_t x );
//rotation_adjust private
void gfx_drawPixel(int16_t x, int16_t y, uint16_t color);
//hvLine private
void gfx_drawLine( int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color );
void gfx_drawRect( int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color );
void gfx_fillRect( int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color );
void gfx_drawCircle( int16_t x0, int16_t y0, int16_t r,uint16_t color );
void gfx_drawTriangle( int16_t x0, int16_t y0,int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color );
//drawChar private
void gfx_write( uint8_t ch );
void gfx_print( const char* s );
void gfx_println( const char* s );

#endif    /* _SSD1306_GFX_H */

//--- end of file ---//
