//---------------------------------------------
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    TEST PLATFORM FOR FIRMWARE
// ##
// #### TESTS_MOCK_USART.C ####################
//---------------------------------------------

// Includes Modules for tests --------------------------------------------------
#include "tests_mock_usart.h"
#include "tests_ok.h"

#include <stdio.h>
#include <string.h>


// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------
// Mock the Usart1
unsigned char usart1_have_data = 0;
char usart1_msg_received [200] = { 0 };
char usart1_msg_sended [200] = { 0 };

void (*pU1Cb) (char *);

// Mock the Usart2
unsigned char usart2_have_data = 0;
char usart2_msg_received [200] = { 0 };
char usart2_msg_sended [200] = { 0 };

void (*pU2Cb) (char *);


// Module Functions ------------------------------------------------------------
/////////////////////////////
// Usart1 Mocked Functions //
/////////////////////////////
void Usart1Send (char * msg)
{
    strcpy(usart1_msg_sended, msg);
    PrintYellow("usart1 tx -> ");
    printf("%s\n", msg);    
}


unsigned char Usart1ReadBuffer (unsigned char * bout, unsigned short max_len)
{
    unsigned char len = 0;
    len = strlen(usart1_msg_received);
    if (max_len > len)
        strcpy(bout, usart1_msg_received);
    else
        printf("error on Usart1ReadBuffer max_len\n");

    return len;
}


void Usart1FillRxBuffer (char * msg)
{
    strcpy(usart1_msg_received, msg);
    PrintYellow("usart1 rx <- ");
    printf("%s\n", msg);
    usart1_have_data = 1;
}


void Usart1Callback (void (* pCb) (char *))
{
    pU1Cb = pCb;
}


unsigned char Usart1HaveData (void)
{
    return usart1_have_data;
}


void Usart1HaveDataReset (void)
{
    usart1_have_data = 0;
}


/////////////////////////////
// Usarts Mocked Functions //
/////////////////////////////
void Usart2Send (char * msg)
{
    strcpy(usart2_msg_sended, msg);
    PrintBoldWhite("usart2 tx -> ");
    printf("%s\n", msg);
    if (pU2Cb != NULL)
        pU2Cb(msg);
}


void Usart2Callback (void (* pCb) (char *))
{
    pU2Cb = pCb;
}


unsigned char Usart2ReadBuffer (unsigned char * bout, unsigned short max_len)
{
    unsigned char len = 0;
    len = strlen(usart2_msg_received);
    if (max_len > len)
        strcpy(bout, usart2_msg_received);
    else
        printf("error on Usart2ReadBuffer max_len\n");

    return len;
}


void Usart2FillRxBuffer (char * msg)
{
    strcpy(usart2_msg_received, msg);
    PrintYellow("usart2 rx <- ");
    printf("%s\n", msg);
    usart2_have_data = 1;
}


unsigned char Usart2HaveData (void)
{
    return usart2_have_data;
}


void Usart2HaveDataReset (void)
{
    usart2_have_data = 0;
}


//--- end of file ---//


