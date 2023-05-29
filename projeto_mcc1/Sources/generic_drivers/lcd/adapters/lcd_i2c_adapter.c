
/* Self Header */
#include "lcd_i2c_adapter.h"

#ifndef LCD_DISABLE_I2C_ADAPTER

#ifdef LCD_STATIC_OBJECTS_CREATION

/*!< The static list of parallel adapter structures that is used by the API */
static lcdI2CHardwareConfig_t g_lcdI2CAdapterList[LCD_MAX_STATIC_OBJECTS];

static uint8_t g_staticI2CAdaptersCreated;

#endif

/**
 * @brief Creates a I2C hardware adaptor configuration object.
 *
 * @param cols - Number of columns that LCD contains.
 * @param lines - Number of lines that LCD contains.
 * @param char_size - LCD character size.
 * @param base - I2C base memory map.
 * @param slave_addr - LCD slave address.
 *
 * @return - Created I2C LCD hardware configuration object based on provided
 *           parameters.
 */
lcdConfig_t LCD_CreateI2CConfig(
	uint8_t cols, uint8_t lines, uint8_t char_size, I2C_Type *base,
	uint8_t slave_addr
);

/**
 * @brief Dispatches a given byte of data to display
 *
 * @param handle - LCD handler object instance
 * @param value - Byte to be sent to LCD display
 *
 */
void LCD_I2CWriteBits(lcdHandle_t* handle, uint8_t value)
{
	uint8_t high = value & 0xf0;
	uint8_t low = (value << 4) & 0xf0;

	write4bits(high);
	EnablePulse(handle);

	write4bits(low);
	EnablePulse(handle);
}

/**
 * @brief Sets reset bit on LCD display using I2C adapter
 *
 * @param handle - LCD handler object instance
 *
 */
void LCD_I2CSetRs(lcdHandle_t* handle)
{

}

/**
 * @brief Clear reset bit on LCD display using I2C adapter
 *
 * @param handle - LCD handler object instance
 *
 */
void LCD_I2CClrRs(lcdHandle_t* handle)
{

}

/**
 * @brief Sets enable bit on LCD display using I2C adapter
 *
 * @param handle - LCD handler object instance
 *
 */
void LCD_I2CSetEn(lcdHandle_t* handle)
{

}

/**
 * @brief Clear reset bit on LCD display using I2C adapter
 *
 * @param handle - LCD handler object instance
 *
 */
void LCD_I2CClrEn(lcdHandle_t* handle)
{

}

#endif
