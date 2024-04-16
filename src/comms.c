//-------------------------------------------------------
// #### PROJECT: Magnet Antenna  G030 - Custom Board ####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### COMMS.C #########################################
//-------------------------------------------------------
#include "comms.h"

#include <string.h>


// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------


// Module Functions ------------------------------------------------------------
comms_answers_e Comms_CheckMsg (char * pStr)
{

    // if (strncmp(pStr, "get_params", sizeof("get_params") - 1) == 0)
    //     return COMMS_GET_PARAMS;

    // if (strncmp(pStr, "get_temp", sizeof("get_temp") - 1) == 0)
    //     return COMMS_GET_TEMP;

    if (strncmp(pStr, "keepalive", sizeof("keepalive") - 1) == 0)
        return COMMS_KEEPALIVE;

    if (strncmp(pStr, "get_name", sizeof("get_name") - 1) == 0)
        return COMMS_GET_NAME;

    // if (strncmp(pStr, "set_display", sizeof("set_display") - 1) == 0)
    //     return SET_DISPLAY;


    return COMMS_ERROR;
}

