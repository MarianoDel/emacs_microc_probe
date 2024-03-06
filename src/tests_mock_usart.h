//---------------------------------------------
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    TEST PLATFORM FOR FIRMWARE
// ##
// #### TESTS_MOCK_USART.H ####################
//---------------------------------------------
#ifndef _TESTS_MOCK_USART_H_
#define _TESTS_MOCK_USART_H_


// Module Exported Types Constants and Macros ----------------------------------



// Module Exported Functions ---------------------------------------------------
void Usart1Send (char * msg);
unsigned char Usart1ReadBuffer (unsigned char * bout, unsigned short max_len);
void Usart1FillRxBuffer (char * msg);

void Usart2Send (char * msg);
unsigned char Usart2ReadBuffer (unsigned char * bout, unsigned short max_len);
void Usart2FillRxBuffer (char * msg);
unsigned char Usart2HaveData (void);
void Usart2HaveDataReset (void);

void Usart2Callback (void (* pCb) (char *));

#endif
//--- end of file ---//


