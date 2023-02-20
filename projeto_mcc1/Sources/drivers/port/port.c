/***************************************************************************************
 * M�dulo      : port.c
 * Revis�o     : 1.0
 * Data        : 16/02/2023
 * Descri��o   : Arquivo com implementa��es dos M�dulos PORT da Familia Kinetis KL05.
 * Coment�rios : Nenhum.
 * Autor(es)   : Matheus Leitzke Pinto
 ***************************************************************************************/

/*ARQUIVOS DE CABE�ALHO*/
/*=======================================================================================*/

#include "port.h"

/*FIM: ARQUIVOS DE CABE�ALHO*/
/*=======================================================================================*/

/*FUN��ES P�BLICAS*/
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
void PORT_IRQEnable(PORT_Type *base, uint8_t pin, portIRQ_t irq)
{
	assert(base);
	assert( ( pin >= 0 ) && ( pin < 32 ) );

	base->PCR[pin] &= ~PORT_PCR_IRQC_MASK; /*Limpa configura��es anteriores*/
	base->PCR[pin] |= PORT_PCR_IRQC(irq);
}

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
uint32_t PORT_GetIRQFlag(PORT_Type *base, uint8_t pin)
{
	assert(base);
	assert( ( pin >= 0 ) && ( pin < 32 ) );

	return base->ISFR & ((uint32_t)1 << pin);
}

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
void PORT_ClearIRQFlag(PORT_Type *base, uint8_t pin)
{
	assert(base);
	assert( ( pin >= 0 ) && ( pin < 32 ) );

	base->ISFR = ( (uint32_t) 1 << pin );
}


/*FIM: FUN��ES P�BLICAS*/
/*=======================================================================================*/

/***************************************************************************************
 * FIM: M�dulo - port.c
 ***************************************************************************************/
