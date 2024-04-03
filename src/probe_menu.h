//-------------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### PROBE_MENU.H ##############################
//-------------------------------------------------

// Prevent recursive inclusion -------------------------------------------------
#ifndef _PROBE_MENU_H_
#define _PROBE_MENU_H_


// Module Exported Types Constants and Macros ----------------------------------
typedef enum {
    DO_NOTHING = 0,
    SHOW_INIT,
    SHOW_CONNECT,
    SHOW_DISCONNECT,    
    SHOW_START    
    
} probe_actions_e;


// Module Exported Functions ---------------------------------------------------
void ProbeMenu_UpdateTimer (void);
void ProbeMenu_Reset (void);
void ProbeMenu (probe_actions_e action);


#endif    /* _PROBE_MENU_H_ */

//--- end of file ---//

