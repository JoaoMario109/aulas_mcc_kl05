/***************************************************************************************
 * Módulo      : port.c
 * Revisão     : 1.0
 * Data        : 16/02/2023
 * Descrição   : Arquivo com implementações dos Módulos PORT da Familia Kinetis KL05.
 * Comentários : Nenhum.
 * Autor(es)   : Matheus Leitzke Pinto
 ***************************************************************************************/

/*ARQUIVOS DE CABEÇALHO*/
/*=======================================================================================*/

#include "port.h"

/*FIM: ARQUIVOS DE CABEÇALHO*/
/*=======================================================================================*/

/*FUNÇÕES PÚBLICAS*/
/*=======================================================================================*/

/**********************************************************************
 * Função		:	PORT_IRQEnable
 *
 * Descrição	:   Habilita a geração de interrupção pelo pino de
 * 					um módulo PORT.
 *
 * Entradas		:   *base - registrador base do periférico PORT.
 *                  pin	- numeração do pino;
 * 					irq - uma das alternativas de interrupção do pino.
 *
 * Saídas		:  Nenhuma.
 *
 * Comentários 	:  Nenhum.
 * ********************************************************************/
void PORT_IRQEnable(PORT_Type *base, uint8_t pin, portIRQ_t irq)
{
	assert(base);
	assert( ( pin >= 0 ) && ( pin < 32 ) );

	base->PCR[pin] &= ~PORT_PCR_IRQC_MASK; /*Limpa configurações anteriores*/
	base->PCR[pin] |= PORT_PCR_IRQC(irq);
}

/**********************************************************************
 * Função		:	PORT_GetIRQFlag
 *
 * Descrição	:   Retorna se a flag de interrupção de pino do PORT foi setada.
 *
 * Entradas		:   *base - registrador base do periférico PORT.
 *                  pin	- numeração do pino;
 *
 * Saídas		:  0 - se flag não setada;
 *                 !0 - se flag setada.
 *
 * Comentários 	:  Nenhum.
 * ********************************************************************/
uint32_t PORT_GetIRQFlag(PORT_Type *base, uint8_t pin)
{
	assert(base);
	assert( ( pin >= 0 ) && ( pin < 32 ) );

	return base->ISFR & ((uint32_t)1 << pin);
}

/**********************************************************************
 * Função		:	PORT_ClearIRQFlag
 *
 * Descrição	:   Limpa flag de interrupção do pino de PORT.
 *
 * Entradas		:   *base - registrador base do periférico PORT.
 *                  pin	- numeração do pino;
 *
 * Saídas		:  Nenhuma.
 *
 * Comentários 	:  Nenhum.
 * ********************************************************************/
void PORT_ClearIRQFlag(PORT_Type *base, uint8_t pin)
{
	assert(base);
	assert( ( pin >= 0 ) && ( pin < 32 ) );

	base->ISFR = ( (uint32_t) 1 << pin );
}


/*FIM: FUNÇÕES PÚBLICAS*/
/*=======================================================================================*/

/***************************************************************************************
 * FIM: Módulo - port.c
 ***************************************************************************************/
