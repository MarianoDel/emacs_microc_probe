//-----------------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    STM32G030
// ##
// #### TEMP_SENSOR.H #################################
//-----------------------------------------------------
#ifndef _TEMP_SENSOR_H_
#define _TEMP_SENSOR_H_


// Module Exported Functions ---------------------------------------------------
void Temp_Sensor_SetRef (short new_ref_30);
void Temp_Sensor_Init (unsigned short new_temp);
short Temp_Sensor_GetTemp (unsigned short new_temp);


#endif /* _TEMP_SENSOR_H_ */


