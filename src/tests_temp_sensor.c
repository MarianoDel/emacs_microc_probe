//---------------------------------------------------
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    TEST PLATFORM FOR FIRMWARE
// ##
// #### TESTS_TEMP_SENSOR.C #########################
//---------------------------------------------------

// Includes Modules for tests --------------------------------------------------
#include "temp_sensor.h"

//helper modules
#include "tests_ok.h"

#include <stdio.h>
#include <string.h>


// Externals - Globals on Module to Test ---------------------------------------
extern unsigned short ref_30;


// Globals ---------------------------------------------------------------------


// Module Auxialiary Functions -------------------------------------------------


// Module Functions for testing ------------------------------------------------
void Test_RefSet (void);
void Test_Init (void);
void Test_GetTemp (void);


// Module Functions ------------------------------------------------------------
int main(int argc, char *argv[])
{

    Test_RefSet ();

    Test_Init ();

    Test_GetTemp ();    

    return 0;
}


void Test_RefSet (void)
{
    printf("Test Temp_Sensor -- RefSet\n");

    printf(" -- original set: %d", ref_30);

    float adj = 0.0;
    adj = ref_30 * 3.0 / 3.2;
    unsigned short new_ref_30 = (unsigned short) adj;

    printf(" adj for 3.2V: %d\n", new_ref_30);
    
    Temp_Sensor_SetRef (new_ref_30);

    printf("Result: ");
    if (new_ref_30 == ref_30)
        PrintOK();
    else
        PrintERR();
    
    
}


void Test_Init (void)
{
    printf("Test Temp_Sensor -- Init\n");

    Temp_Sensor_SetRef (1037);
    printf(" -- original set: %d\n", ref_30);

    Temp_Sensor_Init (1037);
    short new_temp = Temp_Sensor_GetTemp (1037);

    printf("Result: ");
    if (new_temp == 30)
        PrintOK();
    else
    {
        PrintERR();
        printf(" setted: %d getted: %d\n", 30, new_temp);
    }
}


void Test_GetTemp (void)
{
    printf("Test Temp_Sensor -- GetTemp\n");

    Temp_Sensor_SetRef (1037);
    printf(" -- original set: %d\n", ref_30);

    Temp_Sensor_Init (1037);
    short new_temp = Temp_Sensor_GetTemp (1037);

    printf("temp: ");
    for (int i = 0; i < 16; i++)
    {
        new_temp = Temp_Sensor_GetTemp (1105);
        printf("%dC ", new_temp);
    }
    printf("\n");
    
    printf("Result: ");
    if ((new_temp <= (50 + 1)) ||
        (new_temp >= (50 - 1)))
        PrintOK();
    else
    {
        PrintERR();
        printf(" setted: %d getted: %d\n", 50, new_temp);
    }
}


// Module Auxiliary Functions --------------------------------------------------


// Module Mocked Functions -----------------------------------------------------


//--- end of file ---//


