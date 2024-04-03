//----------------------------------------
// Tests for gtk lib
//----------------------------------------

#include <gtk/gtk.h>
#include <stdint.h>
#include "tests_oled_application.h"


// Module Private Types Constants and Macros -----------------------------------
typedef struct {
    int R;
    int G;
    int B;
} rgb_st;

#ifdef OLED_128_64
#define DISPLAYHEIGHT    64
#define DISPLAYWIDTH    128
#endif

#ifdef OLED_128_32
#define DISPLAYHEIGHT    32
#define DISPLAYWIDTH    128
#endif


// Module Private Functions ----------------------------------------------------
void end_program (GtkWidget *, gpointer);
static void init_surface (void);
// static void fill_surface (void);
static void unfill_surface (void);

static void draw_patch (GdkPixbuf * p, int x1, int x2, int y1, int y2, rgb_st *);

static void put_pixel (GdkPixbuf *pixbuf,
                       int x,
                       int y,
                       guchar red,
                       guchar green,
                       guchar blue,
                       guchar alpha);

    
gboolean Print_Callback (gpointer user_data);    //send data for testing pourpose

static void draw_oled_pixel_bright (int row, int col);
static void draw_oled_pixel_dark (int row, int col);

void sw1_button_function (void);
void sw2_button_function (void);



// Globals ---------------------------------------------------------------------
GdkPixbuf * pix;
GtkWidget *imag1;


//Module Functions -------------------------------------------------------------


int main(int argc, char *argv[])
{
    gtk_init (&argc, &argv);
    GtkBuilder *builder = gtk_builder_new();
#ifdef OLED_128_64
    gtk_builder_add_from_file (builder, "oled_128_64.glade", NULL);
#endif

#ifdef OLED_128_32    
    gtk_builder_add_from_file (builder, "oled_128_32.glade", NULL);
#endif

    //widgets from builder
    GtkWidget *win = (GtkWidget *) gtk_builder_get_object (builder, "window1");
    GtkWidget *btn1 = (GtkWidget *) gtk_builder_get_object (builder, "ccwButton");
    GtkWidget *btn2 = (GtkWidget *) gtk_builder_get_object (builder, "cwButton");
    GtkWidget *btn3 = (GtkWidget *) gtk_builder_get_object (builder, "setButton");    
    GtkWidget *btn4 = (GtkWidget *) gtk_builder_get_object (builder, "quitButton");
    imag1 = (GtkWidget *) gtk_builder_get_object (builder, "image1");

    g_signal_connect(btn1, "clicked", G_CALLBACK(cw_button_function), NULL);    
    g_signal_connect(btn2, "clicked", G_CALLBACK(ccw_button_function), NULL);
    g_signal_connect(btn3, "clicked", G_CALLBACK(set_button_function), NULL);    
    g_signal_connect(btn4, "clicked", G_CALLBACK(end_program), NULL);    

    gtk_widget_show_all (win);
    init_surface();

    // gint mydata = 5;
    // gpointer data = (gpointer *) &mydata;
    // gdk_threads_add_timeout (1000, Print_Callback, data);

    gdk_threads_add_timeout (1000, Test_Timeouts_Loop_1000ms, NULL);    
    gdk_threads_add_timeout (1, Test_Timeouts_Loop_1ms, NULL);
    gdk_threads_add_idle (Test_Main_Loop, NULL);    
    gtk_main();
    
    return 0;
}


// This one is for internal tests only -----------------------------------------
gint new_loop = 0;
gint loops = 0;
gboolean Print_Callback (gpointer user_data)
{
    gint * udata = user_data;
    if (!new_loop)
    {
        g_print("data getted: %d\n", *udata);
        loops = *udata;
        new_loop = 1;
    }

    if (loops)
    {
        g_print("new loop: %d\n", loops);
        loops--;
    }
    else
    {
        g_print("ending loop\n");
        return FALSE;
    }
    
    return TRUE;
}
// End of internal tests -------------------------------------------------------




void end_program (GtkWidget *wid, gpointer ptr)
{
    gtk_main_quit();
}


static void init_surface (void)
{
    gint width;
    gint height;
#if defined OLED_128_64
    gdk_pixbuf_get_file_info ("oled_128_64.jpeg", &width, &height);
    g_print("File is: %d width %d height\n", width, height);
                          
    GError *gerror = NULL;
    pix = gdk_pixbuf_new_from_file ("oled_128_64.jpeg", &gerror);

    if (pix == NULL)
    {
        g_print("Error loading pix: %s\n", gerror->message);
        g_error_free(gerror);
    }
    else
    {
        g_print("pix loaded\n");
        unfill_surface();
    }
#elif defined OLED_128_32
    gdk_pixbuf_get_file_info ("oled_128_32_2.jpeg", &width, &height);
    g_print("File is: %d width %d height\n", width, height);
                          
    GError *gerror = NULL;
    pix = gdk_pixbuf_new_from_file ("oled_128_32_2.jpeg", &gerror);

    if (pix == NULL)
    {
        g_print("Error loading pix: %s\n", gerror->message);
        g_error_free(gerror);
    }
    else
    {
        g_print("pix loaded\n");
        unfill_surface();
    }
#else
#error "Please select what king of Oled to use"    
#endif
}


static void draw_patch (GdkPixbuf * p, int x1, int x2, int y1, int y2, rgb_st * rgb)
{
    for (int j = y1; j < y2; j++)
    {
        for (int i = x1; i < x2; i++)
            put_pixel(p, i, j, rgb->R, rgb->G, rgb->B, 0);
    }
}


#if defined OLED_128_64
#define PATCH_COLUMN_START    30
#define PATCH_WIDTH    2
#define PATCH_ROW_START    84
#define PATCH_HEIGHT    2
#elif defined OLED_128_32
#define PATCH_COLUMN_START    46
#define PATCH_WIDTH    3
#define PATCH_ROW_START    120
#define PATCH_HEIGHT    4
#endif
static void unfill_surface (void)
{
    rgb_st rgb;
    rgb.R = 14;
    rgb.G = 16;
    rgb.B = 37;    // blue background
    // rgb.R = 13;
    // rgb.G = 251;
    // rgb.B = 251;    // blue foreground
    // rgb.R = 115;
    // rgb.G = 115;
    // rgb.B = 115;    // grey background
    // rgb.R = 135;
    // rgb.G = 175;
    // rgb.B = 53;    // green as in lcd

#if defined OLED_128_64    
    int x1 = PATCH_COLUMN_START;
    int x2 = x1 + 128 * PATCH_WIDTH;
    int y1 = PATCH_ROW_START;
    int y2 = y1 + 64 * PATCH_HEIGHT;
#elif defined OLED_128_32
    int x1 = PATCH_COLUMN_START;
    int x2 = x1 + 128 * PATCH_WIDTH;
    int y1 = PATCH_ROW_START;
    int y2 = y1 + 32 * PATCH_HEIGHT;    
#endif

    draw_patch(pix, x1, x2, y1, y2, &rgb);
    
    gtk_image_set_from_pixbuf (GTK_IMAGE(imag1), pix);
}


static void draw_oled_pixel_bright (int row, int col)
{
    rgb_st rgb;
    rgb.R = 13;
    rgb.G = 251;
    rgb.B = 251;    // blue foreground

    if ((row > 63) || (col > 127))
        return;   

    int x1 = PATCH_COLUMN_START + col * PATCH_WIDTH;
    int x2 = x1 + PATCH_WIDTH;
    int y1 = PATCH_ROW_START + row * PATCH_HEIGHT;
    int y2 = y1 + PATCH_HEIGHT;

    draw_patch(pix, x1, x2, y1, y2, &rgb);
}


static void draw_oled_pixel_dark (int row, int col)
{
    rgb_st rgb;
    rgb.R = 14;
    rgb.G = 16;
    rgb.B = 37;    // blue background

    if ((row > 63) || (col > 127))
        return;   

    int x1 = PATCH_COLUMN_START + col * PATCH_WIDTH;
    int x2 = x1 + PATCH_WIDTH;
    int y1 = PATCH_ROW_START + row * PATCH_HEIGHT;
    int y2 = y1 + PATCH_HEIGHT;

    draw_patch(pix, x1, x2, y1, y2, &rgb);
}


// static void fill_surface (void)
// {
//     // fill_with_patches(pix);
//     fill_with_lcd_patches(pix);    
//     gtk_image_set_from_pixbuf (GTK_IMAGE(imag1), pix);    
// }

static void put_pixel (GdkPixbuf *pixbuf,
                       int x,
                       int y,
                       guchar red,
                       guchar green,
                       guchar blue,
                       guchar alpha)
{
    int width, height, rowstride, n_channels;
    guchar *pixels, *p;

    n_channels = gdk_pixbuf_get_n_channels (pixbuf);
    // g_print("n_channels: %d\n", n_channels);

    g_assert (gdk_pixbuf_get_colorspace (pixbuf) == GDK_COLORSPACE_RGB);
    g_assert (gdk_pixbuf_get_bits_per_sample (pixbuf) == 8);
    // g_assert (gdk_pixbuf_get_has_alpha (pixbuf));
    // g_assert (n_channels == 4);

    width = gdk_pixbuf_get_width (pixbuf);
    height = gdk_pixbuf_get_height (pixbuf);

    g_assert (x >= 0 && x < width);
    g_assert (y >= 0 && y < height);

    rowstride = gdk_pixbuf_get_rowstride (pixbuf);
    pixels = gdk_pixbuf_get_pixels (pixbuf);

    p = pixels + y * rowstride + x * n_channels;
    p[0] = red;
    p[1] = green;
    p[2] = blue;
    // p[3] = alpha;
}


// I2C Mock Functions ----------------------------------------------------------
extern uint8_t SSD1306_buffer[];
void I2C1_SendMultiByte (unsigned char *pdata, unsigned char addr, unsigned short size)
{
    unsigned char * pmem;
    
    // check if first byte is write cmd
    if (*pdata != 0x40)
        return;

    int row = 0;
    int col = 0;

    // g_print("send mem cmd %d with size: %d\n", *(pdata + 0), size);

    // in reality I finish sending all mem data
    pmem = SSD1306_buffer + 1;
            

    // 8 pages of 128 bytes each = 1024
    for (int page_cntr = 0; page_cntr < DISPLAYHEIGHT / 8; page_cntr++)
    {
        //8 lines from 128 bytes (DISPLAYWIDTH)
        // bit 0 line 0
        // bit 7 line 7
        int page_offset = page_cntr * DISPLAYWIDTH;

        for (int bit_offset = 0; bit_offset < 8; bit_offset++)
        {
            unsigned char c = 1;    // bit mask in line
            c <<= bit_offset;
            
            for (int i = 0; i < DISPLAYWIDTH; i++)
            {
                col = i;
                row = page_cntr * 8 + bit_offset;
                
                if (*(pmem + i + page_offset) & c)
                    draw_oled_pixel_bright(row, col);
                else
                    draw_oled_pixel_dark(row, col);
            }
        }
    }

    // efective draw and show the new pixels
    gtk_image_set_from_pixbuf (GTK_IMAGE(imag1), pix);
}


void I2C1_SendMultiByte_Int (unsigned char addr, unsigned char *pdata, unsigned short size)
{
    I2C1_SendMultiByte (pdata, addr, size);
}


unsigned char I2C1_CheckEnded_Int (void)
{
    return 1;
}
// Testing Function loop -------------------------------------------------------
// static GMutex mutex;


// extern volatile unsigned short show_select_timer;
// volatile unsigned short mode_effect_timer = 0;
// unsigned char mode_state;

// volatile unsigned char dmx_buff_data[3] = { 0 };
// volatile unsigned char Packet_Detected_Flag = 0;
// volatile unsigned short DMX_channel_selected = 1;
// parameters_typedef mem_conf;
// unsigned char pwm_channels [2] = { 0 };

// sw_actions_t switch_actions = selection_none;
// int setup_done = 0;
// gboolean Test_Main_Loop (gpointer user_data)
// {
//     resp_t resp = resp_continue;

//     // mem_conf.channels_operation_mode = 1;
//     resp = ManualMode (pwm_channels, switch_actions);

//     if (resp == resp_need_to_save)
//     {
//         printf("memory needs a save!\n");
//     }

//     //wraper to clean sw
//     g_mutex_lock (&mutex);

//     if (switch_actions != selection_none)
//         switch_actions = selection_none;
    
//     g_mutex_unlock (&mutex);
//     // usleep(500);
        
//     return TRUE;
// }


// gboolean Test_Timeouts (gpointer user_data)
// {
//     //timeout lcd_utils internal
//     if (show_select_timer)
//         show_select_timer--;

//     //timeout for dmx_mode or manual_mode
//     if (mode_effect_timer)
//         mode_effect_timer--;
    
//     return TRUE;
// }

// //buttons functions
// void dwn_button_function (void)
// {
//     g_mutex_lock (&mutex);
//     switch_actions = selection_dwn;
//     g_mutex_unlock (&mutex);
// }

// void up_button_function (void)
// {
//     g_mutex_lock (&mutex);
//     switch_actions = selection_up;
//     g_mutex_unlock (&mutex);
// }

// void set_button_function (void)
// {
//     g_mutex_lock (&mutex);
//     switch_actions = selection_enter;
//     g_mutex_unlock (&mutex);
// }



//--- end of file ---//
