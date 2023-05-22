/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "adc.h"

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/


/*
 * ADC0 trigger select
Selects the ADC0 trigger source when alternative triggers are functional in stop and VLPS modes. .
0x0 External trigger pin input (EXTRG_IN)
0x1 CMP0 output
0x4 PIT trigger 0
0x5 PIT trigger 1
0x8 TPM0 overflow
0x9 TPM1 overflow
0xA TPM2 overflow
0xC RTC alarm
0xD RTC seconds
0xE LPTMR0 trigger*/
void ADC_ConfigTriggerSource(uint8_t hwTrigger)
{
    /* Configure SIM for ADC hw trigger source selection */
    SIM->SOPT7 |= (0x00000080U | hwTrigger);
}

void ADC_SetLongSampleTimeMode( ADC_Type *base, adcLongSampleMode_t mode )
{
	assert(base);

	base->CFG1 &= ~ADC_CFG1_ADLSMP_MASK; /*Limpa configura��es anteriores*/
	if ( ADC_LONG_SAMPLE_DISABLE != mode )
	{
		base->CFG1 |= ADC_CFG1_ADLSMP(1U);

		base->CFG2 &= ~ADC_CFG2_ADLSTS_MASK;
		base->CFG2 |= ADC_CFG2_ADLSTS(mode);
	}
}

uint8_t ADC_DoAutoCalibration(ADC_Type *base)
{
    bool bHWTrigger = false;
    volatile uint32_t tmp32; /* 'volatile' here is for the dummy read of ADCx_R[0] register. */
    uint8_t status = GENERAL_STATUS_SUCCESS;

    /* The calibration would be failed when in hardware mode.
     * Remember the hardware trigger state here and restore it later if the hardware trigger is enabled.*/
    if (0U != (ADC_SC2_ADTRG_MASK & base->SC2))
    {
        bHWTrigger = true;
        base->SC2 &= ~ADC_SC2_ADTRG_MASK;
    }

    /* Clear the CALF and launch the calibration. */
    base->SC3 |= ADC_SC3_CAL_MASK | ADC_SC3_CALF_MASK;

    while (false == ADC_IsConversionDone(base, 0U))
    {
        /* Check the CALF when the calibration is active. */
        if (0U != (ADC_CALIBRATION_FAILED_FLAG & ADC_GetStatusFlags(base)))
        {
            status = GENERAL_STATUS_FAIL;
            break;
        }
    }
    tmp32 = base->R[0]; /* Dummy read to clear COCO caused by calibration. */

    /* Restore the hardware trigger setting if it was enabled before. */
    if (bHWTrigger)
    {
        base->SC2 |= ADC_SC2_ADTRG_MASK;
    }
    /* Check the CALF at the end of calibration. */
    if (0U != (ADC_CALIBRATION_FAILED_FLAG & ADC_GetStatusFlags(base)))
    {
        status = GENERAL_STATUS_FAIL;
    }
    if (GENERAL_STATUS_SUCCESS != status) /* Check if the calibration process is succeed. */
    {
        return status;
    }

    /* Calculate the calibration values. */
    tmp32 = base->CLP0 + base->CLP1 + base->CLP2 + base->CLP3 + base->CLP4 + base->CLPS;
    tmp32 = 0x8000U | (tmp32 >> 1U);
    base->PG = tmp32;

    return GENERAL_STATUS_SUCCESS;
}

void ADC_SetHardwareCompareConfig(ADC_Type *base, adcHardwareCompareMode_t hardwareCompareMode, int16_t value1, int16_t value2)
{
    uint32_t tmp32 = base->SC2 & ~(ADC_SC2_ACFE_MASK | ADC_SC2_ACFGT_MASK | ADC_SC2_ACREN_MASK);

    /* Enable the feature. */
    tmp32 |= ADC_SC2_ACFE_MASK;

    /* Select the hardware compare working mode. */
    switch (hardwareCompareMode)
    {
        case ADC_HARDWARE_COMPARE_MODE_0:
            break;
        case ADC_HARDWARE_COMPARE_MODE_1:
            tmp32 |= ADC_SC2_ACFGT_MASK;
            break;
        case ADC_HARDWARE_COMPARE_MODE_2:
            tmp32 |= ADC_SC2_ACREN_MASK;
            break;
        case ADC_HARDWARE_COMPARE_MODE_3:
            tmp32 |= ADC_SC2_ACFGT_MASK | ADC_SC2_ACREN_MASK;
            break;
        default:
            break;
    }
    base->SC2 = tmp32;

    /* Load the compare values. */
    base->CV1 = ADC_CV1_CV(value1);
    base->CV2 = ADC_CV2_CV(value2);
}


void ADC_SetHardwareAverage(ADC_Type *base, adcHardwareAverageMode_t mode)
{
    uint32_t tmp32 = base->SC3 & ~(ADC_SC3_AVGE_MASK | ADC_SC3_AVGS_MASK);

    if (ADC_HARDWARE_AVG_DISABLE != mode)
    {
        tmp32 |= ADC_SC3_AVGE_MASK | ADC_SC3_AVGS(mode);
    }
    base->SC3 = tmp32;
}


uint32_t ADC_GetStatusFlags(ADC_Type *base)
{
    uint32_t ret = 0;

    if (0U != (base->SC2 & ADC_SC2_ADACT_MASK))
    {
        ret |= ADC_ACTIVE_FLAG;
    }
    if (0U != (base->SC3 & ADC_SC3_CALF_MASK))
    {
        ret |= ADC_CALIBRATION_FAILED_FLAG;
    }
    return ret;
}

void ADC_ClearCalibStatusFlags(ADC_Type *base)
{
	base->SC3 |= ADC_SC3_CALF_MASK;
}

void ADC_SetChConfig(ADC_Type *base, uint32_t channelGroup, uint32_t channelNumber, bool enableIRQ)
{
    assert(channelGroup < ADC_SC1_COUNT);

    uint32_t sc1 = ADC_SC1_ADCH(channelNumber); /* Set the channel number. */

    /* Enable the interrupt when the conversion is done. */
    if (enableIRQ)
    {
        sc1 |= ADC_SC1_AIEN_MASK;
    }
    base->SC1[channelGroup] = sc1;
}

bool ADC_IsConversionDone(ADC_Type *base, uint32_t channelGroup)
{
    assert(channelGroup < ADC_SC1_COUNT);

    bool ret = false;

    if (0U != (base->SC1[channelGroup] & ADC_SC1_COCO_MASK))
    {
        ret = true;
    }
    return ret;
}
