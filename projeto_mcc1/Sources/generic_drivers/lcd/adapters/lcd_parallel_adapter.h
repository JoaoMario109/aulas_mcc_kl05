#ifndef LCD_HD44780_PARALLEL_HARDWARE_ADAPTER_H_
#define LCD_HD44780_PARALLEL_HARDWARE_ADAPTER_H_

/* LCD definitions includes */
#include "lcd.h"

#ifndef LCD_DISABLE_PARALLEL_ADAPTER

/*!< Structure that holds the necessary LCD pin information when using GPIO. */
typedef struct
{
	/*!< Port register attached to this pin */
	portPinsRegister_t portRegister;

	/*!< Pin mask */
	portPinMask_t pinMask;
} lcdPin_t;

/*!< Structure that holds information when using parallel connection with GPIO */
typedef struct
{
	/*!< The pins for data/commands transaction.*/
#ifdef LCD_8_BIT_MODE
	lcdPin_t data[8]; /*!< data[7] -> DB7, data[6] -> DB6, ...*/
#else
	lcdPin_t data[4]; /*!< data[3] -> DB7, data[2] -> DB6, ...*/
#endif

	/*!< Register select pin.*/
	lcdPin_t rs;

	/*!< Enable pin.*/
	lcdPin_t en;
} lcdParallelHardwareAdapter_t;

/**
 * @brief Creates a Parallel hardware adaptor configuration object.
 *
 * @param cols - Number of columns that LCD contains.
 * @param lines - Number of lines that LCD contains.
 * @param char_size - LCD character size.
 *
 * @return - Created I2C LCD hardware configuration object based on provided
 *           parameters.
 */
lcdParallelHardwareAdapter_t LCD_CreateParallelAdapter(
	uint8_t cols, uint8_t lines, uint8_t char_size,
#ifdef LCD_8_BIT_MODE
	lcdPin_t data[8],
#else
	lcdPin_t data[4],
#endif
	lcdPin_t reset, lcdPin_t enable
);

/**
 * @brief Dispatches a given byte of data to display
 *
 * @param handle - LCD handler object instance
 * @param value - Byte to be sent to LCD display
 *
 */
void LCD_ParallelWriteBits(lcdHandle_t* handle, uint8_t value);

/**
 * @brief Sets reset bit on LCD display using parallel adapter
 *
 * @param handle - LCD handler object instance
 *
 */
void LCD_ParallelSetRs(lcdHandle_t* handle);

/**
 * @brief Clear reset bit on LCD display using parallel adapter
 *
 * @param handle - LCD handler object instance
 *
 */
void LCD_ParallelClrRs(lcdHandle_t* handle);

/**
 * @brief Sets enable bit on LCD display using parallel adapter
 *
 * @param handle - LCD handler object instance
 *
 */
void LCD_ParallelSetEn(lcdHandle_t* handle);

/**
 * @brief Clear reset bit on LCD display using parallel adapter
 *
 * @param handle - LCD handler object instance
 *
 */
void LCD_ParallelClrEn(lcdHandle_t* handle);

#endif

#endif /* !LCD_HD44780_PARALLEL_HARDWARE_ADAPTER_H_ */
