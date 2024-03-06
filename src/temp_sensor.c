//---------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    STM32G030
// ##
// #### TEMP_SENSOR.C #########################
//---------------------------------------------

// Includes --------------------------------------------------------------------
#include "temp_sensor.h"
#include "dsp.h"


// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------
ma16_u16_data_obj_t temp_filter;
short ref_30 = 1037;    // factory default


// Module Private Functions ----------------------------------------------------
short ConvertTemp (unsigned short adc_temp);


// Module Functions ------------------------------------------------------------
void Temp_Sensor_SetRef (short new_ref_30)
{
    ref_30 = new_ref_30;
}


void Temp_Sensor_Init (unsigned short new_temp)
{
    MA16_U16Circular_Reset (&temp_filter);

    for (int i = 0; i < 16; i++)
        MA16_U16Circular (&temp_filter, new_temp);
    
}


// return temperature mean in degrees
short Temp_Sensor_GetTemp (unsigned short new_temp)
{
    unsigned short new_temp_filtered = 0;
    new_temp_filtered = MA16_U16Circular (&temp_filter, new_temp);

    return ConvertTemp (new_temp_filtered);
}


short ConvertTemp (unsigned short adc_temp)
{
    // slope = 2.5mV/C -> 3.2pts/C
    // reference 30C 1037pts; or factory setted @ 3Vcc
    int calc = 0;
    
    calc = adc_temp - ref_30;
    calc = calc * 10;
    calc = calc / 32;
    calc = calc + 30;
    
    return (short) calc;
}


//--- end of file ---//


