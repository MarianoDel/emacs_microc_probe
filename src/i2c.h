//---------------------------------------------
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    STM32F103
// ##
// #### I2C.H #################################
//---------------------------------------------
#ifndef _I2C_H_
#define _I2C_H_

// Config Defines --------------------------------------------------------------
#define I2C_ADDRESS_OWN    0x01
#define I2C_ADDRESS_SLV    0x3C    //para 32?
// #define I2C_ADDRESS_SLV    0x3D    //para 64?
#define I2C_ADDRESS_MSK    0x7F

#define I2C_USE_I2C1
// #define I2C_USE_I2C2
#define I2C_WITH_INTS

// Common Defines --------------------------------------------------------------
#define I2C_CR2_NBYTES_Pos           (16U)                                     
#define I2C_CR2_NBYTES_Msk           (0xFFU << I2C_CR2_NBYTES_Pos)             /*!< 0x00FF0000 */
#define I2C_CR2_RELOAD_Pos           (24U)                                     
#define I2C_CR2_RELOAD_Msk           (0x1U << I2C_CR2_RELOAD_Pos)              /*!< 0x01000000 */
#define I2C_CR2_AUTOEND_Pos          (25U)                                     
#define I2C_CR2_AUTOEND_Msk          (0x1U << I2C_CR2_AUTOEND_Pos)             /*!< 0x02000000 */
#define I2C_CR2_SADD_Pos             (0U)                                      
#define I2C_CR2_SADD_Msk             (0x3FFU << I2C_CR2_SADD_Pos)              /*!< 0x000003FF */

// Module Exported Functions ---------------------------------------------------
void I2C1_Init (void);
void I2C1_SendByte (unsigned char addr, unsigned char data);
unsigned char I2C1_SendAddr (unsigned char addr);
void I2C1_SendMultiByte (unsigned char *pdata, unsigned char addr, unsigned short size);
void I2C1_SendMultiByte_Int (unsigned char addr, unsigned char *pdata, unsigned short size);
unsigned char I2C1_CheckEnded_Int (void);

void I2C2_Init (void);
void I2C2_SendByte (unsigned char, unsigned char);


#endif    /* _I2C_H_ */

//--- end of file ---//

