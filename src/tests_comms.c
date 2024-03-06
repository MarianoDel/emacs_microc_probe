//---------------------------------------------
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    TEST PLATFORM FOR FIRMWARE
// ##
// #### TESTS_COMMS.C #########################
//---------------------------------------------

// Includes Modules for tests --------------------------------------------------
#include "comms.h"

//helper modules
#include "tests_ok.h"

#include <stdio.h>
#include <string.h>


// Externals - Globals on Module to Test ---------------------------------------



// Globals ---------------------------------------------------------------------


// Module Auxialiary Functions -------------------------------------------------


// Module Functions for testing ------------------------------------------------
void Test_Comms (void);


// Module Functions ------------------------------------------------------------
int main(int argc, char *argv[])
{

    Test_Comms ();

    return 0;
}


void Test_Comms (void)
{
    char s_test [100] = { 0 };
    int error = 0;
    comms_answers_e resp = 0;
    
    printf("Test Comms\n");

    if (!error)
    {
        strcpy (s_test, "keepalive");
        printf(" test %s\n", s_test);
        resp = Comms_CheckMsg (s_test);
        if (resp != COMMS_KEEPALIVE)
            error = 1;
    }

    if (!error)
    {
        strcpy (s_test, "get_temp");
        printf(" test %s\n", s_test);
        resp = Comms_CheckMsg (s_test);
        if (resp != COMMS_GET_TEMP)
            error = 1;
    }

    if (!error)
    {
        strcpy (s_test, "get_params");
        printf(" test %s\n", s_test);
        resp = Comms_CheckMsg (s_test);
        if (resp != COMMS_GET_PARAMS)
            error = 1;
    }

    if (!error)
    {
        strcpy (s_test, "get_name");
        printf(" test %s\n", s_test);
        resp = Comms_CheckMsg (s_test);
        if (resp != COMMS_GET_NAME)
            error = 1;
    }


    if (!error)
    {
        strcpy (s_test, "get_namx");
        printf(" test %s error str\n", s_test);
        resp = Comms_CheckMsg (s_test);
        if (resp != COMMS_ERROR)
            error = 1;
    }
    
    printf("Result: ");
    if (error == 0)
        PrintOK();
    else
        PrintERR();
    
}




// Module Auxiliary Functions --------------------------------------------------


// Module Mocked Functions -----------------------------------------------------


//--- end of file ---//


