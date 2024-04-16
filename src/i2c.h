//---------------------------------------------
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    STM32G030
// ##
// #### I2C.H #################################
//---------------------------------------------
#ifndef _I2C_H_
#define _I2C_H_

// Config Defines --------------------------------------------------------------
// #define I2C_USE_I2C1
#define I2C_USE_I2C2

#define I2C_ADDRESS_OWN    0x01
#define I2C_ADDRESS_SLV    0x3C    //para 32?
// #define I2C_ADDRESS_SLV    0x3D    //para 64?
#define I2C_ADDRESS_MSK    0x7F


// #define I2C_WITH_INTS

// Common Defines --------------------------------------------------------------
#define RCC_I2C1_CLK    (RCC->APBENR1 & 0x00200000)
#define RCC_I2C1_CLKEN    (RCC->APBENR1 |= 0x00200000)
#define RCC_I2C1_CLKDIS    (RCC->APBENR1 &= ~0x00200000)

#define RCC_I2C2_CLK    (RCC->APBENR1 & 0x00400000)
#define RCC_I2C2_CLKEN    (RCC->APBENR1 |= 0x00400000)
#define RCC_I2C2_CLKDIS    (RCC->APBENR1 &= ~0x00400000)

// Module Exported Functions ---------------------------------------------------
#ifdef I2C_USE_I2C1
void I2C1_Init (void);
void I2C1_SendByteTest (unsigned char);
void I2C1_SendByte (unsigned char, unsigned char);
void I2C1_SendAddr (unsigned char);
void I2C1_SendMultiByte (unsigned char *, unsigned char, unsigned short);
#endif

#ifdef I2C_USE_I2C2
void I2C2_Init (void);
void I2C2_SendByteTest (unsigned char);
void I2C2_SendByte (unsigned char, unsigned char);
void I2C2_SendAddr (unsigned char);
void I2C2_SendMultiByte (unsigned char *, unsigned char, unsigned short);
void I2C2_IRQHandler (void);

#ifdef I2C_WITH_INTS
unsigned char I2C2_Int_CheckEnded (void);
void I2C2_Int_SendMultiByte (unsigned char *, unsigned char, unsigned short);
#endif
#endif

#endif    /* _I2C_H_ */

//--- end of file ---//

