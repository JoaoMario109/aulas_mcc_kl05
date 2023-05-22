/***************************************************************************************
 * M�dulo      : uart.c
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


/*ARQUIVOS DE CABE�ALHO*/
/*=======================================================================================*/

#include "uart.h"

/*FIM: ARQUIVOS DE CABE�ALHO*/
/*=======================================================================================*/

/*V�RIAVEIS GLOBAIS PRIVADAS*/
/*=======================================================================================*/

static moduleClock_t g_uartClkSrc;

/*FIM: V�RIAVEIS GLOBAIS PRIVADAS*/
/*=======================================================================================*/


/*FUN��ES P�BLICAS*/
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
uint8_t UART_SetClkSrc(UART0_Type * base, uartClkSrc_t src)
{
	switch(src)
	{
	case UART_CLOCK_DISABLE:
		g_uartClkSrc = 0U;
		break;
	case UART_CLOCK_FLL:
		g_uartClkSrc = MCG_FLL_CLK_FREQ;
		break;
	case UART_CLOCK_IRC:
		g_uartClkSrc = MCG_IRC_CLK_FREQ;
		break;
	case UART_CLOCK_OSC:
		g_uartClkSrc = OSC_ER_CLK_FREQ;
		break;
	default:
		return GENERAL_STATUS_FAIL;
		break;
	}

	SIM->SOPT2 &= ~SIM_SOPT2_UART0SRC_MASK;
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(src);

	return GENERAL_STATUS_SUCCESS;
}

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
				uint8_t stopBitsN)
{
	uint16_t sbr_field;

	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
	/*Limpa configura��es iniciais*/
	base->C1 = 0x00U;
	base->C2 = 0x00U;
	base->C3 = 0x00U;
	base->S2 = 0x00U;

	sbr_field  = g_uartClkSrc/(baudRate*16); 		/*Define o valor do campo SBR*/
	base->BDH |= UART0_BDH_SBR((sbr_field >> 8)); 		/*Coloca os MSB de SBR em UART0_BDH*/
	base->BDL  = UART0_BDL_SBR(sbr_field); 				/*Coloca os LSB de SBR em UART0_BDL*/

	base->C2 |= enTxRx;

	UART_CleanRxBuffer(base); /*Limpa flags de inicializa��o de Rx*/

	base->C1 &= ~UART0_C1_PT_MASK; /*limpa configura��o anterior de paridade*/
	base->C1 |= parity; /*Habilita paridade*/
}

/*FIM: FUN��ES P�BLICAS*/
/*=======================================================================================*/


/***************************************************************************************
 * FIM: M�dulo - uart.c
 ***************************************************************************************/
