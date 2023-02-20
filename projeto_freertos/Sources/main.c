#include "MKL05Z4.h"
#include "FreeRTOS.h"
#include "task.h"

void task(void * arg)
{
	TickType_t prevTime = xTaskGetTickCount();

	for(;;)
	{
		GPIOB->PTOR = 1 << 9;
		vTaskDelayUntil(&prevTime, 1000/portTICK_RATE_MS);
	}
}

int main(void)
{
	SIM->SCGC5 |= 1 << 10;
	PORTB->PCR[9] |= PORT_PCR_MUX(1);
	GPIOB->PDDR |= 1 << 9;

	xTaskCreate( task,
			     "myTask",
				 200,
				 NULL,
				 1,
				 NULL);

	vTaskStartScheduler();

	return 0;
}
