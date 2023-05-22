/***************************************************************************************
 * M�dulo      : TPM_HAL.c
 *
 * Revis�o     : 1.0
 *
 * Data        : 26/10/2017
 *
 * Descri��o   : Arquivo com implementa��es dos M�dulos Timer/PWM (TPM) da Familia Kinetis KL25.
 *
 * Coment�rios : Nenhum.
 *
 * Autor(es)   : Matheus Leitzke Pinto
 ***************************************************************************************/

/*ARQUIVOS DE CABE�ALHO*/
/*=======================================================================================*/

#include "tpm.h"

/*FIM: ARQUIVOS DE CABE�ALHO*/
/*=======================================================================================*/

/*VARI�VEIS PRIVADAS*/
/*=======================================================================================*/

static uint16_t matchVal;
static moduleClock_t g_tpmClkSrc;

/*FIM: VARI�VEIS PRIVADAS*/
/*=======================================================================================*/

/*FUN��ES P�BLICAS*/
/*=======================================================================================*/

/**********************************************************************
 * Fun��o		:	TPM_SetCounterClkSrc
 *
 * Descri��o	:   Define a fonte de clock utilizada pelo contador.
 *
 * Entradas		:   *base - registrador base do perif�rico TPM;
 *                  src   - tipo enumerado que define a fonte de clock.
 *
 * Sa�das 		: GENERAL_STATUS_SUCCESSSS, se configura��o ocorreu corretamente;
 *                  GENERAL_STATUS_FAIL, caso contr�rio.
 *
 * Coment�rios 	: 	Deve ser chamada antes da fun��o TPM_Init.
 * ********************************************************************/
uint8_t TPM_SetCounterClkSrc(TPM_Type *base, tpmClkSrc_t src)
{
	switch(src)
	{
	case TPM_CNT_CLOCK_DISABLE:
		g_tpmClkSrc = 0U;
		break;
	case TPM_CNT_CLOCK_FLL:
		g_tpmClkSrc = MCG_FLL_CLK_FREQ;
		break;
	case TPM_CNT_CLOCK_IRC:
		g_tpmClkSrc = MCG_IRC_CLK_FREQ;
		break;
	case TPM_CNT_CLOCK_OSC:
		g_tpmClkSrc = OSC_ER_CLK_FREQ;
		break;
	default:
		return GENERAL_STATUS_FAIL;
		break;
	}

	SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(src);

	return GENERAL_STATUS_SUCCESS;
}

/**********************************************************************
 * Fun��o		:	TPM_Init
 *
 * Descri��o	:   Inicializa��o do driver do m�dulo TPM.
 *
 * Entradas		:   *base    - registrador base do perif�rico TPM;
 *                  modulo   - valor de final de contagem;
 *                  prescale - tipo enumerado com um dos poss�veis
 *                             valores de prescaler.
 *
 * Sa�das 		:   Nenhuma.
 *
 * Coment�rios 	: 	Deve ser chamada ap�s a fun��o TPM_SetCounterClkSrc.
 * ********************************************************************/
void TPM_Init(TPM_Type *base, uint16_t modulo, tpmPrescalerValues_t prescale)
{
	assert(base);

	if (TPM0 == base)
	{
		SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK; /* Ativa o clock do m�dulo TPM0 */
	}
	else
	{
		if (TPM1 == base)
		{
			SIM->SCGC6 |= SIM_SCGC6_TPM1_MASK; /* Ativa o clock do m�dulo TPM1 */
		}
	}

	base->MOD = modulo;

    base->SC &= ~TPM_SC_PS_MASK;
    base->SC |= TPM_SC_PS(prescale);

    base->CNT = 0x00U;					/* Reseta registrador contador */
}


/**********************************************************************
 * Fun��o		:	TPM_InitChannel
 *
 * Descri��o	:   Inicializa um canal do m�dulo TPM.
 *
 * Entradas		:   *base  - registrador base do perif�rico TPM;
 *                  chNum  - num�ro do canal;
 *                  mode   - modo de opera��o do canal, sendo
 * 							 definidos pelo tipo TPM_chMode_t.
 * 					config - poss�veisconfigura��es de cada modo de
 * 							 opera��o do canal, sendo definidos pelo
 * 							 tipo TPM_chConfig_t.
 *
 * Sa�das		:   Nenhuma.
 *
 * Coment�rios 	: 	Nenhum.
 * ********************************************************************/
void TPM_InitChannel(TPM_Type *base, uint8_t chNum, tpmChMode_t mode, tpmChConfig_t config){

	assert(base);
	assert( ( chNum >=0 ) && (chNum < 7) );

	switch(mode)
	{
		case(TPM_OUT_COMPARE_MODE):
			base->SC &= ~TPM_SC_CPWMS_MASK; /*N�o � Center PWM*/
			if(config == TPM_OUT_TOGGLE_GONFIG)
				base->CONTROLS[chNum].CnSC = (TPM_CnSC_MSA_MASK | TPM_CnSC_ELSA_MASK);
			else if(config == TPM_OUT_CLEAR_GONFIG)
				base->CONTROLS[chNum].CnSC = (TPM_CnSC_MSA_MASK | TPM_CnSC_ELSB_MASK);
			else if(config == TPM_OUT_SET_GONFIG)
				base->CONTROLS[chNum].CnSC = (TPM_CnSC_MSA_MASK | (TPM_CnSC_ELSA_MASK) | TPM_CnSC_ELSA_MASK);
			break;
		case(TPM_IN_COMPARE_MODE):
				base->SC &= ~TPM_SC_CPWMS_MASK; /*N�o � Center PWM*/
			if(config == TPM_IN_RISING_EDGE_CONFIG)
				base->CONTROLS[chNum].CnSC = TPM_CnSC_ELSA_MASK;
			else if(config == TPM_IN_FALLING_EDGE_CONFIG)
				base->CONTROLS[chNum].CnSC = TPM_CnSC_ELSB_MASK;
			else
				base->CONTROLS[chNum].CnSC = (TPM_CnSC_ELSA_MASK | TPM_CnSC_ELSA_MASK);
			break;
		case(TPM_EDGE_PWM_MODE):
		base->SC &= ~TPM_SC_CPWMS_MASK; /*N�o � Center PWM*/
			if(config == TPM_PWM_HIGH_TRUE_CONFIG)
				base->CONTROLS[chNum].CnSC = (TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK);
			else
				base->CONTROLS[chNum].CnSC = (TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK);
			break;
		case(TPM_CENTER_PWM_MODE):
				base->SC |= TPM_SC_CPWMS_MASK; /*� Center PWM*/
			if(config == TPM_PWM_HIGH_TRUE_CONFIG)
				base->CONTROLS[chNum].CnSC = (TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK);
			else
				base->CONTROLS[chNum].CnSC = (TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK);
			break;
		default:
			break;
	}
}

/*FIM: FUN��ES P�BLICAS*/
/*=======================================================================================*/

/***************************************************************************************
 * FIM: M�dulo - TPM_HAL.c
 ***************************************************************************************/
