//-------------------------------------------------------
// #### SSD1306 OLED Display I2C Lib ####################
// ##
// ## @Author: Derk Steggewentz, 3/2015 
// ## @Modifications: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### SSD1306_DISPLAY.C ###############################
//-------------------------------------------------------

// Includes --------------------------------------------------------------------
#include "ssd1306_display.h"
#include "i2c.h"


#include <stdlib.h>
#include <string.h>


// Module Private Types & Macros -----------------------------------------------    
#define SSD1306_SETCONTRAST 0x81
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON 0xA5
#define SSD1306_NORMALDISPLAY 0xA6
#define SSD1306_INVERTDISPLAY 0xA7
#define SSD1306_DISPLAYOFF 0xAE
#define SSD1306_DISPLAYON 0xAF
#define SSD1306_SETDISPLAYOFFSET 0xD3
#define SSD1306_SETCOMPINS 0xDA
#define SSD1306_SETVCOMDETECT 0xDB
#define SSD1306_SETDISPLAYCLOCKDIV 0xD5
#define SSD1306_SETPRECHARGE 0xD9
#define SSD1306_SETMULTIPLEX 0xA8
#define SSD1306_SETLOWCOLUMN 0x00
#define SSD1306_SETHIGHCOLUMN 0x10
#define SSD1306_SETSTARTLINE 0x40
#define SSD1306_MEMORYMODE 0x20
#define SSD1306_COLUMNADDR 0x21
#define SSD1306_PAGEADDR 0x22
#define SSD1306_COMSCANINC 0xC0
#define SSD1306_COMSCANDEC 0xC8
#define SSD1306_SEGREMAP 0xA0
#define SSD1306_CHARGEPUMP 0x8D
#define SSD1306_EXTERNALVCC 0x1
#define SSD1306_SWITCHCAPVCC 0x2
#define SSD1306_ACTIVATE_SCROLL 0x2F
#define SSD1306_DEACTIVATE_SCROLL 0x2E


// I2C result status
#define TRANSFER_CMPLT    (0x00u)
#define TRANSFER_ERROR    (0xFFu)


// static uint32 display_write_buf( uint8_t* buf, uint16_t size );
#if defined I2C_USE_I2C1
#define display_write_buf(X,Y)    I2C1_SendMultiByte((X), I2C_ADDRESS_SLV, (Y))
#define display_write_buf_int(X,Y)    I2C1_SendMultiByte_Int(I2C_ADDRESS_SLV, (X), (Y))
#define display_wait_end()    while (!I2C1_CheckEnded_Int())
#define display_check_end()    I2C1_CheckEnded_Int()
#elif defined I2C_USE_I2C2
#define display_write_buf(X,Y)    I2C2_SendMultiByte((X), I2C_ADDRESS_SLV, (Y))
#define display_write_buf_int(X,Y)    I2C2_Int_SendMultiByte((X), I2C_ADDRESS_SLV, (Y))
#define display_wait_end()    while (!I2C2_Int_CheckEnded())
#define display_check_end()    I2C2_Int_CheckEnded()
#else
#error "Select what I2C to use on i2c.h"
#endif


// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------
static uint8_t _i2caddr;
static unsigned char cmdbuf[129] = { 0 };


// Module Private Functions ----------------------------------------------------


// Module Functions ------------------------------------------------------------

///////////////////////////////////////////////////////
// DISPLAY FUNCTIONS THAT SEND COMMANDS THROUGHT I2C //
///////////////////////////////////////////////////////

// call before first use of other functions
void display_init( uint8_t i2caddr )
{

    //TODO: quitar esto de aca por favor!!!!
    // gfx_init( DISPLAYWIDTH, DISPLAYHEIGHT );
    
#ifdef I2C_WITH_INTS    
    cmdbuf[0] = 0x00;
    cmdbuf[1] = SSD1306_DISPLAYOFF;
    cmdbuf[2] = SSD1306_SETDISPLAYCLOCKDIV;
    cmdbuf[3] = 0x80;
    cmdbuf[4] = SSD1306_SETMULTIPLEX;
    cmdbuf[5] = 0x3f;
    cmdbuf[6] = SSD1306_SETDISPLAYOFFSET;
    cmdbuf[7] = 0x00;
    cmdbuf[8] = SSD1306_SETSTARTLINE | 0x0;
    cmdbuf[9] = SSD1306_CHARGEPUMP;
    cmdbuf[10] = 0x14;
    cmdbuf[11] = SSD1306_MEMORYMODE;
    cmdbuf[12] = 0x02;    //cmbio a page de 0x00 a 0x0
    cmdbuf[13] = SSD1306_SEGREMAP | 0x1;
    // cmdbuf[13] = SSD1306_SEGREMAP | 0x0       
    cmdbuf[14] = SSD1306_COMSCANDEC;
    cmdbuf[15] = SSD1306_SETCOMPINS;
    cmdbuf[16] = 0x12;
    cmdbuf[17] = SSD1306_SETCONTRAST;
    cmdbuf[18] = 0xcf;
    cmdbuf[19] = SSD1306_SETPRECHARGE;
    cmdbuf[20] = 0xf1;
    cmdbuf[21] = SSD1306_SETVCOMDETECT;
    cmdbuf[22] = 0x40;
    cmdbuf[23] = SSD1306_DISPLAYALLON_RESUME;
    cmdbuf[24] = SSD1306_NORMALDISPLAY;
    cmdbuf[25] = SSD1306_DISPLAYON;
    
    display_write_buf_int(cmdbuf, 26);
    display_wait_end();
#else
    
    uint8_t cmd[] = {
        0x00,
        SSD1306_DISPLAYOFF,
        SSD1306_SETDISPLAYCLOCKDIV,
        0x80,
        SSD1306_SETMULTIPLEX,
        0x3f,
        SSD1306_SETDISPLAYOFFSET,
        0x00,
        SSD1306_SETSTARTLINE | 0x0,
        SSD1306_CHARGEPUMP,
        0x14,
        SSD1306_MEMORYMODE,
        0x02,    //cmbio a page de 0x00 a 0x02
        SSD1306_SEGREMAP | 0x1,
        // SSD1306_SEGREMAP | 0x0,        
        SSD1306_COMSCANDEC,
        SSD1306_SETCOMPINS,
        0x12,
        SSD1306_SETCONTRAST,
        0xcf,
        SSD1306_SETPRECHARGE,
        0xf1,
        SSD1306_SETVCOMDETECT,
        0x40,
        SSD1306_DISPLAYALLON_RESUME,
        SSD1306_NORMALDISPLAY,
        SSD1306_DISPLAYON,
    };
    
    display_write_buf( cmd, sizeof(cmd) ); 
#endif
  
}


// useful to turn off display if not used by application
// w/o going through the init process
void display_off( uint8_t i2caddr )
{
    _i2caddr = i2caddr;

#ifdef I2C_WITH_INTS        

    cmdbuf[0] = 0x00;
    cmdbuf[1] = SSD1306_DISPLAYOFF;
    display_write_buf_int( cmdbuf, 2 );
    display_wait_end();
    
#else
    uint8_t cmd[] = {
        0x00,
        SSD1306_DISPLAYOFF
    };
    display_write_buf( cmd, sizeof(cmd) );        
#endif

}


// contrast: 0 ...255
void display_contrast( uint8_t contrast )
{
#ifdef I2C_WITH_INTS        

    cmdbuf[0] = 0x00;
    cmdbuf[1] = SSD1306_SETCONTRAST;
    cmdbuf[2] = contrast;

    display_write_buf_int( cmdbuf, 3 );
    display_wait_end();
    
#else
    uint8_t cmd[] = {
        0x00,  
        SSD1306_SETCONTRAST,
        contrast
    };
    display_write_buf( cmd, sizeof(cmd) );
#endif
}


// invert <> 0 for inverse display, invert == 0 for normal display
void display_invert ( uint8_t invert )
{
#ifdef I2C_WITH_INTS

    cmdbuf[0] = 0x00;
    cmdbuf[1] = invert ? SSD1306_INVERTDISPLAY : SSD1306_NORMALDISPLAY;
    display_write_buf_int( cmdbuf, 2);
    display_wait_end();
#else
    uint8_t cmd[] = {
        0x00,  
        0
    };
    cmd[1] = invert ? SSD1306_INVERTDISPLAY : SSD1306_NORMALDISPLAY;
    display_write_buf( cmd, sizeof(cmd) );
#endif
}


void display_stopscroll(void)
{
#ifdef I2C_WITH_INTS
    cmdbuf[0] = 0x00;
    cmdbuf[1] = SSD1306_DEACTIVATE_SCROLL;

    display_write_buf_int( cmdbuf, 2 );
    display_wait_end();

#else
    uint8_t cmd[] = {
        0x00,
        SSD1306_DEACTIVATE_SCROLL
    };
    display_write_buf( cmd, sizeof(cmd) );
#endif
}


void display_scroll( SCROLL_AREA start, SCROLL_AREA end, SCROLL_DIR dir, SCROLL_SPEED speed )
{
#ifdef I2C_WITH_INTS

    cmdbuf[0] = 0x00;
    cmdbuf[1] = dir;                    // 0x26 or 0x2
    cmdbuf[2] = 0x00;                   // dummy byt
    cmdbuf[3] = start;                  // start pag
    cmdbuf[4] = speed;                  // scroll step interval in terms of frame frequency
    cmdbuf[5] = end;                    // end pag
    cmdbuf[6] = 0x00;                   // dummy byt
    cmdbuf[7] = 0xFF;                   // dummy byt
    cmdbuf[8] = SSD1306_ACTIVATE_SCROLL; // 0x2

    display_write_buf_int( cmdbuf, 9);
    display_wait_end();

#else
    uint8_t cmd[] = {
        0x00,
        dir,                    // 0x26 or 0x2a
        0x00,                   // dummy byte
        start,                  // start page
        speed,                  // scroll step interval in terms of frame frequency 
        end,                    // end page
        0x00,                   // dummy byte
        0xFF,                   // dummy byte
        SSD1306_ACTIVATE_SCROLL // 0x2F
    };
    display_write_buf( cmd, sizeof(cmd) );
#endif
}




typedef enum {
    DISPLAY_UPDATE_INIT = 0,
    DISPLAY_UPDATE_SET_PAGE_CMD_0,
    DISPLAY_UPDATE_SET_PAGE_CMD_0_END,
    DISPLAY_UPDATE_SET_PAGE_CMD_1,
    DISPLAY_UPDATE_SET_PAGE_CMD_1_END,
    DISPLAY_UPDATE_SET_PAGE_CMD_2,
    DISPLAY_UPDATE_SET_PAGE_CMD_2_END,
    DISPLAY_UPDATE_SEND_PAGE,
    DISPLAY_UPDATE_SEND_PAGE_END,
    DISPLAY_UPDATE_ENDED

} d_update_st_t;

unsigned char d_update_post_update = 0;
unsigned char d_update_page = 0;
d_update_st_t d_update_st = DISPLAY_UPDATE_ENDED;
extern uint8_t SSD1306_buffer[];
void display_update_int_state_machine (void)
{
#ifdef I2C_WITH_INTS
    switch (d_update_st)
    {
    case DISPLAY_UPDATE_INIT:
        d_update_page = 0;
        d_update_st++;
        break;

    case DISPLAY_UPDATE_SET_PAGE_CMD_0:
        //seteo el 0 de cada pagina
        cmdbuf[0] = 0x00;
        cmdbuf[1] = 0x02;    //page addr 0

        display_write_buf_int( cmdbuf, 2);
        d_update_st++;
        break;

    case DISPLAY_UPDATE_SET_PAGE_CMD_0_END:
        if (display_check_end())
            d_update_st++;

        break;

    case DISPLAY_UPDATE_SET_PAGE_CMD_1:
        //seteo el 0 de cada pagina
        cmdbuf[0] = 0x00;
        cmdbuf[1] = 0x10;    //page addr 0

        display_write_buf_int( cmdbuf, 2);
        d_update_st++;
        break;

    case DISPLAY_UPDATE_SET_PAGE_CMD_1_END:
        if (display_check_end())
            d_update_st++;

        break;

    case DISPLAY_UPDATE_SET_PAGE_CMD_2:
        //seteo el 0 de cada pagina
        cmdbuf[0] = 0x00;
        cmdbuf[1] = 0xB0 | d_update_page;    //seteo la pagina

        display_write_buf_int( cmdbuf, 2);
        d_update_st++;
        break;

    case DISPLAY_UPDATE_SET_PAGE_CMD_2_END:
        if (display_check_end())
            d_update_st++;

        break;
        
    case DISPLAY_UPDATE_SEND_PAGE:
        cmdbuf[0] = 0x40;
        memcpy(cmdbuf + 1, SSD1306_buffer+1 + d_update_page * 128, 128);
        display_write_buf_int( cmdbuf, sizeof(cmdbuf) );    //size cmdbuf = 129
        d_update_st++;
        break;

    case DISPLAY_UPDATE_SEND_PAGE_END:
        if (display_check_end())        
        {
            if (d_update_page < 7)
            {
                d_update_page++;
                d_update_st = DISPLAY_UPDATE_SET_PAGE_CMD_0;
            }
            else
            {
                if (d_update_post_update)
                {
                    d_update_post_update--;
                    d_update_st = DISPLAY_UPDATE_INIT;
                }
                else
                    d_update_st = DISPLAY_UPDATE_ENDED;
            }
        }
        break;

    case DISPLAY_UPDATE_ENDED:
    default:
        break;
    }
#endif
}


unsigned char display_is_free (void)
{
    if (d_update_st == DISPLAY_UPDATE_ENDED)
        return 1;

    return 0;
    
}

void display_update (void)
{
#ifdef I2C_WITH_INTS

    //check if can send it, else ask for postupdate
    if (d_update_st == DISPLAY_UPDATE_ENDED)
        d_update_st = DISPLAY_UPDATE_INIT;
    else
    {
        if (d_update_post_update < 2)
            d_update_post_update++;
    }
    
#else
    uint8_t cmd[2] = { 0 };
    uint8_t datab[129];

    // en page mode escribo a las 8 paginas
    for (unsigned char page = 0; page < 8; page++)
    {
        //seteo el 0 de cada pagina
        cmd[0] = 0x00;
        cmd[1] = 0x02;    //page addr 0
        display_write_buf( cmd, sizeof(cmd) ); 
        
        cmd[0] = 0x00;
        cmd[1] = 0x10;    //page addr 0
        display_write_buf( cmd, sizeof(cmd) ); 

        //seteo la pagina
        cmd[1] = 0xB0 | page;

        display_write_buf( cmd, sizeof(cmd) ); 

        datab[0] = 0x40;
        memcpy(datab + 1, SSD1306_buffer + 1 + page * 128, 128);
        display_write_buf( datab, sizeof(datab) ); 
    }
#endif    
}


// TODO: mejorar esto o agregarlo a todos los comandos
// responde 0 si no puede
// 1 si puede
unsigned char display_update_int_contrast (unsigned char new_contrast)
{
    unsigned char done = 0;
    if (d_update_st == DISPLAY_UPDATE_ENDED)
    {
        display_contrast(new_contrast);
        done = 1;
    }

    return done;
}




////////////////////////////////////////////////
// DISPLAY FUNCTIONS THAT ONLY UPDATES MEMORY //
////////////////////////////////////////////////
extern uint8_t* _displaybuf;
extern uint16_t _displaybuf_size;
void display_clear(void)
{
    memset( _displaybuf, 0x00, _displaybuf_size );
    SSD1306_buffer[0] = 0x40; // to be sure its there
}





