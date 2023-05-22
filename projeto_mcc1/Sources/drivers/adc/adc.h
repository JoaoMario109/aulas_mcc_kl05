/***************************************************************************************
 * M�dulo      : adc.h
 * Revis�o     : 1.0
 * Data        : 16/02/2023
 * Descri��o   : Arquivo com implementa��es do M�dulo ADC da Familia Kinetis KL05.
 * Coment�rios : Nenhum.
 * Autor(es)   : Matheus Leitzke Pinto
 ***************************************************************************************/

#ifndef ADC_DRV_H_
#define ADC_DRV_H_

#include "drivers/common_driver.h"


/*Flags de status do canal*/
enum adcChannelStatusFlags
{
    ADC_CHANNEL_CONVERSION_DONE_FLAG = ADC_SC1_COCO_MASK, /* Convers�o pronta. */
};

/*Flags de status do ADC*/
enum adcStatusFlags
{
    ADC_ACTIVE_FLAG = ADC_SC2_ADACT_MASK, /*ADC est� ativo. */
    ADC_CALIBRATION_FAILED_FLAG = ADC_SC3_CALF_MASK, /*!< Calibra��o falhou. */
};

/*Divisor do clock do ADC*/
typedef enum adcClockDivider
{
    ADC_CLOCK_DIV_1 = 0U, /* Dividir por 1 a entrada do clock para o m�dulo. */
    ADC_CLOCK_DIV_2 = 1U, /* Dividir por 2 a entrada do clock para o m�dulo. */
    ADC_CLOCK_DIV_4 = 2U, /* Dividir por 4 a entrada do clock para o m�dulo. */
    ADC_CLOCK_DIV_8 = 3U, /* Dividir por 8 a entrada do clock para o m�dulo. */
} adcClockDivider_t;

/*Resolu��o do ADC*/
typedef enum adcResolution
{
    ADC_RESOLUTION_8_BIT = 0U,  /* Resolu��o de 8 bits. */
    ADC_RESOLUTION_12_BIT = 1U, /* Resolu��o de 12 bits. */
    ADC_RESOLUTION_10_BIT = 2U, /* Resolu��o de 10 bits. */
} adcResolution_t;

/*Fonte de clock do ADC*/
typedef enum adcClockSource
{
    ADC_CLOCK_SRC_ALT_0 = 0U, /* Alternativa de fonte de clock 0. */
    ADC_CLOCK_SRC_ALT_1 = 1U, /* Alternativa de fonte de clock 1. */
    ADC_CLOCK_SRC_ALT_2 = 2U, /* Alternativa de fonte de clock 2. */
    ADC_CLOCK_SRC_ALT_3 = 3U, /* Alternativa de fonte de clock 3. */
    ADC_ASYNC_CLOCK_SRC = ADC_CLOCK_SRC_ALT_3, /* Usa fonte de clock ass�ncrona interna. */
} adcClockSource_t;

/* Modo de longa amostragem. */
typedef enum adcLongSampleMode
{
    ADC_LONG_SAMPLE_CYCLE_24 = 0U,  /* 20 extra ADCK cycles, 24 ADCK cycles total. */
    ADC_LONG_SAMPLE_CYCLE_16 = 1U,  /* 12 extra ADCK cycles, 16 ADCK cycles total. */
    ADC_LONG_SAMPLE_CYCLE_10 = 2U,  /* 6 extra ADCK cycles, 10 ADCK cycles total. */
    ADC_LONG_SAMPLE_CYCLE_6 = 3U,   /* 2 extra ADCK cycles, 6 ADCK cycles total. */
    ADC_LONG_SAMPLE_DISABLE = 4U, /* Desabilita o recurso de longa amostragem. */
} adcLongSampleMode_t;

/* Modo m�dia por hardware. */
typedef enum adcHardwareAverageMode
{
    ADC_HARDWARE_AVG_COUNT_4 = 0U,   /* M�dia de hardware com 4 amostras. */
    ADC_HARDWARE_AVG_COUNT_8 = 1U,   /* M�dia de hardware com 8 amostras. */
    ADC_HARDWARE_AVG_COUNT_16 = 2U,  /* M�dia de hardware com 16 amostras. */
    ADC_HARDWARE_AVG_COUNT_32 = 3U,  /* M�dia de hardware com 32 amostras. */
    ADC_HARDWARE_AVG_DISABLE = 4U,   /* Desabilita o recurso de m�dia por hardware.*/
} adcHardwareAverageMode_t;

/* Fonte de disparo de �nicio de convers�o do ADC. */
typedef enum adcHardwareTriggerSrc
{
	ADC_EXTRG_IN 	  = 0x0U,
    ADC_CMP0_OUTPUT   = 0x1U,
    ADC_PIT_TRIGGER_0 = 0x4U,
	ADC_PIT_TRIGGER_1 = 0x5U,
    ADC_TPM0_OVERFLOW = 0x8U,
	ADC_TPM1_OVERFLOW = 0x9U,
	ADC_TPM2_OVERFLOW = 0xAU,
	ADC_RTC_ALARM 	  = 0xCU,
	ADC_RTC_SECONDS   = 0xDU,
    ADC_LPTMR0_TRIGGER = 0xEU,
} adcHardwareTriggerSrc_t;

/* Modo de compara��o no hardware. O valor converito x s� estar� dispon�vel se
 * a condi��o escolhida for atendida. Os valores "value1" e "value2" e  s�o definidos
 * em  adcHardwareCompareConfig_t */
typedef enum adcHardwareCompareMode
{
    ADC_HARDWARE_COMPARE_MODE_0 = 0U, /* x < value1. */
    ADC_HARDWARE_COMPARE_MODE_1 = 1U, /* x > value1. */
    ADC_HARDWARE_COMPARE_MODE_2 = 2U, /* if value1 <= value2, then x < value1 || x > value2;
                                           else, value1 > x > value2. */
    ADC_HARDWARE_COMPARE_MODE_3 = 3U, /* if value1 <= value2, then value1 <= x <= value2;
                                           else x >= value1 || x <= value2. */
} adcHardwareCompareMode_t;

/* Configura��o de compara��o no hardware. */
typedef struct adcHardwareCompareConfig
{
    adcHardwareCompareMode_t hardwareCompareMode; /* Modo de compara��o no hardware. */
    int16_t value1;                               /* Defini��o de valor value1 de compara��o. */
    int16_t value2;                               /* Defini��o de valor value2 de compara��o. */
} adcHardwareCompareConfig_t;

/* Configura��o de convers�o de canal. */
typedef struct adcChannelConfig
{
    uint32_t channelNumber;                    /* Valor de 0-31, correspondente ao n�mero do canal.
    bool enableInterruptOnConversionCompleted; /* Gera pedido de interrup��o quando convers�o est� completa. */
} adcChannelConfig_t;


#if defined(__cplusplus)
extern "C" {
#endif

/**********************************************************************
 * Fun��o		:	ADC_SetClockDiv
 *
 * Descri��o	:   Define o valor de divis�o do clock de entrada do ADC.
 *
 * Entradas		:   *base - registrador base do perif�rico ADC;
 *                  div   - valor de divis�o, limitado por tipo enumerado.
 *
 * Sa�das		:   Nenhuma.
 *
 * Coment�rios 	: 	Nenhum.
 * ********************************************************************/
static inline void ADC_SetClockDiv( ADC_Type *base, adcClockDivider_t div )
{
	assert(base);

	base->CFG1 &= ~ADC_CFG1_ADIV_MASK;
	base->CFG1 |= ADC_CFG1_ADIV(div);
}

/**********************************************************************
 * Fun��o		:	ADC_SetLowPowerMode
 *
 * Descri��o	:   Define o consumo de pot�ncia de convers�o como baixa.
 *
 * Entradas		:   *base - registrador base do perif�rico ADC.
 *
 * Sa�das		:   Nenhuma.
 *
 * Coment�rios 	: 	Nenhum.
 * ********************************************************************/
static inline void ADC_SetLowPowerMode( ADC_Type *base )
{
	assert(base);

	base->CFG1 |= ADC_CFG1_ADLPC_MASK;
}

/**********************************************************************
 * Fun��o		:	ADC_SetNormalPowerMode
 *
 * Descri��o	:   Define o consumo de pot�ncia de convers�o como normal.
 *
 * Entradas		:   *base - registrador base do perif�rico ADC.
 *
 * Sa�das		:   Nenhuma.
 *
 * Coment�rios 	: 	Nenhum.
 * ********************************************************************/
static inline void ADC_SetNormalPowerMode( ADC_Type *base )
{
	assert(base);

	base->CFG1 &= ~ADC_CFG1_ADLPC_MASK;
}

/**********************************************************************
 * Fun��o		:	ADC_SetHighSpeedMode
 *
 * Descri��o	:   Define a velocidade de convers�o como r�pida.
 *
 * Entradas		:   *base - registrador base do perif�rico ADC.
 *
 * Sa�das		:   Nenhuma.
 *
 * Coment�rios 	: 	Nenhum.
 * ********************************************************************/
static inline void ADC_SetHighSpeedMode( ADC_Type *base )
{
	assert(base);

	base->CFG2 |= ADC_CFG2_ADLSTS_MASK;
}

/**********************************************************************
 * Fun��o		:	ADC_SetNormalSpeedMode
 *
 * Descri��o	:   Define a velocidade de convers�o como normal.
 *
 * Entradas		:   *base - registrador base do perif�rico ADC.
 *
 * Sa�das		:   Nenhuma.
 *
 * Coment�rios 	: 	Nenhum.
 * ********************************************************************/
static inline void ADC_SetNormalSpeedMode( ADC_Type *base )
{
	assert(base);

	base->CFG2 &= ~ADC_CFG2_ADLSTS_MASK;
}

/**********************************************************************
 * Fun��o		:	ADC_AsyncClkOutEnable
 *
 * Descri��o	:   Habilita sa�da do clock interno ass�ncrono.
 *
 * Entradas		:   *base - registrador base do perif�rico ADC.
 *
 * Sa�das		:   Nenhuma.
 *
 * Coment�rios 	: 	Nenhum.
 * ********************************************************************/
static inline void ADC_AsyncClkOutEnable( ADC_Type *base )
{
	assert(base);

	base->CFG2 |= ADC_CFG2_ADACKEN_MASK;
}

/**********************************************************************
 * Fun��o		:	ADC_SetVAltVoltage
 *
 * Descri��o	:   Define e tens�o de refer�ncia como VAlt.
 *
 * Entradas		:   *base - registrador base do perif�rico ADC.
 *
 * Sa�das		:   Nenhuma.
 *
 * Coment�rios 	: 	Nenhum.
 * ********************************************************************/
static inline void ADC_AsyncClkOutDisable( ADC_Type *base )
{
	assert(base);

	base->CFG2 &= ~ADC_CFG2_ADACKEN_MASK;
}

/**********************************************************************
 * Fun��o		:	ADC_SetVAltVoltage
 *
 * Descri��o	:   Define e tens�o de refer�ncia como VAlt.
 *
 * Entradas		:   *base - registrador base do perif�rico ADC.
 *
 * Sa�das		:   Nenhuma.
 *
 * Coment�rios 	: 	Nenhum.
 * ********************************************************************/
static inline void ADC_SetVAltVoltage( ADC_Type *base )
{
	assert(base);

	base->SC2 &= ~ADC_SC2_REFSEL_MASK;
	base->SC2 |= ADC_SC2_REFSEL(1U);
}

/**********************************************************************
 * Fun��o		:	ADC_SetVRefVoltage
 *
 * Descri��o	:   Define e tens�o de refer�ncia como VRef.
 *
 * Entradas		:   *base - registrador base do perif�rico ADC.
 *
 * Sa�das		:   Nenhuma.
 *
 * Coment�rios 	: 	Nenhum.
 * ********************************************************************/
static inline void ADC_SetVRefVoltage( ADC_Type *base )
{
	assert(base);

	base->SC2 &= ~ADC_SC2_REFSEL_MASK;
}

/**********************************************************************
 * Fun��o		:	ADC_ContinuousConversionEnable
 *
 * Descri��o	:   Habilita o modo de convers�o continua do m�dulo ADC.
 *
 * Entradas		:   *base - registrador base do perif�rico ADC.
 *
 * Sa�das		:   Nenhuma.
 *
 * Coment�rios 	: 	Nenhum.
 * ********************************************************************/
static inline void ADC_ContinuousConversionEnable( ADC_Type *base )
{
	assert(base);

	base->SC3 |= ADC_SC3_ADCO_MASK;
}

/**********************************************************************
 * Fun��o		:	ADC_ContinuousConversionDisable
 *
 * Descri��o	:   Desabilita o modo de convers�o continua do m�dulo ADC.
 *
 * Entradas		:   *base - registrador base do perif�rico ADC.
 *
 * Sa�das		:   Nenhuma.
 *
 * Coment�rios 	: 	Nenhum.
 * ********************************************************************/
static inline void ADC_ContinuousConversionDisable( ADC_Type *base )
{
	assert(base);

	base->SC3 &= ~ADC_SC3_ADCO_MASK;
}

/**********************************************************************
 * Fun��o		:	ADC_Init
 *
 * Descri��o	:   Inicializa o pino do m�dulo ADC.
 *
 * Entradas		:   *base - registrador base do perif�rico ADC.
 *
 * Sa�das		:   Nenhuma.
 *
 * Coment�rios 	: 	Nenhum.
 * ********************************************************************/
static inline void ADC_Init(ADC_Type *base)
{
    assert(NULL != base);

    /* Enable the clock. */
    SIM_SCGC6 |= 1 << 27;
}

/**********************************************************************
 * Fun��o		:	ADC_Deinit
 *
 * Descri��o	:   Desabilita o m�dulo ADC.
 *
 * Entradas		:   *base - registrador base do perif�rico ADC.
 *
 * Sa�das		:   Nenhuma.
 *
 * Coment�rios 	: 	Nenhum.
 * ********************************************************************/
static inline void ADC_Deinit(ADC_Type *base)
{
	assert(NULL != base);

    /* Disable the clock. */
	SIM_SCGC6 &= ~(1 << 27);
}

/**********************************************************************
 * Fun��o		:	ADC_DoAutoCalibration
 *
 * Descri��o	:   Realiza auto-calibragem no ADC.
 *
 * Entradas		:   *base - registrador base do perif�rico ADC.
 *
 * Sa�das		: GENERAL_STATUS_SUCCESSSS, calibra��o ocorreu corretamente;
 *                  GENERAL_STATUS_FAIL, caso contr�rio.
 *
 * Coment�rios 	: 	Nenhum.
 * ********************************************************************/
uint8_t ADC_DoAutoCalibration(ADC_Type *base);

/**********************************************************************
 * Fun��o		:	ADC_SetOffsetValue
 *
 * Descri��o	:   Esse valor de offset entra em vigor no
 *                  resultado da convers�o. Se o valor do offset n�o
 *                  for zero, o resultado da leitura � subtra�do por ele.
 *                  Observe que a calibra��o do hardware preenche o
 *                  valor de deslocamento automaticamente.
 *
 * Entradas		:   *base - registrador base do perif�rico ADC;
 *                  value - valor de offset.
 *
 * Sa�das		:   Nenhuma.
 *
 * Coment�rios 	: 	Nenhum.
 * ********************************************************************/
static inline void ADC_SetOffsetValue(ADC_Type *base, int16_t value)
{
	assert(NULL != base);

    base->OFS = (uint32_t)(value);
}

/**********************************************************************
 * Fun��o		:	ADC_EnableDMA
 *
 * Descri��o	:   Habilita gera��o de gatilho do DMA quando convers�o
 *                  do ADC estiver completa.
 *
 * Entradas		:   *base - registrador base do perif�rico ADC.
 *
 * Sa�das		:   Nenhuma.
 *
 * Coment�rios 	: 	Nenhum.
 * ********************************************************************/
static inline void ADC_EnableDMA(ADC_Type *base)
{
	assert(NULL != base);

	base->SC2 |= ADC_SC2_DMAEN_MASK;
}

/**********************************************************************
 * Fun��o		:	ADC_DisableDMA
 *
 * Descri��o	:   Desabilita gera��o de gatilho do DMA quando convers�o
 *                  do ADC estiver completa.
 *
 * Entradas		:   *base - registrador base do perif�rico ADC.
 *
 * Sa�das		:   Nenhuma.
 *
 * Coment�rios 	: 	Nenhum.
 * ********************************************************************/
static inline void ADC_DisableDMA(ADC_Type *base)
{
	assert(NULL != base);

	base->SC2 &= ~ADC_SC2_DMAEN_MASK;
}

/**********************************************************************
 * Fun��o		:	ADC_EnableHardwareTrigger
 *
 * Descri��o	:   Habilita e seleciona fonte de disparo de inicio
 *                  de convers�o no ADC por hardware.
 *
 * Entradas		:   *base - registrador base do perif�rico ADC;
 * 					src   - uma das poss�veis fontes de convers�o
 * 					        definido em tipo enumerado.
 *
 * Sa�das		:   Nenhuma.
 *
 * Coment�rios 	: 	Nenhum.
 * ********************************************************************/
static inline void ADC_EnableHardwareTrigger(ADC_Type *base, adcHardwareTriggerSrc_t src)
{
	assert(NULL != base);

	/* Configure SIM for ADC hw trigger source selection */
	SIM->SOPT7 |= (0x00000080U | src);

	base->SC2 |= ADC_SC2_ADTRG_MASK;
}

/**********************************************************************
 * Fun��o		:	ADC_DisableHardwareTrigger
 *
 * Descri��o	:   Habilita fonte de disparo de inicio
 *                  de convers�o no ADC por hardware.
 *
 * Entradas		:   *base - registrador base do perif�rico ADC.
 *
 * Sa�das		:   Nenhuma.
 *
 * Coment�rios 	: 	Nenhum.
 * ********************************************************************/
static inline void ADC_DisableHardwareTrigger(ADC_Type *base)
{
	assert(NULL != base);

	base->SC2 &= ~ADC_SC2_ADTRG_MASK;
}

/**********************************************************************
 * Fun��o		:	ADC_SetHardwareCompareConfig
 *
 * Descri��o	:   Define o modo de compara��o de convers�o por
 *                  hardware.
 *
 * Entradas		:   *base - registrador base do perif�rico ADC;
 *                  hardwareCompareMode - um dos quatro modos de
 *                      compara��o, definidos em tipo enumerado;
 *                  value1, value2 - os valores de compara��o.
 *
 * Sa�das		:   Nenhuma.
 *
 * Coment�rios 	: 	Verificar o tipo adcHardwareCompareMode_t para
 *                  mais detalhes.
 * ********************************************************************/
void ADC_SetHardwareCompareConfig(ADC_Type *base, adcHardwareCompareMode_t hardwareCompareMode, int16_t value1, int16_t value2);

/**********************************************************************
 * Fun��o		:	ADC_SetHardwareAverage
 *
 * Descri��o	:   Define o modo de convers�o por m�dia de amostras.
 *
 * Entradas		:   *base - registrador base do perif�rico ADC;
 *                  mode - quantidade de m�dias, definida por tipo
 *                         enumerado.
 *
 * Sa�das		:   Nenhuma.
 *
 * Coment�rios 	: 	Nenhum.
 * ********************************************************************/
void ADC_SetHardwareAverage(ADC_Type *base, adcHardwareAverageMode_t mode);

/**********************************************************************
 * Fun��o		:	ADC_GetStatusFlags
 *
 * Descri��o	:   Retorna a flag de status do ADC.
 *
 * Entradas		:   *base - registrador base do perif�rico ADC.
 *
 * Sa�das		:   - ADC_ACTIVE_FLAG, se ADC est� ativo;
 *                  - ADC_CALIBRATION_FAILED_FLAG, se calibra��o falhou.
 *
 * Coment�rios 	: 	Nenhum.
 * ********************************************************************/
uint32_t ADC_GetStatusFlags(ADC_Type *base);

/**********************************************************************
 * Fun��o		:	ADC_ClearCalibStatusFlags
 *
 * Descri��o	:   Limpa a flag de status de calibra��o do ADC.
 *
 * Entradas		:   *base - registrador base do perif�rico ADC.
 *
 * Sa�das		:   Nenhuma.
 *
 * Coment�rios 	: 	Nenhum.
 * ********************************************************************/
void ADC_ClearCalibStatusFlags(ADC_Type *base);

/**********************************************************************
 * Fun��o		:	ADC_SetChConfig
 *
 * Descri��o	:   Configura um canal do ADC.
 *
 * Entradas		:   *base - registrador base do perif�rico ADC;
 *                  channelGroup - n�mero do grupo do canal: 0 (A) ou 1 (B);
 *                  channelNumber - n�mero do canal;
 *                  enableInterruptOnConversionCompleted - true se deve ser
 *                     gerada interrup��o no fim da convers�o, ou false
 *                     caso contr�rio.
 *
 * Sa�das		:   Nenhuma.
 *
 * Coment�rios 	: 	O grupo do canal A � usado para convers�es disparadas
 *                  por software, enquando o grupo B pode ser usado tanto
 *                  para disparo de software, quanto de hardware.
 * ********************************************************************/
void ADC_SetChConfig(ADC_Type *base, uint32_t channelGroup, uint32_t channelNumber, bool enableInterruptOnConversionCompleted);

/**********************************************************************
 * Fun��o		:	ADC_GetChConversionValue
 *
 * Descri��o	:   Retorna o valor de convers�o do ADC pelo canal atribu�do
 *                  ao grupo A ou B.
 *
 * Entradas		:   *base - registrador base do perif�rico ADC;
 *                   channelGroup - grupo A (0) ou B (1).
 *
 * Sa�das		:   Valor da convers�o.
 *
 * Coment�rios 	: 	Nenhum.
 * ********************************************************************/
static inline uint32_t ADC_GetChConversionValue(ADC_Type *base, uint32_t channelGroup)
{
	assert(NULL != base);
    assert(channelGroup < ADC_R_COUNT);

    return base->R[channelGroup];
}

/**********************************************************************
 * Fun��o		:	ADC_IsConversionDone
 *
 * Descri��o	:   Retorna verdadeiro se convers�o do canal atribu�do
 *                  ao grupo A ou B do ADC est� pronto.
 *
 * Entradas		:   *base - registrador base do perif�rico ADC;
 *                   channelGroup - grupo A (0) ou B (1).
 *
 * Sa�das		:   true, se convers�o finalizada;
 *                  false, caso contr�rio.
 *
 * Coment�rios 	: 	Nenhum.
 * ********************************************************************/
bool ADC_IsConversionDone(ADC_Type *base, uint32_t channelGroup);


#if defined(__cplusplus)
}
#endif

#endif /* ADC_DRV_H_ */
