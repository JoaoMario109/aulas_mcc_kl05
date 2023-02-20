/***************************************************************************************
 * M�dulo      : gpio.c
 * Revis�o     : 1.0
 * Data        : 16/02/2023
 * Descri��o   : Arquivo com implementa��es dos M�dulos GPIO da Familia Kinetis KL05.
 * Coment�rios : Nenhum.
 * Autor(es)   : Matheus Leitzke Pinto
 ***************************************************************************************/

/*ARQUIVOS DE CABE�ALHO*/
/*=======================================================================================*/

#include "gpio.h"

/*FIM: ARQUIVOS DE CABE�ALHO*/
/*=======================================================================================*/

/*FUN��ES PRIVADAS*/
/*=======================================================================================*/

/**********************************************************************
 * Fun��o		:	SetPortAsGPIO
 *
 * Descri��o	:   Realiza a multiplexa��o do pino do PORT como GPIO.
 *
 * Entradas		:   *base - registrador base do perif�rico GPIO;
 *                  pin   - numera��o do pino.
 *
 * Sa�das		:  	Nenhuma.
 *
 * Coment�rios 	: 	Nenhum.
 * ********************************************************************/
static void SetPortAsGPIO(GPIO_Type *base, uint8_t pin)
{
	if ( base == GPIOA )
	{
		PORT_SetMux(PORTA, pin, PORT_MUX_ALT1);
	}
	else
	{
		if ( base == GPIOB )
		{
			PORT_SetMux(PORTB, pin, PORT_MUX_ALT1);
		}
	}
}

/*FIM - FUN��ES PRIVADAS*/
/*=======================================================================================*/

/*FUN��ES P�BLICAS*/
/*=======================================================================================*/

/**********************************************************************
 * Fun��o		:	GPIO_InitInputPin
 *
 * Descri��o	:   Inicializa pino de m�dulo GPIO como entrada.
 *
 * Entradas		:   *base - registrador base do perif�rico GPIO;
 *                  pin   - numera��o do pino.
 *
 * Sa�das		:  	Nenhuma.
 *
 * Coment�rios 	: 	Essa fun��o tamb�m realiza a multiplexa��o do
 *                  do pino do PORT relacionado ao GPIO.
 * ********************************************************************/
void GPIO_InitInputPin(GPIO_Type *base, uint8_t pin)
{
	SetPortAsGPIO(base, pin);
	base->PDDR &= ~(1 << pin);
}

/**********************************************************************
 * Fun��o		:	GPIO_InitOutputPin
 *
 * Descri��o	:   Inicializa pino de m�dulo GPIO como sa�da.
 *
 * Entradas		:   *base - registrador base do perif�rico GPIO;
 *                  pin   - numera��o do pino;
 *                  initVal - valor l�gico inicial do pino (0 ou !0).
 *
 * Sa�das		:  	Nenhuma.
 *
 * Coment�rios 	: 	Essa fun��o tamb�m realiza a multiplexa��o do
 *                  do pino do PORT relacionado ao GPIO.
 * ********************************************************************/
void GPIO_InitOutputPin(GPIO_Type *base, uint8_t pin, uint8_t initVal)
{
	   SetPortAsGPIO(base, pin);
	   base->PDDR |= (1 << pin);
	   if(initVal == 0)
	   {
		   base->PCOR = 1 << pin;
	   }
	   else
	   {
		   base->PSOR = 1 << pin;
	   }
}

/*FIM: FUN��ES P�BLICAS*/
/*=======================================================================================*/

/***************************************************************************************
 * FIM: M�dulo - gpio.c
 ***************************************************************************************/
