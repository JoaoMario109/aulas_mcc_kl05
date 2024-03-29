/***************************************************************************************
 * M�dulo      : port.h
 * Revis�o     : 1.0
 * Data        : 09/02/2023
 * Descri��o   : Arquivo com implementa��es dos M�dulos PORT da Familia Kinetis KL05.
 * Coment�rios : Nenhum.
 * Autor(es)   : Matheus Leitzke Pinto
 ***************************************************************************************/

#ifndef PORT_DRV_H_
#define PORT_DRV_H_

/*ARQUIVOS DE CABE�ALHO*/
/*=======================================================================================*/

#include "drivers/common_driver.h"

/*FIM: ARQUIVOS DE CABE�ALHO*/
/*=======================================================================================*/

/*TIPOS ENUMERADOS*/
/*=======================================================================================*/

/*Poss�veis alternativas de multiplexa��o dos pinos das PORTs*/
typedef enum{
	PORT_MUX_ALT0 		= 0x0u,
	PORT_MUX_ALT1 		= 0x1u,
	PORT_MUX_ALT2 		= 0x2u,
	PORT_MUX_ALT3 		= 0x3u,
	PORT_MUX_DISABLE 	= PORT_MUX_ALT0
}portMux_t;

/*Poss�veis alternativas de interrup��o dos pinos das PORTs*/
typedef enum{
	PORT_IRQ_DISABLE 		 	 = 0x0U,
	PORT_IRQ_DMA_ON_RISING_EDGE  = 0x1U,
	PORT_IRQ_DMA_ON_FALLING_EDGE = 0x2U,
	PORT_IRQ_DMA_ON_EITHER_EDGE  = 0x3U,
	PORT_IRQ_ON_lOW_LEVEL		 = 0x8U,
	PORT_IRQ_ON_RISING_EDGE 	 = 0x9U,
	PORT_IRQ_ON_FALLING_EDGE 	 = 0xAU,
	PORT_IRQ_ON_EITHER_EDGE 	 = 0xBU,
	PORT_IRQ_ON_HIGH_LEVEL 	 	 = 0xCU
}portIRQ_t;

/*FIM: TIPOS ENUMERADOS*/
/*=======================================================================================*/

/*PROTOTIPOS - FUN��ES P�BLICAS*/
/*=======================================================================================*/

/**********************************************************************
 * Fun��o		:	PORT_IRQEnable
 *
 * Descri��o	:   Habilita a gera��o de interrup��o pelo pino de
 * 					um m�dulo PORT.
 *
 * Entradas		:   *base - registrador base do perif�rico PORT.
 *                  pin	- numera��o do pino;
 * 					irq - uma das alternativas de interrup��o do pino.
 *
 * Sa�das		:  Nenhuma.
 *
 * Coment�rios 	:  Nenhum.
 * ********************************************************************/
void PORT_IRQEnable(PORT_Type *base, uint8_t pin, portIRQ_t irq);

/**********************************************************************
 * Fun��o		:	PORT_GetIRQFlag
 *
 * Descri��o	:   Retorna se a flag de interrup��o de pino do PORT foi setada.
 *
 * Entradas		:   *base - registrador base do perif�rico PORT.
 *                  pin	- numera��o do pino;
 *
 * Sa�das		:  0 - se flag n�o setada;
 *                 !0 - se flag setada.
 *
 * Coment�rios 	:  Nenhum.
 * ********************************************************************/
uint32_t PORT_GetIRQFlag(PORT_Type *base, uint8_t pin);

/**********************************************************************
 * Fun��o		:	PORT_ClearIRQFlag
 *
 * Descri��o	:   Limpa flag de interrup��o do pino de PORT.
 *
 * Entradas		:   *base - registrador base do perif�rico PORT.
 *                  pin	- numera��o do pino;
 *
 * Sa�das		:  Nenhuma.
 *
 * Coment�rios 	:  Nenhum.
 * ********************************************************************/
void PORT_ClearIRQFlag(PORT_Type *base, uint8_t pin);

/*FIM: PROTOTIPOS - FUN��ES P�BLICAS*/
/*=======================================================================================*/


/*PROTOTIPOS - FUN��ES INLINE*/
/*=======================================================================================*/

static inline void PORT_Init(PORT_Type* base);
static inline void PORT_SetMux(PORT_Type *base, uint8_t pin, portMux_t mux);
static inline void PORT_PullEnable(PORT_Type *base, uint8_t pin);
static inline void PORT_HighStrengthEnable(PORT_Type *base, uint8_t pin);

/*FIM: PROTOTIPOS - FUN��ES INLINE*/
/*=======================================================================================*/

/*FUN��ES INLINE*/
/*=======================================================================================*/

/**********************************************************************
 * Fun��o		:	PORT_Init
 *
 * Descri��o	:   Deve ser chamado antes de utilizar um m�dulo PORT.
 *
 * Entradas		:   *base - registrador base do perif�rico PORT.
 *
 * Sa�das 		:   Nenhuma.
 *
 * Coment�rios 	: 	Nenhum.
 * ********************************************************************/
static inline void PORT_Init(PORT_Type* base)
{
	assert(base);

	if( PORTA == base )
	{
		   SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK; /* Ativa o clock do perif�rico PORTA*/
	}
	else
	{
		if( PORTB == base )
		{
			SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK; /* Ativa o clock do perif�rico PORTB*/
			/*Desabilita NMI em PTB5 e habilita como GPIO*/
			base->PCR[5] &= ~PORT_PCR_MUX_MASK; /*Limpa configura��es anteriores*/
			base->PCR[5] |= PORT_PCR_MUX(PORT_MUX_ALT1);
		}
	}
}
/**********************************************************************
 * Fun��o		:	PORT_PullEnable
 *
 * Descri��o	:   Habilita pullUp no pino do m�dulo PORT.
 *
 * Entradas		:   *base - registrador base do perif�rico PORT.
 *                  pin	  - numera��o do pino;
 *
 * Sa�das		:   Nenhuma.
 *
 * Coment�rios 	: 	Nenhum.
 * ********************************************************************/
static inline void PORT_PullEnable(PORT_Type *base, uint8_t pin)
{
	assert(base);
	assert( ( pin >= 0 ) && ( pin < 32 ) );

	base->PCR[pin] |= PORT_PCR_PE_MASK;
}

/**********************************************************************
 * Fun��o		:	PORT_HighStrengthEnable
 *
 * Descri��o	:   Habilita a intensidade da corrente do pino como alta.
 *
 * Entradas		:   *base - registrador base do perif�rico PORT.
 *                  pin	  - numera��o do pino;
 *
 * Sa�das		:   Nenhuma.
 *
 * Coment�rios 	: 	Nenhum.
 * ********************************************************************/
static inline void PORT_HighStrengthEnable(PORT_Type *base, uint8_t pin)
{
	assert(base);
	assert( ( pin >= 0 ) && ( pin < 32 ) );

	base->PCR[pin] |= PORT_PCR_DSE_MASK;
}

/**********************************************************************
 * Fun��o		:	PORTB_SetMux
 *
 * Descri��o	:   Configura a multiplexa��o no pino do m�dulo PORTB.
 *
 * Entradas		:   *base - registrador base do perif�rico PORT.
 *                  pin	- numera��o do pino;
 * 					mux - uma das alternativas de multiplexa��o do pino.
 *
 * Sa�das		:   Nenhuma.
 *
 * Coment�rios 	: 	Nenhum.
 * ********************************************************************/
static inline void PORT_SetMux(PORT_Type *base, uint8_t pin, portMux_t mux)
{
	assert(base);
	assert( ( pin >= 0 ) && ( pin < 32 ) );

	base->PCR[pin] &= ~PORT_PCR_MUX_MASK; /*Limpa configura��es anteriores*/
	base->PCR[pin] |= PORT_PCR_MUX(mux);
}

/*FIM: FUN��ES INLINE*/
/*=======================================================================================*/

#endif /* PORT_DRV_H_ */

/***************************************************************************************
 * FIM: M�dulo - port.h
 ***************************************************************************************/
