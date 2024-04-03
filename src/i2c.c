//---------------------------------------------
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    STM32F103
// ##
// #### I2C.C #################################
//---------------------------------------------

// Includes --------------------------------------------------------------------
#include "i2c.h"
#include "stm32f10x.h"
#include "hard.h"


// Private Types Constants and Macros ------------------------------------------
// #define I2C_USE_I2C1
// #define I2C_USE_I2C2
// #define I2C_WITH_INTS


#define RCC_I2C1_CLK    (RCC->APB1ENR & 0x00200000)
#define RCC_I2C1_CLKEN    (RCC->APB1ENR |= 0x00200000)
#define RCC_I2C1_CLKDIS    (RCC->APB1ENR &= ~0x00200000)

#define RCC_I2C2_CLK    (RCC->APB1ENR & 0x00400000)
#define RCC_I2C2_CLKEN    (RCC->APB1ENR |= 0x00400000)
#define RCC_I2C2_CLKDIS    (RCC->APB1ENR &= ~0x00400000)

#ifdef I2C_WITH_INTS
typedef enum {
    wait_start,
    wait_addr,
    sending_bytes

} i2c1_int_states_e;

typedef struct {
    unsigned char slave_addr;
    unsigned char * pbuff;
    unsigned short buff_size;
    
} i2c1_int_pckt_st;
#endif

// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------


// Module Private Functions ----------------------------------------------------


// Module Funtions -------------------------------------------------------------
////////////////////
// I2C1 FUNCTIONS //
////////////////////
#ifdef I2C_USE_I2C1
void I2C1_Init (void)
{
    // Clock for Peripheral
    if (!RCC_I2C1_CLK)
        RCC_I2C1_CLKEN;

    // Speed and Port options
    // APB freq = 32MHz peripheral clk = 10MHz
    I2C1->CR2 = 10;
    // I2C1->CR2 = 2;    
    I2C1->CCR |= I2C_CCR_FS | I2C_CCR_DUTY | 0x0004;    //100KHz
    // I2C1->CCR |= I2C_CCR_FS | I2C_CCR_DUTY | 0x0006;    //100KHz    

    // I2C1 remap to PB8 PB9 SCL SDA
    unsigned int temp = 0;
    temp = AFIO->MAPR;
    temp |= AFIO_MAPR_I2C1_REMAP;
    AFIO->MAPR = temp;

    I2C1->CR1 = I2C_CR1_PE;

#ifdef I2C_WITH_INTS
    // I2C1->CR2 |= I2C_CR2_ITEVTEN | I2C_CR2_ITBUFEN;
    I2C1->CR2 |= I2C_CR2_ITEVTEN;    
    // Int and priority
    NVIC_EnableIRQ(I2C1_EV_IRQn);
    NVIC_SetPriority(I2C1_EV_IRQn, 8);
#endif
}


void I2C1_SendByte (unsigned char addr, unsigned char data)
{
    // wait no busy line
    while (I2C1->SR2 & I2C_SR2_BUSY);

    // send START
    I2C1->CR1 |= I2C_CR1_START;
    // wait for START be sent
    while (!(I2C1->SR1 & I2C_SR1_SB));
    I2C1->SR1 &= ~I2C_SR1_AF;    // reset NACK
    // send slave addr
    I2C1->DR = addr;

    // wait for slave addr be sent
    unsigned short dummy = 0;
    unsigned char error = 1;
    do {
        if (I2C1->SR1 & I2C_SR1_ADDR)
        {
            dummy = I2C1->SR2;    //dummy read to clear ADDR
            dummy++;
        }
            
        if ((I2C1->SR1 & I2C_SR1_AF) ||
            (I2C1->SR1 & I2C_SR1_TIMEOUT))
        {
            error = 0;
            I2C1->CR1 |= I2C_CR1_STOP;
            return;
        }
            
    } while (error);


    while (!(I2C1->SR1 & I2C_SR1_TXE));
    I2C1->DR = data;

    // wait for send STOP
    while (!(I2C1->SR1 & I2C_SR1_TXE));
    I2C1->CR1 |= I2C_CR1_STOP;

}


unsigned char I2C1_SendAddr (unsigned char addr)
{
    // wait no busy line
    while (I2C1->SR2 & I2C_SR2_BUSY);

    // send START
    I2C1->CR1 |= I2C_CR1_START;
    // wait for START be sent
    while (!(I2C1->SR1 & I2C_SR1_SB));

    I2C1->SR1 &= ~I2C_SR1_AF;    // reset NACK
    I2C1->DR = addr;

    // wait for slave addr be sent
    unsigned short dummy = 0;
    unsigned char error = 1;
    do {
        if (I2C1->SR1 & I2C_SR1_ADDR)
        {
            dummy = I2C1->SR2;    //dummy read to clear ADDR
            error = 0;
            dummy = 1;
        }
            
        if ((I2C1->SR1 & I2C_SR1_AF) ||
            (I2C1->SR1 & I2C_SR1_TIMEOUT))
        {
            error = 0;
            dummy = 2;
        }
            
    } while (error);
        
    I2C1->CR1 |= I2C_CR1_STOP;
    return (unsigned char) dummy;
}


// no ints
void I2C1_SendMultiByte (unsigned char *pdata, unsigned char addr, unsigned short size)
{
    // wait no busy line
    while (I2C1->SR2 & I2C_SR2_BUSY);
    
    // send START
    I2C1->CR1 |= I2C_CR1_START;
    // wait for START be sent
    while (!(I2C1->SR1 & I2C_SR1_SB));

    I2C1->SR1 &= ~I2C_SR1_AF;    // reset NACK
    // send slave addr is right aligned
    I2C1->DR = (addr << 1) & 0xFE;

    // wait for slave addr be sent
    unsigned short dummy = 0;
    unsigned char error = 1;
    do {
        if (I2C1->SR1 & I2C_SR1_ADDR)
        {
            dummy = I2C1->SR2;    //dummy read to clear ADDR
            dummy++;
            error = 0;
        }
            
        if ((I2C1->SR1 & I2C_SR1_AF) ||
            (I2C1->SR1 & I2C_SR1_TIMEOUT))
        {
            error = 0;
            I2C1->CR1 |= I2C_CR1_STOP;
            return;
        }
            
    } while (error);


    for (int i = 0; i < size; i++)
    {
        while (!(I2C1->SR1 & I2C_SR1_TXE));
        I2C1->DR = *(pdata + i);
    }

    // wait to send STOP
    while (!(I2C1->SR1 & I2C_SR1_TXE));
    I2C1->CR1 |= I2C_CR1_STOP;
    
}


#ifdef I2C_WITH_INTS
i2c1_int_pckt_st i2c1_int_pckt;
volatile unsigned char i2c1_int_active = 0;
// with ints
void I2C1_SendMultiByte_Int (unsigned char addr, unsigned char *pdata, unsigned short size)
{
    if (!(I2C1->SR2 & I2C_SR2_MSL))
    {
        i2c1_int_active = 1;
        i2c1_int_pckt.slave_addr = addr;
        i2c1_int_pckt.pbuff = pdata;
        i2c1_int_pckt.buff_size = size;
        // send START
        I2C1->CR1 |= I2C_CR1_START;
    }
}


unsigned char I2C1_CheckEnded_Int (void)
{
    if ((I2C1->SR2 & I2C_SR2_MSL) ||
        (i2c1_int_active))
        return 0;
    else
        return 1;
}


i2c1_int_states_e i2c1_int_state = wait_start;
volatile unsigned short i2c1_pdata_cnt = 0;
void I2C1_EV_IRQHandler (void)
{
    unsigned char error_send_stop = 0;
    
    switch (i2c1_int_state)
    {
    case wait_start:
        if (I2C1->SR1 & I2C_SR1_SB)
        {
            // send addr
            I2C1->DR = (i2c1_int_pckt.slave_addr << 1) & 0xFE;
            I2C1->SR1 &= ~I2C_SR1_AF;    // reset NACK
            
            i2c1_pdata_cnt = 0;
            i2c1_int_state++;
        }    // else discard silently
        break;

    case wait_addr:
        if (I2C1->SR1 & I2C_SR1_ADDR)
        {
            unsigned short dummy = I2C1->SR2;    //dummy read to clear ADDR
            dummy += 1;
            if (I2C1->SR1 & I2C_SR1_TXE)
            {
                I2C1->DR = *(i2c1_int_pckt.pbuff + i2c1_pdata_cnt);
                i2c1_pdata_cnt++;
                i2c1_int_state++;
            }
            else    // protocol error, send stop
                error_send_stop = 1;

        }
        else    // protocol error, send stop
            error_send_stop = 1;
        
        break;

    case sending_bytes:
        if (I2C1->SR1 & I2C_SR1_TXE)
        {
            if (i2c1_pdata_cnt < i2c1_int_pckt.buff_size)
            {
                I2C1->DR = *(i2c1_int_pckt.pbuff + i2c1_pdata_cnt);
                i2c1_pdata_cnt++;
            }
            else    // transmittion end, send stop
                error_send_stop = 1;
            
        }
        else    // protocol error, send stop
            error_send_stop = 1;
        
        break;

    }

    if (error_send_stop)
    {
        I2C1->CR1 |= I2C_CR1_STOP;
        i2c1_int_state = wait_start;
        i2c1_int_active = 0;
    }
}
#endif    //I2C_WITH_INTS

#endif    //I2C_USE_I2C1

////////////////////
// I2C2 FUNCTIONS //
////////////////////
#ifdef I2C_USE_I2C2
void I2C2_Init (void)
{
    // Clock for Peripheral
    if (!RCC_I2C2_CLK)
        RCC_I2C2_CLKEN;

    // Clock switch HSI to SYSCLK
    // RCC->CFGR3 |= RCC_CFGR3_I2C2SW;

    // Speed and Port options
    I2C2->TIMINGR = 0x00201D2B;    //for HSI
    I2C2->CR1 = I2C_CR1_PE;
    I2C2->CR2 = I2C_CR2_AUTOEND |
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
        else
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
#endif    //I2C_USE_I2C2

//--- end of file ---//
