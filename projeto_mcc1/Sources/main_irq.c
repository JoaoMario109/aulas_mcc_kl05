#include "mcu_general_config.h"
#include "generic_drivers/lcd/lcd.h"
#include "libraries/delay/delay.h"
#include "libraries/emb_util/emb_util.h"
#include "drivers/port/port.h"
#include "drivers/gpio/gpio.h"

/* Ver p�gina 147 e 148 do manual de refer�ncia do KL05 para saber quais
 * pinos possuem IRQ. */

void PORTA_IRQHandler( void )
{
	if ( PORT_GetIRQFlag( PORTA, 10 ) )// PTA10 causou interrup��o?
    {
        //Limpa a flag de ISR do PTA10
        PORT_ClearIRQFlag( PORTA, 10 );
        //Inverte valor l�gico de PTB9
        GPIO_TogglePin( GPIOB, 9 );
    }
}

void NMI_Handler(void)
{
	for ( uint32_t i = 0; ; ++i )
	{
		__asm ( "nop" );
		++i;
	}
}

void HardFault_Handler(void)
{
	for ( uint32_t i = 0; ; ++i )
	{
		__asm ( "nop" );
		++i;
	}
}

int main(void)
{
	PORT_Init( PORTA );
	PORT_Init( PORTB );

	GPIO_InitOutputPin( GPIOB, 9, 1 );
	GPIO_InitInputPin( GPIOA, 10 );
	PORT_PullEnable( PORTA, 10 );
	PORT_IRQEnable( PORTA, 10, PORT_IRQ_ON_FALLING_EDGE );

    NVIC_EnableIRQ( PORTA_IRQn );

    for ( ; ; )
    {

    }

    return 0;
}
