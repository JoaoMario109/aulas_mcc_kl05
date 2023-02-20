/***************************************************************************************
 * Módulo      : TPM_HAL.c
 *
 * Revisão     : 1.0
 *
 * Data        : 26/10/2017
 *
 * Descrição   : Arquivo com implementações dos Módulos Timer/PWM (TPM) da Familia Kinetis KL25.
 *
 * Comentários : Nenhum.
 *
 * Autor(es)   : Matheus Leitzke Pinto
 ***************************************************************************************/

/*ARQUIVOS DE CABEÇALHO*/
/*=======================================================================================*/

#include "tpm.h"

/*FIM: ARQUIVOS DE CABEÇALHO*/
/*=======================================================================================*/

/*VARIÁVEIS PRIVADAS*/
/*=======================================================================================*/

static uint16_t matchVal;
static moduleClock_t g_tpmClkSrc;

/*FIM: VARIÁVEIS PRIVADAS*/
/*=======================================================================================*/

/*FUNÇÕES PÚBLICAS*/
/*=======================================================================================*/

/**********************************************************************
 * Função		:	TPM_SetCounterClkSrc
 *
 * Descrição	:   Define a fonte de clock utilizada pelo contador.
 *
 * Entradas		:   *base - registrador base do periférico TPM;
 *                  src   - tipo enumerado que define a fonte de clock.
 *
 * Saídas 		:   STATUS_SUCCESS, se configuração ocorreu corretamente;
 *                  STATUS_FAIL, caso contrário.
 *
 * Comentários 	: 	Deve ser chamada antes da função TPM_Init.
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
		return STATUS_FAIL;
		break;
	}

	SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(src);

	return STATUS_SUCCESS;
}

/**********************************************************************
 * Função		:	TPM_Init
 *
 * Descrição	:   Inicialização do driver do módulo TPM.
 *
 * Entradas		:   *base    - registrador base do periférico TPM;
 *                  modulo   - valor de final de contagem;
 *                  prescale - tipo enumerado com um dos possíveis
 *                             valores de prescaler.
 *
 * Saídas 		:   Nenhuma.
 *
 * Comentários 	: 	Deve ser chamada após a função TPM_SetCounterClkSrc.
 * ********************************************************************/
void TPM_Init(TPM_Type *base, uint16_t modulo, tpmPrescalerValues_t prescale)
{
	assert(base);

	if (TPM0 == base)
	{
		SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK; /* Ativa o clock do módulo TPM0 */
	}
	else
	{
		if (TPM1 == base)
		{
			SIM->SCGC6 |= SIM_SCGC6_TPM1_MASK; /* Ativa o clock do módulo TPM1 */
		}
	}

	base->MOD = modulo;

    base->SC &= ~TPM_SC_PS_MASK;
    base->SC |= TPM_SC_PS(prescale);

    base->CNT = 0x00U;					/* Reseta registrador contador */
}


/**********************************************************************
 * Função		:	TPM_InitChannel
 *
 * Descrição	:   Inicializa um canal do módulo TPM.
 *
 * Entradas		:   *base  - registrador base do periférico TPM;
 *                  chNum  - numéro do canal;
 *                  mode   - modo de operação do canal, sendo
 * 							 definidos pelo tipo TPM_chMode_t.
 * 					config - possíveisconfigurações de cada modo de
 * 							 operação do canal, sendo definidos pelo
 * 							 tipo TPM_chConfig_t.
 *
 * Saídas		:   Nenhuma.
 *
 * Comentários 	: 	Nenhum.
 * ********************************************************************/
void TPM_InitChannel(TPM_Type *base, uint8_t chNum, tpmChMode_t mode, tpmChConfig_t config){

	assert(base);
	assert( ( chNum >=0 ) && (chNum < 7) );

	switch(mode)
	{
		case(TPM_OUT_COMPARE_MODE):
			base->SC &= ~TPM_SC_CPWMS_MASK; /*Não é Center PWM*/
			if(config == TPM_OUT_TOGGLE_GONFIG)
				base->CONTROLS[chNum].CnSC = (TPM_CnSC_MSA_MASK | TPM_CnSC_ELSA_MASK);
			else if(config == TPM_OUT_CLEAR_GONFIG)
				base->CONTROLS[chNum].CnSC = (TPM_CnSC_MSA_MASK | TPM_CnSC_ELSB_MASK);
			else if(config == TPM_OUT_SET_GONFIG)
				base->CONTROLS[chNum].CnSC = (TPM_CnSC_MSA_MASK | (TPM_CnSC_ELSA_MASK) | TPM_CnSC_ELSA_MASK);
			break;
		case(TPM_IN_COMPARE_MODE):
				base->SC &= ~TPM_SC_CPWMS_MASK; /*Não é Center PWM*/
			if(config == TPM_IN_RISING_EDGE_CONFIG)
				base->CONTROLS[chNum].CnSC = TPM_CnSC_ELSA_MASK;
			else if(config == TPM_IN_FALLING_EDGE_CONFIG)
				base->CONTROLS[chNum].CnSC = TPM_CnSC_ELSB_MASK;
			else
				base->CONTROLS[chNum].CnSC = (TPM_CnSC_ELSA_MASK | TPM_CnSC_ELSA_MASK);
			break;
		case(TPM_EDGE_PWM_MODE):
		base->SC &= ~TPM_SC_CPWMS_MASK; /*Não é Center PWM*/
			if(config == TPM_PWM_HIGH_TRUE_CONFIG)
				base->CONTROLS[chNum].CnSC = (TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK);
			else
				base->CONTROLS[chNum].CnSC = (TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK);
			break;
		case(TPM_CENTER_PWM_MODE):
				base->SC |= TPM_SC_CPWMS_MASK; /*É Center PWM*/
			if(config == TPM_PWM_HIGH_TRUE_CONFIG)
				base->CONTROLS[chNum].CnSC = (TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK);
			else
				base->CONTROLS[chNum].CnSC = (TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK);
			break;
		default:
			break;
	}
}

/*FIM: FUNÇÕES PÚBLICAS*/
/*=======================================================================================*/

/***************************************************************************************
 * FIM: Módulo - TPM_HAL.c
 ***************************************************************************************/
