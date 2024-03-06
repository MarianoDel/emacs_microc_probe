//---------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    STM32G030
// ##
// #### DMA.H #################################
//---------------------------------------------

#ifndef _DMA_H_
#define _DMA_H_

// Module Exported Types Constants and Macros ----------------------------------
#define sequence_ready         (DMA1->ISR & DMA_ISR_TCIF1)
// #define sequence_ready_reset   (DMA1->IFCR = DMA_ISR_TCIF1)
#define sequence_ready_reset   (DMA1->IFCR |= DMA_ISR_TCIF1)

// -- Defines for configuration
// #define DMA_WITH_INTERRUPT


// Module Exported Functions ---------------------------------------------------
void DMAConfig(void);
unsigned char DMASequenceReadyGet (void);
void DMASequenceReadyReset (void);


#ifdef DMA_WITH_INTERRUPT
void DMAEnableInterrupt (void);
void DMADisableInterrupt (void);
void DMA1_Channel1_IRQHandler (void);
#endif


#endif /* _DMA_H_ */
