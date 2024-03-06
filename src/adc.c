//---------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    STM32G030
// ##
// #### ADC.C #################################
//---------------------------------------------

// Includes --------------------------------------------------------------------
#include "adc.h"
#include "stm32g0xx.h"
#include "hard.h"


// Module Private Types Constants and Macros -----------------------------------
#define RCC_ADC_CLK    (RCC->APBENR2 & 0x00100000)
#define RCC_ADC_CLK_ON    (RCC->APBENR2 |= 0x00100000)
#define RCC_ADC_CLK_OFF    (RCC->APBENR2 &= ~0x00100000)


#define ADC_IT_ADRDY                               ADC_IER_ADRDYIE
#define ADC_IT_EOSMP                               ADC_IER_EOSMPIE
#define ADC_IT_EOC                                 ADC_IER_EOCIE
#define ADC_IT_EOSEQ                               ADC_IER_EOSEQIE
#define ADC_IT_OVR                                 ADC_IER_OVRIE
#define ADC_IT_AWD                                 ADC_IER_AWDIE

#define ADC_Channel_0                              ADC_CHSELR_CHSEL0
#define ADC_Channel_1                              ADC_CHSELR_CHSEL1
#define ADC_Channel_2                              ADC_CHSELR_CHSEL2
#define ADC_Channel_3                              ADC_CHSELR_CHSEL3
#define ADC_Channel_4                              ADC_CHSELR_CHSEL4
#define ADC_Channel_5                              ADC_CHSELR_CHSEL5
#define ADC_Channel_6                              ADC_CHSELR_CHSEL6
#define ADC_Channel_7                              ADC_CHSELR_CHSEL7
#define ADC_Channel_8                              ADC_CHSELR_CHSEL8
#define ADC_Channel_9                              ADC_CHSELR_CHSEL9
#define ADC_Channel_10                             ADC_CHSELR_CHSEL10 /*!< Not available for STM32F031 devices */
#define ADC_Channel_11                             ADC_CHSELR_CHSEL11 /*!< Not available for STM32F031 devices */
#define ADC_Channel_12                             ADC_CHSELR_CHSEL12 /*!< Not available for STM32F031 devices */
#define ADC_Channel_13                             ADC_CHSELR_CHSEL13 /*!< Not available for STM32F031 devices */
#define ADC_Channel_14                             ADC_CHSELR_CHSEL14 /*!< Not available for STM32F031 devices */
#define ADC_Channel_15                             ADC_CHSELR_CHSEL15 /*!< Not available for STM32F031 devices */
#define ADC_Channel_16                             ADC_CHSELR_CHSEL16
#define ADC_Channel_17                             ADC_CHSELR_CHSEL17
#define ADC_Channel_18                             ADC_CHSELR_CHSEL18 /*!< Not available for STM32F030 devices */

#define ADC_Channel_TempSensor                     ((uint32_t)ADC_Channel_16)
#define ADC_Channel_Vrefint                        ((uint32_t)ADC_Channel_17)
#define ADC_Channel_Vbat                           ((uint32_t)ADC_Channel_18) /*!< Not available for STM32F030 devices */

#define ADC_Resolution_12b                         ((uint32_t)0x00000000)
#define ADC_Resolution_10b                         ADC_CFGR1_RES_0
#define ADC_Resolution_8b                          ADC_CFGR1_RES_1
#define ADC_Resolution_6b                          ADC_CFGR1_RES

#define ADC_ClockMode_AsynClk                  ((uint32_t)0x00000000)   /*!< ADC Asynchronous clock mode */
#define ADC_ClockMode_SynClkDiv2               ADC_CFGR2_CKMODE_0   /*!<  Synchronous clock mode divided by 2 */
#define ADC_ClockMode_SynClkDiv4               ADC_CFGR2_CKMODE_1   /*!<  Synchronous clock mode divided by 4 */
#define ADC_ClockMode_SynClkDiv1    (ADC_CFGR2_CKMODE_1 | ADC_CFGR2_CKMODE_0)   /*!<  Synchronous clock mode divided by 1 */

#define ADC_ExternalTrigConvEdge_None              ((uint32_t)0x00000000)
#define ADC_ExternalTrigConvEdge_Rising            ADC_CFGR1_EXTEN_0
#define ADC_ExternalTrigConvEdge_Falling           ADC_CFGR1_EXTEN_1
#define ADC_ExternalTrigConvEdge_RisingFalling     ADC_CFGR1_EXTEN

#define ADC_ExternalTrigConv_T1_TRGO               ((uint32_t)0x00000000)
#define ADC_ExternalTrigConv_T1_CC4                ADC_CFGR1_EXTSEL_0

/* TIM2 */
#define ADC_ExternalTrigConv_T2_TRGO               ADC_CFGR1_EXTSEL_1

/* TIM3 */
#define ADC_ExternalTrigConv_T3_TRGO               ((uint32_t)(ADC_CFGR1_EXTSEL_0 | ADC_CFGR1_EXTSEL_1))

/* TIM15 */
#define ADC_ExternalTrigConv_T15_TRGO              ADC_CFGR1_EXTSEL_2

#define ADC_DataAlign_Right                        ((uint32_t)0x00000000)
#define ADC_DataAlign_Left                         ADC_CFGR1_ALIGN

#define ADC_ScanDirection_Upward                   ((uint32_t)0x00000000)
#define ADC_ScanDirection_Backward                 ADC_CFGR1_SCANDIR

#define ADC_DMAMode_OneShot                        ((uint32_t)0x00000000)
#define ADC_DMAMode_Circular                       ADC_CFGR1_DMACFG

#define ADC_AnalogWatchdog_Channel_0                 ((uint32_t)0x00000000)
#define ADC_AnalogWatchdog_Channel_1                 ((uint32_t)0x04000000)
#define ADC_AnalogWatchdog_Channel_2                 ((uint32_t)0x08000000)
#define ADC_AnalogWatchdog_Channel_3                 ((uint32_t)0x0C000000)
#define ADC_AnalogWatchdog_Channel_4                 ((uint32_t)0x10000000)
#define ADC_AnalogWatchdog_Channel_5                 ((uint32_t)0x14000000)
#define ADC_AnalogWatchdog_Channel_6                 ((uint32_t)0x18000000)
#define ADC_AnalogWatchdog_Channel_7                 ((uint32_t)0x1C000000)
#define ADC_AnalogWatchdog_Channel_8                 ((uint32_t)0x20000000)
#define ADC_AnalogWatchdog_Channel_9                 ((uint32_t)0x24000000)
#define ADC_AnalogWatchdog_Channel_10                ((uint32_t)0x28000000) /*!< Not available for STM32F031 devices */
#define ADC_AnalogWatchdog_Channel_11                ((uint32_t)0x2C000000) /*!< Not available for STM32F031 devices */
#define ADC_AnalogWatchdog_Channel_12                ((uint32_t)0x30000000) /*!< Not available for STM32F031 devices */
#define ADC_AnalogWatchdog_Channel_13                ((uint32_t)0x34000000) /*!< Not available for STM32F031 devices */
#define ADC_AnalogWatchdog_Channel_14                ((uint32_t)0x38000000) /*!< Not available for STM32F031 devices */
#define ADC_AnalogWatchdog_Channel_15                ((uint32_t)0x3C000000) /*!< Not available for STM32F031 devices */
#define ADC_AnalogWatchdog_Channel_16                ((uint32_t)0x40000000)
#define ADC_AnalogWatchdog_Channel_17                ((uint32_t)0x44000000)
#define ADC_AnalogWatchdog_Channel_18                ((uint32_t)0x48000000)

#define CALIBRATION_TIMEOUT       ((uint32_t)0x0000F000)


// Externals -------------------------------------------------------------------
extern volatile unsigned short adc_ch [];


#ifdef ADC_WITH_INT
volatile unsigned char adc_int_seq_ready = 0;
#endif


// Globals ---------------------------------------------------------------------
#ifdef ADC_WITH_INT
volatile unsigned short * p_channel;
#endif


// Module Functions ------------------------------------------------------------
//Single conversion mode (CONT=0)
//In Single conversion mode, the ADC performs a single sequence of conversions,
//converting all the channels once.

//Continuous conversion mode (CONT=1)
//In continuous conversion mode, when a software or hardware trigger event occurs,
//the ADC performs a sequence of conversions, converting all the channels once and then
//automatically re-starts and continuously performs the same sequence of conversions

//Discontinuous mode (DISCEN)
//In this mode (DISCEN=1), a hardware or software trigger event is required to start
//each conversion defined in the sequence. Only with (CONT=0)

//TODO: CUIDADO, no funciona con ADCClock >= 16MHz!!!
//TODO: CUIDADO2, en dma no funciona con ADCClock >= 8MHz!!!
void AdcConfig (void)
{
    if (!RCC_ADC_CLK)
        RCC_ADC_CLK_ON;

    // preseteo los registros a default, la mayoria necesita tener ADC apagado
    ADC1->CR = 0x00000000;
    ADC1->IER = 0x00000000;
    ADC1->CFGR1 = 0x00000000;
    ADC1->CFGR2 = 0x00000000;
    ADC1->SMPR = 0x00000000;
    ADC1->CHSELR = 0x00000000;

    //--- Sync clk settings
    //set clock in sync
    // ADC1->CFGR2 = ADC_ClockMode_SynClkDiv4;
    // ADC1->CFGR2 = ADC_ClockMode_SynClkDiv2;
    // ADC1->CFGR2 = ADC_ClockMode_SynClkDiv1;
    // effective freq 64/4 = 16MHz
    //--- end of Sync clk settings    

    //--- Async clk settings
    //set clock async, expect some jitter, change RCC params too
    ADC1->CFGR2 = ADC_ClockMode_AsynClk;

    // Pclk
    // check pllp divider on system clock config & enable it
    RCC->PLLCFGR |= RCC_PLLCFGR_PLLPEN;
    // set ADCSEL to PLLP
    RCC->CCIPR |= RCC_CCIPR_ADCSEL_0;
    // end of Pclk

    // Hsi16
    // set ADCSEL to HSI16
    // RCC->CCIPR |= RCC_CCIPR_ADCSEL_1;
    // end of Hsi16

    // SYSCLK
    // set ADCSEL to SYSCLK
    // RCC->CCIPR = 0x00;
    // end of SYSCLK
    
    //set the pre divider for async clocks
    // ADC1_COMMON->CCR |= ADC_CCR_PRESC_1;    // pres / 4 freq = 8MHz
    ADC1_COMMON->CCR |= ADC_CCR_PRESC_2;    // pres / 8 freq = 4MHz
    // ADC1_COMMON->CCR |= ADC_CCR_PRESC_0;    // pres / 2 freq = 16MHz NO FUNCIONA!!!
    //--- end of Async clk settings
    

    //set resolution, trigger & Continuos or Discontinuous
    //remember do a start ADC1->CR |= ADC_CR_ADSTART
    // ADC1->CFGR1 |= ADC_Resolution_10b | ADC_ExternalTrigConvEdge_Rising | ADC_ExternalTrigConv_T3_TRGO;
    // ADC1->CFGR1 |= ADC_Resolution_10b | ADC_ExternalTrigConvEdge_Rising | ADC_ExternalTrigConv_T1_TRGO;
    // ADC1->CFGR1 |= ADC_Resolution_12b | ADC_CFGR1_DISCEN;
    ADC1->CFGR1 |= ADC_Resolution_12b | ADC_CFGR1_CONT;    
    // ADC1->CFGR1 |= ADC_Resolution_12b;

    //set sampling time
    ADC1->SMPR |= ADC_SMPR_SMP1_2 | ADC_SMPR_SMP1_1 | ADC_SMPR_SMP1_0 |
        ADC_SMPR_SMP2_2 | ADC_SMPR_SMP2_1 | ADC_SMPR_SMP2_0;

    // effective conversion time (sampling time + sar_res) / eff freq
    // eff freq = 16MHz, resolution 12b, sampling 160.5, sar_12bits 12.5
    // (160.5 + 12.5) * 1 / 16MHz = 10.8us
    
    // effective end of sequence (conv time * num_chnls)
    // (10.8 * 2) = 21.6us
    
    //set channel selection
    ADC1->CHSELR |= ADC_All_Orer_Channels;
    
#ifdef ADC_WITH_INT        
    //set interrupts
    ADC1->IER |= ADC_IT_EOC;

    //set pointer
    p_channel = &adc_ch[0];

    NVIC_EnableIRQ(ADC1_IRQn);
    NVIC_SetPriority(ADC1_IRQn, 3);
#endif

#ifdef ADC_WITH_TEMP_SENSE
    ADC->CCR |= ADC_CCR_TSEN;
#endif

    //calibrar ADC
    ADCGetCalibrationFactor();

#ifdef ADC_WITH_DMA
    ADC1->CFGR1 |= ADC_CFGR1_DMAEN | ADC_CFGR1_DMACFG;
#endif
    
    // Enable ADC1
    ADC1->CR |= ADC_CR_ADEN;
}

#ifdef ADC_WITH_INT
void ADC1_IRQHandler (void)
{
    if (ADC1->ISR & ADC_IT_EOC)
    {
#if (ADC_CHANNEL_QUANTITY > 1)
        if (ADC1->ISR & ADC_IT_EOSEQ)
        {
            p_channel = &adc_ch[ADC_LAST_CHANNEL_QUANTITY];
            *p_channel = ADC1->DR;
            p_channel = &adc_ch[0];
            adc_int_seq_ready = 1;            
        }
        else
        {
            *p_channel = ADC1->DR;
            if (p_channel < &adc_ch[ADC_LAST_CHANNEL_QUANTITY])
                p_channel++;
        }
#else
        adc_ch[0] = ADC1->DR;
        adc_int_seq_ready = 1;
#endif
        //clear pending
        ADC1->ISR |= ADC_IT_EOC | ADC_IT_EOSEQ;
    }
}
#endif


//lee el ADC sin cambiar el sample time anterior
unsigned short ReadADC1_SameSampleTime (unsigned int channel)
{
    // Configure the ADC Channel
    ADC1->CHSELR = channel;

    // Start the conversion
    ADC1->CR |= (uint32_t)ADC_CR_ADSTART;

    // Wait until conversion completion
    while((ADC1->ISR & ADC_ISR_EOC) == 0);

    // Get the conversion value
    return (uint16_t) ADC1->DR;
}

unsigned short ReadADC1Check (unsigned char channel)
{
    if (ADC1->CR & 0x01)			//reviso ADEN
        return 0xFFFF;

    //espero que este listo para convertir
    while ((ADC1->ISR & 0x01) == 0);	//espero ARDY = 1

    if ((ADC1->CFGR1 & 0x00010000) == 0)			//reviso DISCONTINUOS = 1
        return 0xFFFF;

    if (ADC1->CFGR1 & 0x00002000)					//reviso CONT = 0
        return 0xFFFF;

    if (ADC1->CFGR1 & 0x00000C00)					//reviso TRIGGER = 00
        return 0xFFFF;

    if (ADC1->CFGR1 & 0x00000020)					//reviso ALIGN = 0
        return 0xFFFF;

    if (ADC1->CFGR1 & 0x00000018)					//reviso RES = 00
        return 0xFFFF;

    //espero que no se este convirtiendo ADCSTART = 0
    while ((ADC1->CR & 0x02) != 0);	//espero ADCSTART = 0

    ADC1->CHSELR = 0x00000001;	//solo convierto CH0

    return 1;
}

unsigned int ADCGetCalibrationFactor (void)
{
    uint32_t tmpreg = 0, calibrationcounter = 0, calibrationstatus = 0;

    /* Set the ADC calibartion */
    ADC1->CR |= (uint32_t)ADC_CR_ADCAL;

    /* Wait until no ADC calibration is completed */
    do
    {
        calibrationstatus = ADC1->CR & ADC_CR_ADCAL;
        calibrationcounter++;
    } while((calibrationcounter != CALIBRATION_TIMEOUT) && (calibrationstatus != 0x00));

    if((uint32_t)(ADC1->CR & ADC_CR_ADCAL) == RESET)
    {
        /*Get the calibration factor from the ADC data register */
        tmpreg = ADC1->DR;
    }
    else
    {
        /* Error factor */
        tmpreg = 0x00000000;
    }
    return tmpreg;
}


//--- end of file ---//


