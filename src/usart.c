//----------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    STM32G030
// ##
// #### USART.C ################################
//----------------------------------------------
#include "hard.h"
#include "stm32g0xx.h"
#include "usart.h"

#include <string.h>


// Module Private Types Constants and Macros -----------------------------------
#define USART1_CLK    (RCC->APBENR2 & 0x00004000)
#define USART1_CLK_ON    (RCC->APBENR2 |= 0x00004000)
#define USART1_CLK_OFF    (RCC->APBENR2 &= ~0x00004000)

#define USART2_CLK    (RCC->APBENR1 & 0x00020000)
#define USART2_CLK_ON    (RCC->APBENR1 |= 0x00020000)
#define USART2_CLK_OFF    (RCC->APBENR1 &= ~0x00020000)

// USART with PCKL 32MHz or PCKL 64MHz
#define USART_PCKL32M
// #define USART_PCKL64M

#ifdef USART_PCKL64M
#define USART_64MHz_9600    6666
#define USART_64MHz_115200    555
#define USART_64MHz_250000    256
#endif
#ifdef USART_PCKL32M
#define USART_64MHz_9600    3333
#define USART_64MHz_115200    277
#define USART_64MHz_250000    128
#endif

#define USART_16MHz_9600    1666


// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------
volatile unsigned char usart1_have_data = 0;
volatile unsigned char * ptx1;
volatile unsigned char * ptx1_pckt_index;
volatile unsigned char * prx1;
volatile unsigned char tx1buff[SIZEOF_DATA];
volatile unsigned char rx1buff[SIZEOF_DATA];

volatile unsigned char usart2_have_data = 0;
volatile unsigned char * ptx2;
volatile unsigned char * ptx2_pckt_index;
volatile unsigned char * prx2;
volatile unsigned char tx2buff[SIZEOF_DATA];
volatile unsigned char rx2buff[SIZEOF_DATA];


// Module Private Functions ----------------------------------------------------


// Module Functions ------------------------------------------------------------
// void Usart1RxDisable (void)
// {
// 	USART1->CR1 &= ~USART_CR1_RXNEIE;
// 	USART1->CR1 &= ~USART_CR1_RE;
// }

// void Usart1RxEnable (void)
// {
// 	USART1->CR1 |= USART_CR1_RXNEIE | USART_CR1_RE;
// }



//////////////////////
// USART1 Functions //
//////////////////////
void Usart1Config(void)
{
    if (!USART1_CLK)
        USART1_CLK_ON;

    // Usart1 9600 8N1 fifo disabled oversampled 16
#if (defined CLOCK_FREQ_64_MHZ)
    USART1->BRR = USART_64MHz_9600;
#elif (defined CLOCK_FREQ_16_MHZ)
    // PCKL 16MHz
    USART1->BRR = USART_16MHz_9600;    
#else
#error "No Clock Frequency defined on hard.h"
#endif
    
    USART1->CR2 |= USART_CR2_TXINV;    // tx inverted
    // USART1->CR1 = USART_CR1_RE | USART_CR1_TE | USART_CR1_UE;
    // USART1->CR1 = USART_CR1_RXNEIE | USART_CR1_RE | USART_CR1_UE;	//SIN TX
    USART1->CR1 = USART_CR1_RXNEIE_RXFNEIE | USART_CR1_RE | USART_CR1_TE | USART_CR1_UE;    //Rx int + Tx

    // config pins to alternative
    unsigned int temp;
    temp = GPIOB->MODER;    // 2 bits por pin
    temp &= 0xFFFF0FFF;    // PB6 PB7 alternative
    temp |= 0x0000A000;    
    GPIOB->MODER = temp;
    
    // temp = GPIOA->AFR[1];
    // temp &= 0xFFFFF00F;
    // temp |= 0x00000110;    //PA10 -> AF1 PA9 -> AF1
    // GPIOA->AFR[1] = temp;

    ptx1 = tx1buff;
    ptx1_pckt_index = tx1buff;
    prx1 = rx1buff;
    
    NVIC_EnableIRQ(USART1_IRQn);
    NVIC_SetPriority(USART1_IRQn, 5);
}




// void Usart1Enable_PB7_9600 (void)
// {
//     usart1_mode = USART_IN_MANUAL_MODE;
    
//     USART1->CR1 &= ~(USART_CR1_UE);    //disable
//     USART1->BRR = USART_64MHz_9600;
//     USART1->CR2 &= ~(USART_CR2_STOP_1);	//1 bits stop

//     unsigned int temp;
//     temp = GPIOB->MODER;    //2 bits por pin
//     temp &= 0xFFFF3FFF;    //PB7 alternative
//     temp |= 0x00008000;    //
//     GPIOB->MODER = temp;

//     temp = GPIOA->MODER;    //2 bits por pin
//     temp &= 0xFFCFFFFF;    //PA10 input
//     temp |= 0x00000000;    //
//     GPIOA->MODER = temp;

//     USART1->CR1 = USART_CR1_RXNEIE_RXFNEIE | USART_CR1_RE | USART_CR1_UE;	//no TX
    
// }


void Usart1SendSingle (unsigned char b)
{
    USART1->TDR = b;    
}


void Usart1Send (char * send)
{
    unsigned char i;

    i = strlen(send);
    Usart1SendUnsigned((unsigned char *)send, i);
}


void Usart1SendUnsigned(unsigned char * send, unsigned char size)
{
    if ((ptx1_pckt_index + size) < &tx1buff[SIZEOF_DATA])
    {
        memcpy((unsigned char *)ptx1_pckt_index, send, size);
        ptx1_pckt_index += size;
        USART1->CR1 |= USART_CR1_TXEIE_TXFNFIE;
    }
}


unsigned char Usart1ReadBuffer (unsigned char * bout, unsigned short max_len)
{
    unsigned int len;

    len = prx1 - rx1buff;

    if (len < max_len)
    {
        *prx1 = '\0';    //buffer from int isnt ended with '\0' do it now
        len += 1;    //space for '\0'
    }
    else
    {
        *(bout + max_len - 1) = '\0';
        len = max_len - 1;
    }

    memcpy(bout, (unsigned char *) rx1buff, len);

    //pointer adjust after copy
    prx1 = rx1buff;
    return (unsigned char) len;
}


void USART1_IRQHandler(void)
{
    unsigned char dummy;

    // USART in Rx mode --------------------------------------------------
    if (USART1->ISR & USART_ISR_RXNE_RXFNE)
    {
        dummy = USART1->RDR & 0x0FF;

        if (prx1 < &rx1buff[SIZEOF_DATA - 1])
        {
            if (dummy == '\n')
            {
                *prx1 = '\0';
                usart1_have_data = 1;
            }
            else
            {
                *prx1 = dummy;
                prx1++;
            }
        }
        else
            prx1 = rx1buff;    // fixes blocked with garbage problem
    }

    // USART in Tx mode --------------------------------------------------
    if (USART1->CR1 & USART_CR1_TXEIE_TXFNFIE)
    {
        if (USART1->ISR & USART_ISR_TXE_TXFNF)
        {
            if ((ptx1 < &tx1buff[SIZEOF_DATA]) && (ptx1 < ptx1_pckt_index))
            {
                USART1->TDR = *ptx1;
                ptx1++;
            }
            else
            {
                ptx1 = tx1buff;
                ptx1_pckt_index = tx1buff;
                USART1->CR1 &= ~USART_CR1_TXEIE_TXFNFIE;
            }
        }
    }

    if ((USART1->ISR & USART_ISR_ORE) || (USART1->ISR & USART_ISR_NE) || (USART1->ISR & USART_ISR_FE))
    {
        USART1->ICR |= 0x0e;
        dummy = USART1->RDR;
    }
}


unsigned char Usart1HaveData (void)
{
    return usart1_have_data;
}


void Usart1HaveDataReset (void)
{
    usart1_have_data = 0;
}


void Usart1RxDisable (void)
{
    USART1->CR1 &= ~USART_CR1_RXNEIE_RXFNEIE;
    USART1->CR1 &= ~USART_CR1_RE;
}


void Usart1RxEnable (void)
{
    USART1->CR1 |= USART_CR1_RXNEIE_RXFNEIE | USART_CR1_RE;
}


unsigned char Usart1EndOfTx (void)
{
    return (USART1->ISR & USART_ISR_TC);
}

//////////////////////
// USART2 Functions //
//////////////////////
void Usart2Config(void)
{
    if (!USART2_CLK)
        USART2_CLK_ON;

    // Usart1 9600 8N1 fifo disabled oversampled 16
#ifdef CLOCK_FREQ_16_MHZ    // PCKL 16MHz
    USART2->BRR = USART_16MHz_9600;
#endif
#ifdef CLOCK_FREQ_64_MHZ    // PCKL 64MHz
    USART2->BRR = USART_64MHz_9600;
#endif
    // USART1->CR2 |= USART_CR2_STOP_1;	//2 bits stop
    // USART1->CR1 = USART_CR1_RE | USART_CR1_TE | USART_CR1_UE;
    // USART1->CR1 = USART_CR1_RXNEIE | USART_CR1_RE | USART_CR1_UE;	//SIN TX
    // USART2->CR1 = USART_CR1_TE | USART_CR1_UE;    //Solo Tx    
    USART2->CR1 = USART_CR1_RXNEIE_RXFNEIE | USART_CR1_RE | USART_CR1_TE | USART_CR1_UE;    //Rx int + Tx

    unsigned int temp;
    temp = GPIOA->AFR[0];
    temp &= 0xFFFF00FF;
    temp |= 0x00001100;    //PA3 -> AF1 PA2 -> AF1
    GPIOA->AFR[0] = temp;

    ptx2 = tx2buff;
    ptx2_pckt_index = tx2buff;
    prx2 = rx2buff;
    
    NVIC_EnableIRQ(USART2_IRQn);
    NVIC_SetPriority(USART2_IRQn, 6);
}


void Usart2SendSingle (unsigned char b)
{
    USART2->TDR = b;    
}


void Usart2Send (char * send)
{
    unsigned char i;

    i = strlen(send);
    Usart2SendUnsigned((unsigned char *)send, i);
}


unsigned char Usart2SendVerifyEmpty (void)
{
    if (ptx2_pckt_index == tx2buff)
        return 1;

    return 0;
}


void Usart2SendUnsigned(unsigned char * send, unsigned char size)
{
    if ((ptx2_pckt_index + size) < &tx2buff[SIZEOF_DATA])
    {
        memcpy((unsigned char *)ptx2_pckt_index, send, size);
        ptx2_pckt_index += size;
        USART2->CR1 |= USART_CR1_TXEIE_TXFNFIE;
    }
}


unsigned char Usart2ReadBuffer (unsigned char * bout, unsigned short max_len)
{
    unsigned int len;

    len = prx2 - rx2buff;

    if (len < max_len)
        len += 1;    //space for '\0' from int
    else
        len = max_len;

    memcpy(bout, (unsigned char *) rx2buff, len);

    //pointer adjust after copy
    prx2 = rx2buff;
    return (unsigned char) len;
}


void USART2_IRQHandler(void)
{
    unsigned char dummy;

    // USART in Rx mode --------------------------------------------------
    if (USART2->ISR & USART_ISR_RXNE_RXFNE)
    {
        dummy = USART2->RDR & 0x0FF;

        if (prx2 < &rx2buff[SIZEOF_DATA - 1])
        {
            if (dummy == '\r')
            {
            }
            else if ((dummy == '\n') || (dummy == 26))    //CTRL+J ("\r\n"); CTRL-Z (26)
            {
                *prx2 = '\0';
                usart2_have_data = 1;
            }
            else
            {
                *prx2 = dummy;
                prx2++;
            }
        }
        else
            prx2 = rx2buff;    //soluciona problema bloqueo con garbage

    }

    // USART in Tx mode -------------------------------------------------
    if (USART2->CR1 & USART_CR1_TXEIE_TXFNFIE)
    {
        if (USART2->ISR & USART_ISR_TXE_TXFNF)
        {
            if ((ptx2 < &tx2buff[SIZEOF_DATA]) && (ptx2 < ptx2_pckt_index))
            {
                USART2->TDR = *ptx2;
                ptx2++;
            }
            else
            {
                ptx2 = tx2buff;
                ptx2_pckt_index = tx2buff;
                USART2->CR1 &= ~USART_CR1_TXEIE_TXFNFIE;
            }
        }
    }

    if ((USART2->ISR & USART_ISR_ORE) || (USART2->ISR & USART_ISR_NE) || (USART2->ISR & USART_ISR_FE))
    {
        USART2->ICR |= 0x0e;
        dummy = USART2->RDR;
    }
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
