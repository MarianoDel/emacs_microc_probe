//----------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    STM32F030
// ##
// #### USART.H ################################
//----------------------------------------------
// Prevent recursive inclusion -------------------------------------------------
#ifndef _USART_H_
#define _USART_H_


// Module Exported Types Constants and Macros ------------------------------------------------------
#define SIZEOF_DATA    256
// #define SIZEOF_TXDATA    128


// Module Exported Functions ---------------------------------------------------
void Usart1Config(void);
void Usart1SendSingle (unsigned char);
void Usart1Send (char *);
void Usart1SendUnsigned(unsigned char *, unsigned char);
unsigned char Usart1ReadBuffer (unsigned char *, unsigned short);
void USART1_IRQHandler(void);
unsigned char Usart1HaveData (void);
void Usart1HaveDataReset (void);
void Usart1RxDisable (void);
void Usart1RxEnable (void);
unsigned char Usart1EndOfTx (void);


void Usart2Config(void);
void Usart2SendSingle (unsigned char);
void Usart2Send (char *);
void Usart2SendUnsigned(unsigned char *, unsigned char);
unsigned char Usart2SendVerifyEmpty (void);
unsigned char Usart2ReadBuffer (unsigned char *, unsigned short);
void USART2_IRQHandler(void);
unsigned char Usart2HaveData (void);
void Usart2HaveDataReset (void);

#endif    /* _USART_H_ */

//--- end of file ---//
