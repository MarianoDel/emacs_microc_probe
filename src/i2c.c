//---------------------------------------------
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    STM32G030
// ##
// #### I2C.C #################################
//---------------------------------------------

// Includes --------------------------------------------------------------------
#include "i2c.h"
#include "stm32g0xx.h"


// Externals -------------------------------------------------------------------

// Globals ---------------------------------------------------------------------

// Module Private Functions ----------------------------------------------------
void I2C1_ChangeNBytes (unsigned char);
void I2C1_ChangeSlaveAddr (unsigned char);
void I2C1_SendLastChunk (unsigned char *, unsigned char);
void I2C1_SendMiddleChunk (unsigned char *, unsigned char);
void I2C2_ChangeNBytes (unsigned char);
void I2C2_ChangeSlaveAddr (unsigned char);
void I2C2_SendLastChunk (unsigned char *, unsigned char);
void I2C2_SendMiddleChunk (unsigned char *, unsigned char);


#define I2C1_AUTOEND    do { I2C1->CR2 &= ~(I2C_CR2_RELOAD);\
                             I2C1->CR2 |= I2C_CR2_AUTOEND;\
                           } while (0)
      
#define I2C1_RELOAD    do { I2C1->CR2 &= ~(I2C_CR2_AUTOEND);\
                            I2C1->CR2 |= I2C_CR2_RELOAD;\
                          } while (0)

#define I2C2_AUTOEND    do { I2C2->CR2 &= ~(I2C_CR2_RELOAD);\
                             I2C2->CR2 |= I2C_CR2_AUTOEND;\
                           } while (0)
      
#define I2C2_RELOAD    do { I2C2->CR2 &= ~(I2C_CR2_AUTOEND);\
                            I2C2->CR2 |= I2C_CR2_RELOAD;\
                          } while (0)


// Module Funtions -------------------------------------------------------------
////////////////////
// I2C1 FUNCTIONS //
////////////////////
void I2C1_Init (void)
{
    // Clock for Peripheral
    if (!RCC_I2C1_CLK)
        RCC_I2C1_CLKEN;

    // Clock switch HSI to SYSCLK
    // RCC->CFGR3 |= RCC_CFGR3_I2C1SW;

    // Speed and Port options
    I2C1->TIMINGR = 0x00201D2B;    //for HSI
    I2C1->CR1 = I2C_CR1_PE;
    I2C1->CR2 = I2C_CR2_AUTOEND |
        (2 << I2C_CR2_NBYTES_Pos) |
        (I2C_ADDRESS_SLV << 1);    //el addr + 1 byte

    // Override Gpios Alternative functions
    // PB8 SCL/D15; PB9 SDA/D14 -> AF4
    unsigned int temp = 0;
    temp = GPIOB->AFR[1];
    temp &= 0xFFFFFF00;
    temp |= 0x00000044;    //PB9 -> AF4 PA8 -> AF4
    GPIOB->AFR[1] = temp;

#ifdef I2C_WITH_INTS
    // Int and priority
    NVIC_EnableIRQ(I2C1_IRQn);
    NVIC_SetPriority(I2C1_IRQn, 8);
#endif
}


void I2C1_SendByteTest (unsigned char data)
{
    //check empty
    if (!(I2C1->CR2 & I2C_CR2_START))
    {
        I2C1_ChangeSlaveAddr(0x3C);
        I2C1_ChangeNBytes(1);
        if ((I2C1->ISR & I2C_ISR_TXE) == I2C_ISR_TXE)
        {
            I2C1->TXDR = data;
            I2C1->CR2 |= I2C_CR2_START;
        }
    }
}


void I2C1_SendByteTest2 (unsigned char data)
{
    //check empty
    if ((I2C1->ISR & I2C_ISR_TXE) == I2C_ISR_TXE)
    {
        I2C1->TXDR = data;
        I2C1->CR2 |= I2C_CR2_START;
    }

    while (!(I2C1->ISR & I2C_ISR_TXE));
    I2C1->TXDR = data - 1;
}


void I2C1_SendByte (unsigned char addr, unsigned char data)
{
    //check empty
    if (!(I2C1->CR2 & I2C_CR2_START))
    {
        I2C1_ChangeNBytes(1);
        I2C1_ChangeSlaveAddr(addr);
        if ((I2C1->ISR & I2C_ISR_TXE) == I2C_ISR_TXE)
        {
            I2C1->TXDR = data;
            I2C1->CR2 |= I2C_CR2_START;
        }
    }
}


void I2C1_ChangeSlaveAddr (unsigned char addr)
{
    unsigned int temp;
    temp = I2C1->CR2;
    temp &= ~(I2C_CR2_SADD_Msk);
    temp |= (addr << 1);
    I2C1->CR2 = temp;
}


void I2C1_ChangeNBytes (unsigned char size)
{
    unsigned int temp;
    temp = I2C1->CR2;
    temp &= ~(I2C_CR2_NBYTES_Msk);
    temp |= (size << I2C_CR2_NBYTES_Pos);
    I2C1->CR2 = temp;
}


void I2C1_SendAddr (unsigned char addr)
{    
    //check START ready
    if (!(I2C1->CR2 & I2C_CR2_START))
    {
        I2C1_ChangeNBytes(0);
        I2C1_ChangeSlaveAddr(addr);

        I2C1->CR2 |= I2C_CR2_START;
    }
}


// Send multiple bytes to a slave address
void I2C1_SendMultiByte (unsigned char *pdata, unsigned char addr, unsigned short size)
{
    //check START ready
    if (!(I2C1->CR2 & I2C_CR2_START))
    {
        if (size > 255)
        {
            unsigned short offset = 0;
            
            I2C1_ChangeNBytes(255);
            I2C1_RELOAD;
            I2C1_ChangeSlaveAddr(addr);
            I2C1->CR2 |= I2C_CR2_START;

            while (size > (offset + 255))
            {
                I2C1_SendMiddleChunk((pdata + offset), 255);
                offset += 255;
                if (size > (offset + 255))
                {
                    I2C1_ChangeNBytes(255);
                    I2C1_RELOAD;
                }
            }

            //envio ultimo chunk fig 310 pag 843
            unsigned char bytes_left = size - offset;
            I2C1_ChangeNBytes(bytes_left);
            I2C1_AUTOEND;
            I2C1_SendLastChunk((pdata + offset), bytes_left);
            // I2C1_SendLastChunk_e((pdata + offset), bytes_left);            
        }
        else
        {
            //fig 309 pag 842
            I2C1_ChangeNBytes(size);
            I2C1_AUTOEND;
            I2C1_ChangeSlaveAddr(addr);
            I2C1->CR2 |= I2C_CR2_START;
            I2C1_SendLastChunk(pdata, (unsigned char) size);
        }
    }
}


void I2C1_SendMiddleChunk (unsigned char *pdata, unsigned char size)
{
    unsigned char i = 0;

    while (i < size)
    {
        if (I2C1->ISR & I2C_ISR_TXIS)
        {
            I2C1->TXDR = *(pdata + i);
            i++;
        }
    }

    while (!(I2C1->ISR & I2C_ISR_TCR));
}


//TODO: chequear tambien NACK
void I2C1_SendLastChunk (unsigned char *pdata, unsigned char size)
{
    unsigned char i = 0;

    //clear the stop
    I2C1->ICR |= I2C_ICR_STOPCF;
    while (i < size)
    {
        if (I2C1->ISR & I2C_ISR_TXIS)
        {
            I2C1->TXDR = *(pdata + i);
            i++;
        }
    }

    //espero final de transmision
    while (!(I2C1->ISR & I2C_ISR_STOPF));
    
    // si termino con AUTOEND no recibo TC
    // while (!(I2C1->ISR & I2C_ISR_TC));
    // i2 = I2C1->CR2;
    // i2 += 1;
}

////////////////////
// I2C2 FUNCTIONS //
////////////////////
void I2C2_Init (void)
{
    // Clock for Peripheral
    if (!RCC_I2C2_CLK)
        RCC_I2C2_CLKEN;

    // clk reset
    I2C2->CR1 = I2C_CR1_SWRST;
    I2C2->CR1 &= ~I2C_CR1_SWRST;
    
    // Clock switch HSI to SYSCLK
    // RCC->CFGR3 |= RCC_CFGR3_I2C2SW;

    // Speed and Port options
    // I2C2->TIMINGR = 0x00201D2B;    //for HSI
    I2C2->TIMINGR = 0x00300F38;    //for 64MHz clock 32MHz in PCLK -> 400KHz fastmode    
    I2C2->CR1 = I2C_CR1_PE;
    I2C2->CR2 = I2C_CR2_AUTOEND |
        (2 << I2C_CR2_NBYTES_Pos) |
        (I2C_ADDRESS_SLV << 1);    //el addr + 1 byte

    // Override Gpios Alternative functions
    // PA12 I2C2_SDA -> AF6; PA11 I2C2_SCL -> AF6
    unsigned int temp = 0;
    temp = GPIOA->AFR[1];
    temp &= 0xFFF00FFF;
    temp |= 0x00066000;    //PA12 -> AF6 PA11 -> AF6
    GPIOA->AFR[1] = temp;


#ifdef I2C_WITH_INTS    
    // Int and priority
    NVIC_EnableIRQ(I2C2_IRQn);
    NVIC_SetPriority(I2C2_IRQn, 8);
#endif
}


void I2C2_SendByteTest (unsigned char data)
{
    //check empty
    if (!(I2C2->CR2 & I2C_CR2_START))
    {
        I2C2_ChangeSlaveAddr(0x3C);
        I2C2_ChangeNBytes(1);
        if ((I2C2->ISR & I2C_ISR_TXE) == I2C_ISR_TXE)
        {
            I2C2->TXDR = data;
            I2C2->CR2 |= I2C_CR2_START;
        }
    }
}


void I2C2_SendByteTest2 (unsigned char data)
{
    //check empty
    if ((I2C2->ISR & I2C_ISR_TXE) == I2C_ISR_TXE)
    {
        I2C2->TXDR = data;
        I2C2->CR2 |= I2C_CR2_START;
    }

    while (!(I2C2->ISR & I2C_ISR_TXE));
    I2C2->TXDR = data - 1;
}


void I2C2_SendByte (unsigned char addr, unsigned char data)
{
    //check empty
    if (!(I2C2->CR2 & I2C_CR2_START))
    {
        I2C2_ChangeNBytes(1);
        I2C2_ChangeSlaveAddr(addr);
        if ((I2C2->ISR & I2C_ISR_TXE) == I2C_ISR_TXE)
        {
            I2C2->TXDR = data;
            I2C2->CR2 |= I2C_CR2_START;
        }
    }
}


void I2C2_ChangeSlaveAddr (unsigned char addr)
{
    unsigned int temp;
    temp = I2C2->CR2;
    temp &= ~(I2C_CR2_SADD_Msk);
    temp |= (addr << 1);
    I2C2->CR2 = temp;
}


void I2C2_ChangeNBytes (unsigned char size)
{
    unsigned int temp;
    temp = I2C2->CR2;
    temp &= ~(I2C_CR2_NBYTES_Msk);
    temp |= (size << I2C_CR2_NBYTES_Pos);
    I2C2->CR2 = temp;
}


void I2C2_SendAddr (unsigned char addr)
{    
    //check START ready
    if (!(I2C2->CR2 & I2C_CR2_START))
    {
        I2C2_ChangeNBytes(0);
        I2C2_ChangeSlaveAddr(addr);

        I2C2->CR2 |= I2C_CR2_START;
    }
}


// Send multiple bytes to a slave address
void I2C2_SendMultiByte (unsigned char *pdata, unsigned char addr, unsigned short size)
{
    //check START ready
    if (!(I2C2->CR2 & I2C_CR2_START))
    {
        if (size > 255)
        {
            unsigned short offset = 0;
            
            I2C2_ChangeNBytes(255);
            I2C2_RELOAD;
            I2C2_ChangeSlaveAddr(addr);
            I2C2->CR2 |= I2C_CR2_START;

            while (size > (offset + 255))
            {
                I2C2_SendMiddleChunk((pdata + offset), 255);
                offset += 255;
                if (size > (offset + 255))
                {
                    I2C2_ChangeNBytes(255);
                    I2C2_RELOAD;
                }
            }

            //envio ultimo chunk fig 310 pag 843
            unsigned char bytes_left = size - offset;
            I2C2_ChangeNBytes(bytes_left);
            I2C2_AUTOEND;
            I2C2_SendLastChunk((pdata + offset), bytes_left);
            // I2C2_SendLastChunk_e((pdata + offset), bytes_left);            
        }
        else if (size)
        {
            //fig 309 pag 842
            I2C2_ChangeNBytes(size);
            I2C2_AUTOEND;
            I2C2_ChangeSlaveAddr(addr);
            I2C2->CR2 |= I2C_CR2_START;
            I2C2_SendLastChunk(pdata, (unsigned char) size);
        }
    }
}


void I2C2_SendMiddleChunk (unsigned char *pdata, unsigned char size)
{
    unsigned char i = 0;

    while (i < size)
    {
        if (I2C2->ISR & I2C_ISR_TXIS)
        {
            I2C2->TXDR = *(pdata + i);
            i++;
        }
    }

    while (!(I2C2->ISR & I2C_ISR_TCR));
}


//TODO: chequear tambien NACK
void I2C2_SendLastChunk (unsigned char *pdata, unsigned char size)
{
    unsigned char i = 0;

    //clear the stop
    I2C2->ICR |= I2C_ICR_STOPCF;
    while (i < size)
    {
        if (I2C2->ISR & I2C_ISR_TXIS)
        {
            I2C2->TXDR = *(pdata + i);
            i++;
        }
    }

    //espero final de transmision
    while (!(I2C2->ISR & I2C_ISR_STOPF));
    
    // si termino con AUTOEND no recibo TC
    // while (!(I2C2->ISR & I2C_ISR_TC));
}

volatile unsigned char i2c2_bytes_left = 0;
volatile unsigned char i2c2_stopped = 0;
volatile unsigned char * p_i2c2_data = 0;

// Send multiple bytes to a slave address
void I2C2_Int_SendMultiByte (unsigned char *pdata, unsigned char addr, unsigned short size)
{
    //check START ready
    if (!(I2C2->CR2 & I2C_CR2_START))
    {
        I2C2_ChangeNBytes(size);
        I2C2_AUTOEND;
        I2C2_ChangeSlaveAddr(addr);

        i2c2_bytes_left = size;
        p_i2c2_data = pdata;
        
        I2C2->CR2 |= I2C_CR2_START;
        I2C2->CR1 |= I2C_CR1_TXIE;

        //clear the STOP flag
        I2C2->ICR |= I2C_ICR_STOPCF;
        i2c2_stopped = 0;

        I2C2->CR1 |= I2C_CR1_STOPIE;
    }
}


unsigned char I2C2_Int_CheckEnded (void)
{
    return i2c2_stopped;
}


void I2C2_IRQHandler (void)
{
    //send next byte
    if (I2C2->ISR & I2C_ISR_TXIS)
    {
        if (i2c2_bytes_left)
        {
            i2c2_bytes_left--;
            I2C2->TXDR = *p_i2c2_data;
            p_i2c2_data++;
        }
        else
        {
            //all bytes sended stop int
            I2C2->CR1 &= ~(I2C_CR1_TXIE);
        }
    }

    if (I2C2->ISR & I2C_ISR_STOPF)
    {
        I2C2->ICR |= I2C_ICR_STOPCF;
        I2C2->CR1 &= ~(I2C_CR1_STOPIE);
        i2c2_stopped = 1;
    }
}

//--- end of file ---//
