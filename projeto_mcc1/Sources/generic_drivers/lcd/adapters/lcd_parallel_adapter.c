
/* Self Header */
#include "lcd_parallel_adapter.h"

#ifndef LCD_DISABLE_PARALLEL_ADAPTER

#ifdef LCD_STATIC_OBJECTS_CREATION

/*!< The static list of parallel adapter structures that is used by the API */
static lcdParallelHardwareAdapter_t g_lcdParallelAdapterList[LCD_MAX_STATIC_OBJECTS];
/*!< Amount of static configs already allocated */
static uint8_t g_staticParallelAdaptersCreated;

#endif

/**
 * @brief Creates a Parallel hardware adaptor configuration object.
 *
 * @param cols - Number of columns that LCD contains.
 * @param lines - Number of lines that LCD contains.
 * @param char_size - LCD character size.
 *
 * @return - Created I2C LCD hardware configuration object based on provided
 *					 parameters.
 */
lcdParallelHardwareAdapter_t LCD_CreateParallelAdapter(
	uint8_t cols, uint8_t lines, uint8_t char_size,
#ifdef LCD_8_BIT_MODE
	lcdPin_t data[8],
#else
	lcdPin_t data[4],
#endif
	lcdPin_t reset, lcdPin_t enable
)
{

}

/**
 * @brief Dispatches a given byte of data to display
 *
 * @param handle - LCD handler object instance
 * @param value - Byte to be sent to LCD display
 *
 */
void LCD_ParallelWriteBits(lcdHandle_t* handle, uint8_t value)
{
	lcdParallelHardwareAdapter_t* adapter = (struct lcdParallelHardwareAdapter_t*)(handle->config);
#ifdef LCD_4_BIT_MODE
	uint8_t count = 4;
#else
	uint8_t count = 8;
#endif
	for (int i = 0; i < count; i++)
	{
		if((value >> i) & 0x01)
		{
			MCU_PortSet(adapter->data[i].portRegister, adapter->data[i].pinMask);
		}
		else
		{
			MCU_PortClear(adapter->data[i].portRegister, adapter->data[i].pinMask);
		}
	}

	EnablePulse(handle);
}

/**
 * @brief Sets reset bit on LCD display using parallel adapter
 *
 * @param handle - LCD handler object instance
 *
 */
void LCD_ParallelSetRs(lcdHandle_t* handle)
{
	lcdParallelHardwareAdapter_t* adapter = (struct lcdParallelHardwareAdapter_t*)(handle->config);
	MCU_PortSet(adapter->rs.portRegister, adapter->rs.pinMask)
}

/**
 * @brief Clear reset bit on LCD display using parallel adapter
 *
 * @param handle - LCD handler object instance
 *
 */
void LCD_ParallelClrRs(lcdHandle_t* handle)
{
	lcdParallelHardwareAdapter_t* adapter = (struct lcdParallelHardwareAdapter_t*)(handle->config);
	MCU_PortClear(adapter->rs.portRegister, adapter->rs.pinMask)
}

/**
 * @brief Sets enable bit on LCD display using parallel adapter
 *
 * @param handle - LCD handler object instance
 *
 */
void LCD_ParallelSetEn(lcdHandle_t* handle)
{
	lcdParallelHardwareAdapter_t* adapter = (struct lcdParallelHardwareAdapter_t*)(handle->config);
	MCU_PortSet(adapter->en.portRegister, adapter->en.pinMask)
}

/**
 * @brief Clear reset bit on LCD display using parallel adapter
 *
 * @param handle - LCD handler object instance
 *
 */
void LCD_ParallelClrEn(lcdHandle_t* handle)
{
	lcdParallelHardwareAdapter_t* adapter = (struct lcdParallelHardwareAdapter_t*)(handle->config);
	MCU_PortClear(adapter->en.portRegister, adapter->en.pinMask)
}

#endif
