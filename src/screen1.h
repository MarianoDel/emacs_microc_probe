//---------------------------------------------
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    STM32F303
// ##
// #### SCREEN.H ##############################
//---------------------------------------------
#ifndef _SCREEN_H_
#define _SCREEN_H_


// Config Defines --------------------------------------------------------------


// Common Defines --------------------------------------------------------------
    

// Module Exported Functions ---------------------------------------------------
void SCREEN_Init (void);
void SCREEN_ShowText2 (char *, char *, char *, char *);

void SCREEN_Text2_Line1 (char * line1);
void SCREEN_Text2_Line2 (char * line2);
void SCREEN_Clear (void);

void SCREEN_Text2_BlankLine1 (void);
void SCREEN_Text2_BlankLine2 (void);
    
#endif    /* _SCREEN_H_ */
