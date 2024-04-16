//---------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### SSD1306_PARAMS.H ######################
//---------------------------------------------

#ifndef _SSD1306_PARAMS_H_
#define _SSD1306_PARAMS_H_

//-- Configuration for Oled Display --------------------
// #define OLED_128_64
#define OLED_128_32

#ifdef OLED_128_64
#define DISPLAYWIDTH 128
#define DISPLAYHEIGHT 64
#endif

#ifdef OLED_128_32
#define DISPLAYWIDTH 128
#define DISPLAYHEIGHT 32
#endif


#endif    /* _SSD1306_PARAMS_H_ */

//--- end of file ---//

