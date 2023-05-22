/***************************************************************************************
 * M�dulo      : uart.h
 *
 * Revis�o     : 1.0
 *
 * Data        : 16/02/2023
 *
 * Descri��o   : Arquivo com implementa��es dos M�dulos UART da Familia Kinetis KL05.
 *
 * Coment�rios : Nenhum.
 *
 * Autor(es)   : Matheus Leitzke Pinto
 ***************************************************************************************/

#ifndef UART_DRV_H_
#define UART_DRV_H_

/*ARQUIVOS DE CABE�ALHO*/
/*=======================================================================================*/

#include "drivers/common_driver.h"

/*FIM: ARQUIVOS DE CABE�ALHO*/
/*=======================================================================================*/


/*TIPOS ENUMERADOS*/
/*=======================================================================================*/

/*Poss�veis combina��es para habilitar Tx e Rx*/
typedef enum{
	UART_TX_RX_DISABLE = 0U, 					/* Desabilita Tx e Rx*/
	UART_TX_ENABLE = UART0_C2_TE_MASK,			/* Habilita Tx*/
	UART_RX_ENABLE = UART0_C2_RE_MASK,			/* Habilita Rx*/
	UART_TX_RX_ENABLE =
			UART0_C2_TE_MASK | UART0_C2_RE_MASK	/* Habilita Tx e Rx*/
}uartTxRxEnabling_t;

/*Poss�veis m�todos de paridade*/
typedef enum{
	UART_NO_PARITY = 0U, 						 /* Desabilita Tx e Rx*/
	UART_PARITY_EVEN =
			UART0_C1_PE_MASK | UART0_C2_TE_MASK, /* Habilita Tx*/
	UART_PARITY_ODD =
			UART0_C1_PE_MASK | UART0_C2_RE_MASK, /* Habilita Rx*/
}uartParity_t;

/*Poss�veis fontes de clock para o m�dulo UART*/
typedef enum{
	UART_CLOCK_DISABLE = 0U,
	UART_CLOCK_FLL = 1U,
	UART_CLOCK_OSC = 2U,
	UART_CLOCK_IRC = 3U
}uartClkSrc_t;

/*FIM: TIPOS ENUMERADOS*/
/*=======================================================================================*/


/*PROTOTIPOS - FUN��ES INLINE*/
/*=======================================================================================*/

static inline uint8_t UART_IsRxAvailable(UART0_Type * base);
static inline uint8_t UART_IsTxAvailable(UART0_Type * base);
static inline void UART_Write(UART0_Type * base, uint8_t data);
static inline uint8_t UART_Read(UART0_Type * base);
static inline void UART_EnableRxIRQ(UART0_Type * base);
static inline void UART_DisableTxIRQ(UART0_Type * base);
static inline void UART_CleanRxBuffer(UART0_Type * base);
static inline uint8_t UART_IsRxOverrun(UART0_Type * base);
static inline uint8_t UART_IsFramingError(UART0_Type * base);
static inline uint8_t UART_IsParityError(UART0_Type * base);

/*FIM: PROTOTIPOS - FUN��ES INLINE*/
/*=======================================================================================*/


/*PROTOTIPOS - FUN��ES P�BLICAS*/
/*=======================================================================================*/

/**********************************************************************
 * Fun��o		:	UART_SetClkSrc
 *
 * Descri��o	:   Define a fonte de clock utilizada pelo UART.
 *
 * Entradas		:   *base - registrador base do perif�rico UART;
 * 					src   - fonte do clock definido em tipo
 * 					        enumerado.
 *
 * Sa�das		: GENERAL_STATUS_SUCCESSSS, se configura��o ocorreu corretamente;
 *                  GENERAL_STATUS_FAIL, caso contr�rio.
 *
 * Coment�rios 	: 	Deve ser chamada antes da fun��o UART_Init.
 * ********************************************************************/
uint8_t UART_SetClkSrc(UART0_Type * base, uartClkSrc_t src);

/**********************************************************************
 * Fun��o		:	UART_Init
 *
 * Descri��o	:   Inicializa o pino do m�dulo UART.
 *
 * Entradas		:   *base - registrador base do perif�rico UART;
 * 					baudrate - o baudarate;
 * 					enTxRx - Desabilita ou habilita Tx, Rx ou ambos;
 * 					parity - paridade par ou �mpar;
 * 					stopBitsN - n�mero de bits de parada (1U ou 2U).
 *
 * Sa�das		:   Nenhuma.
 *
 * Coment�rios 	: 	Deve ser chamada ap�s a fun��o UART_SetClkSrc.
 * ********************************************************************/
void UART_Init( UART0_Type * base,
		        uint16_t baudRate,
				uartTxRxEnabling_t enTxRx,
				uartParity_t parity,
				uint8_t stopBitsN);

/*FIM: PROTOTIPOS - FUN��ES P�BLICAS*/
/*=======================================================================================*/


/*FUN��ES INLINE*/
/*=======================================================================================*/

/**********************************************************************
 * Fun��o		:	UART_IsRxOverrun
 *
 * Descri��o	:   Verifica se houve erro de Overrun na recep��o dos dados.
 *
 * Entradas		:   *base - registrador base do perif�rico UART.
 *
 * Sa�das		:   0 : n�o houve Overrun;
 * 					1: houve Overrun.
 *
 * Coment�rios 	: 	Nenhum.
 * ********************************************************************/
static inline uint8_t UART_IsRxOverrun(UART0_Type * base)
{
	return (uint8_t)((base->S1 & UART0_S1_OR_MASK) != 0);
}

/**********************************************************************
 * Fun��o		:	UART_IsFramingError
 *
 * Descri��o	:   Verifica se houve erro de Framing na recep��o dos dados.
 *
 * Entradas		:   *base - registrador base do perif�rico UART.
 *
 * Sa�das		:   0 : n�o houve erro;
 * 					1 : houve erro.
 *
 * Coment�rios 	: 	Nenhum.
 * ********************************************************************/
static inline uint8_t UART_IsFramingError(UART0_Type * base)
{
	return (uint8_t)((UART0_S1 & UART0_S1_FE_MASK) != 0);
}

/**********************************************************************
 * Fun��o		:	UART_IsParityError
 *
 * Descri��o	:   Verifica se houve erro de paridade na recep��o dos dados.
 *
 * Entradas		:   *base - registrador base do perif�rico UART.
 *
 * Sa�das		:   0 : n�o houve erro;
 * 					1 : houve erro.
 *
 * Coment�rios 	: 	Nenhum.
 * ********************************************************************/
static inline uint8_t UART_IsParityError(UART0_Type * base)
{
	return (uint8_t)((UART0_S1 & UART0_S1_PF_MASK) != 0);
}

/**********************************************************************
 * Fun��o		:	UART_CleanRxBuffer
 *
 * Descri��o	:   Limpa flags de erro e buffer de recep��o.
 *
 * Entradas		:   *base - registrador base do perif�rico UART.
 *
 * Sa�das		:   Nenhuma.
 *
 * Coment�rios 	: 	Nenhum.
 * ********************************************************************/
static inline void UART_CleanRxBuffer(UART0_Type * base)
{
	UART_IsRxAvailable(base);  /*L� o registrador UART_S1*/
	UART_Read(base);
}

/**********************************************************************
 * Fun��o		:	UART_Deinit
 *
 * Descri��o	:   Desabilita o m�dulo UART.
 *
 * Entradas		:   *base - registrador base do perif�rico UART.
 *
 * Sa�das		:   Nenhuma.
 *
 * Coment�rios 	: 	Nenhum.
 * ********************************************************************/
static inline void UART_Deinit(UART0_Type * base)
{
	SIM_SCGC4 &= ~SIM_SCGC4_UART0_MASK;

	NVIC_DisableIRQ(UART0_IRQn);
}

/**********************************************************************
 * Fun��o		:	UART_IsRxAvailable
 *
 * Descri��o	:   Verifica se h� dados para serem lidos do buffer Rx.
 *
 * Entradas		:   *base - registrador base do perif�rico UART.
 *
 * Sa�das		:   0: n�o h� dados para serem lidos;
 * 					1: existem dados n�o lidos no buffer.
 *
 * Coment�rios 	: 	Nenhum.
 * ********************************************************************/
static inline uint8_t UART_IsRxAvailable(UART0_Type * base)
{
	return (uint8_t)((UART0_S1 & UART0_S1_RDRF_MASK) != 0);
}

/**********************************************************************
 * Fun��o		:	UART_IsTxAvailable
 *
 * Descri��o	:   Verifica se o buffer Tx est� dispon�vel para escrita.
 *
 * Entradas		:   *base - registrador base do perif�rico UART.
 *
 * Sa�das		:   0: buffer Tx n�o dispon�vel;
 * 					1: buffer Tx dispon�vel para escrita.
 *
 * Coment�rios 	: 	Nenhum.
 * ********************************************************************/
static inline uint8_t UART_IsTxAvailable(UART0_Type * base)
{
	return (uint8_t)((UART0_S1 & UART0_S1_TDRE_MASK) != 0);
}

/**********************************************************************
 * Fun��o		:	UART_Write
 *
 * Descri��o	:   Envia um dado de 8 bits pelo m�dulo UART0.
 *
 * Entradas		:   *base - registrador base do perif�rico UART;
 * 					data  - valor de 8 bits.
 *
 * Sa�das		:   Nenhuma.
 *
 * Coment�rios 	: 	Nenhum.
 * ********************************************************************/
static inline void UART_Write(UART0_Type * base, uint8_t data)
{
	UART0_D = data;
}

/**********************************************************************
 * Fun��o		:	UART_Read
 *
 * Descri��o	:   Retorna um dado de 8 bits pelo m�dulo UART.
 *
 * Entradas		:   *base - registrador base do perif�rico UART.
 *
 * Sa�das		:   valor de 8 bits.
 *
 * Coment�rios 	: 	- Se n�o foi enviado valor novo para o RX, ser� lido o �ltimo
 * 					  valor recebido.
 * 					- Utilizar a fun��o UART0_IsRxAvailable para
 * 					  verificar se foi enviado novo dado.
 * ********************************************************************/
static inline uint8_t UART_Read(UART0_Type * base)
{
	return (uint8_t)UART0_D;
}

/**********************************************************************
 * Fun��o		:	UART_EnableTxIRQ
 *
 * Descri��o	:   Habilita interrup��o de envio de dado completo
 * 					pelo m�dulo UART.
 *
 * Entradas		:   *base - registrador base do perif�rico UART.
 *
 * Sa�das		:   Nenhuma.
 *
 * Coment�rios 	: 	Nenhum.
 * ********************************************************************/
static inline void UART_EnableTxIRQ(UART0_Type * base)
{
	UART0_C2 |= UART0_C2_TIE_MASK;
}

/**********************************************************************
 * Fun��o		:	UART_EnableRxIRQ
 *
 * Descri��o	:   Habilita interrup��o de recep��o de dado completo
 * 					pelo m�dulo UART.
 *
 * Entradas		:   *base - registrador base do perif�rico UART.
 *
 * Sa�das		:   Nenhuma.
 *
 * Coment�rios 	: 	Nenhum.
 * ********************************************************************/
static inline void UART_EnableRxIRQ(UART0_Type * base)
{
	/*Habilita interrup��o de recep��o completa, overrun e framing error*/
	UART0_C2 |= UART0_C2_RIE_MASK;
	UART0_C3 |= UART0_C3_ORIE_MASK | UART0_C3_FEIE_MASK;
}

/**********************************************************************
 * Fun��o		:	UART_DisableTxIRQ
 *
 * Descri��o	:   Desabilita interrup��o de envio de dado completo
 * 					pelo m�dulo UART.
 *
 * Entradas		:   *base - registrador base do perif�rico UART.
 *
 * Sa�das		:   Nenhuma.
 *
 * Coment�rios 	: 	Nenhum.
 * ********************************************************************/
static inline void UART_DisableTxIRQ(UART0_Type * base)
{
	UART0_C2 &= ~UART0_C2_TIE_MASK;
}

/**********************************************************************
 * Fun��o		:	UART_DisableRxIRQ
 *
 * Descri��o	:   Desabilita interrup��o de recep��o de dado completo
 * 					pelo m�dulo UART.
 *
 * Entradas		:   *base - registrador base do perif�rico UART.
 *
 * Sa�das		:   Nenhuma.
 *
 * Coment�rios 	: 	Nenhum.
 * ********************************************************************/
static inline void UART_DisableRxIRQ(UART0_Type * base)
{
	UART0_C2 &= ~UART0_C2_RIE_MASK;
}

/*FIM: FUN��ES INLINE*/
/*=======================================================================================*/

#endif /*UART_DRV_H_*/

/***************************************************************************************
 * FIM: M�dulo - uart.h
 ***************************************************************************************/
