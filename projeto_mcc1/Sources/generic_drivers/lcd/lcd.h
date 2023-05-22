/**
 * @file    lcd.h
 * @author  Matheus Leitzke Pinto <matheus.pinto@ifsc.edu.br>
 * @version 1.0
 * @date    2021
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * A generic driver for LCD with HD44780 controller.
 * It is necessary to include the "mcu_general_config.h" file with
 * specific implementations of:
 *     - MCU_PortSet(portPinsRegister, portPinMask) and
 *     - MCU_PortClear(portPinsRegister, portPinMask).
 * It is also necessary the definition of types:       
 *     - portPinsRegister_t - the register type used by "MCU_PortSet"
 *                            and "MCU_PortClear";
 *     - portPinMask_t - the mask type used by "MCU_PortSet"
 *                       and "MCU_PortClear" to indicate the pin
 *                       position to be referred.
 *
 * Supported OSes:
 *
 *   - FreeRTOS.
 *
 */

#ifndef LCD_HD44780_H_
#define LCD_HD44780_H_

#include "../../mcu_general_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @addtogroup lcd
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!< HD44780 control commands.*/
#define LCD_CLEAR_DISPLAY 0x01
#define LCD_RETURN_HOME 0x02
#define LCD_ENTRY_MODE_SET 0x04
#define LCD_DISPLAY_CONTROL 0x08
#define LCD_CURSOR_SHIFT 0x10
#define LCD_FUNCTION_SET 0x20
#define LCD_SET_CG_RAM_ADDR 0x40
#define LCD_SET_DD_RAM_ADDR 0x80

/*!< Flags for display entry mode.*/
#define LCD_ENTRY_RIGHT 0x00
#define LCD_ENTRY_LEFT 0x02
#define LCD_ENTRY_SHIFT_INCREMENT 0x01
#define LCD_ENTRY_SHIFT_DECREMENT 0x00

/*!< Flags for display on/off control.*/
#define LCD_DISPLAY_ON 0x04
#define LCD_DISPLAY_OFF 0x00
#define LCD_CURSOR_ON 0x02
#define LCD_CURSOR_OFF 0x00
#define LCD_BLINK_ON 0x01
#define LCD_BLINK_OFF 0x00

/*!< Flags for display/cursor shift.*/
#define LCD_DISPLAY_MOVE 0x08
#define LCD_CURSOR_MOVE 0x00
#define LCD_MOVE_RIGHT 0x04
#define LCD_MOVE_LEFT 0x00

/*!< Flags for function set.*/
#define LCD_4_BIT_MODE 0x00
#ifndef LCD_4_BIT_MODE
	#define LCD_8_BIT_MODE 0x10
#endif
#define LCD_2_LINE 0x08
#define LCD_1_LINE 0x00
#define LCD_5x10_DOTS 0x04
#define LCD_5x8_DOTS 0x00

/* !< Defines if LCD instances will be created statically.
 *    If commented, LCD instances will be allocated dynamically in heap.
 */
#define LCD_STATIC_OBJECTS_CREATION
#ifdef LCD_STATIC_OBJECTS_CREATION
	#define LCD_MAX_STATIC_OBJECTS 1 /*!< The number of object instances that will be created statically.*/
#endif

/*!< Uncomment this macro if want to use reentrant access of API.*/
#define LCD_REENTRANT_ACCESS

/*!< Defines all possible hardware adapters when creating a new lcd */
typedef enum
{
	LCD_PARALLEL_HARD_ADAPTER,
	LCD_I2C_HARD_ADAPTER
} lcdHardwareAdapters_t;

/*!< The handle that will must be passed to the API to communicate with specific lcd module.*/
typedef void* lcdHandle_t;

/*!< Hardware configuration structure describing current display hardware connections */
typedef void* lcdHardwareConfig_t;

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
} lcdParallelConf_t;

/*!< Structure that holds information when using I2C module */
typedef struct
{
	/*!< Module memory map to use */
	I2C_Type *base;

	/*!< Slave address for display module */
	uint8_t slave_addr;
} lcdI2CConf_t;

/*!< Structure that holds the LCD bus information.*/
typedef struct
{
	/*!< Hardware connection configuration */
	lcdHardwareConfig_t* config;

	/*!< Bus dispatcher callback */
	void (*dispatch)(lcdHandle_t handle, uint8_t value);
} lcdBus_t;

/*!
 * @brief LCD manager configuration structure
 *
 * This structure holds the configuration settings for the LCD module.
 */
typedef struct
{
	/*!< The display columns number. */
	uint8_t cols;

	/*!< The display lines number. */
	uint8_t lines;

	/*!< The display character size in pixels. */
	uint8_t char_size;

	/*!< The LCD bus. */
	lcdBus_t bus;
} lcdConfig_t;

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
 * @brief Creates a Parallel hardware adaptor configuration object.
 *
 * @param cols - Number of columns that LCD contains.
 * @param lines - Number of lines that LCD contains.
 * @param char_size - LCD character size.
 *
 * @return - Created I2C LCD hardware configuration object based on provided
 *           parameters.
 */
lcdConfig_t LCD_CreateParallelConfig(
	uint8_t cols, uint8_t lines, uint8_t char_size,
#ifdef LCD_8_BIT_MODE
	lcdPin_t data[8],
#else
	lcdPin_t data[4],
#endif
	lcdPin_t reset, lcdPin_t enable
);

/**
 * @brief Initialize the LCD module.
 *
 * @param config - the variable with the configurations defined.
 *
 * @return - The specific LCD module handle that must be passed
 *           to the API for communication or;
 *         - NULL, if was not possible to create the handle.
 */
lcdHandle_t LCD_Init(lcdConfig_t *config);

/**
 * @brief Clear the LCD screen.
 *void (*dispatch)(lcdHandle_t handle, uint8_t value);
 * @param handle - the specific LCD handle.
 */
void LCD_Clear(lcdHandle_t handle);

/**
 * @brief Set the cursor to the LCD home.
 *
 * @param handle - the specific LCD handle.
 */
void LCD_Home(lcdHandle_t handle);

/**
 * @brief Set the cursor to a specific position.
 *
 * @param handle - the specific LCD handle.
 * @param col    - the cursor column number.
 * @param row    - the cursor row number.
 */
void LCD_SetCursor(lcdHandle_t handle, uint8_t col, uint8_t row);

/**
 * @brief Turn the display off.
 *
 * @param handle - the specific LCD handle.
 */
void LCD_NoDisplay(lcdHandle_t handle);

/**
 * @brief Turn the display on.
 *
 * @param handle - the specific LCD handle.
 */
void LCD_Display(lcdHandle_t handle);

/**
 * @brief Turns the underline cursor off.
 *
 * @param handle - the specific LCD handle.
 */
void LCD_NoCursor(lcdHandle_t handle);

/**
 * @brief Turns the underline cursor on.
 *
 * @param handle - the specific LCD handle.
 */
void LCD_Cursor(lcdHandle_t handle);

/**
 * @brief Turn off the blinking cursor.
 *
 * @param handle - the specific LCD handle.
 */
void LCD_NoBlink(lcdHandle_t handle);

/**
 * @brief Turn on the blinking cursor.
 *
 * @param handle - the specific LCD handle.
 */
void LCD_Blink(lcdHandle_t handle);

/**
 * @brief Scroll the display to the left
 *        without changing the RAM.
 *
 * @param handle - the specific LCD handle.
 */
void LCD_ScrollDisplayLeft(lcdHandle_t handle);

/**
 * @brief Scroll the display to the right
 *        without changing the RAM.
 *
 * @param handle - the specific LCD handle.
 */
void LCD_ScrollDisplayRight(lcdHandle_t handle);

/**
 * @brief This is for text that flows Left to Right.
 *
 * @param handle - the specific LCD handle.
 */
void LCD_LeftToRight(lcdHandle_t handle);

/**
 * @brief This is for text that flows Right to Left.
 *
 * @param handle - the specific LCD handle.
 */
void LCD_RightToLeft(lcdHandle_t handle);

/**
 * @brief This will 'right justify' text from the cursor.
 *
 * @param handle - the specific LCD handle.
 */
void LCD_Autoscroll(lcdHandle_t handle);

/**
 * @brief This will 'left justify' text from the cursor.
 *
 * @param handle - the specific LCD handle.
 */
void LCD_NoAutoscroll(lcdHandle_t handle);

/**
 * @brief Allows us to fill the first 8 CGRAM locations
 *        with custom characters.
 *
 * @param handle   - the specific LCD handle.
 * @param location - the CGRAM location.
 * @param charmap  - the custom character mapped in a matrix.
 */
void LCD_CreateChar(lcdHandle_t handle, uint8_t location, uint8_t charmap[]);

/**
 * @brief Write a string in the current LCD cursor position.
 *
 * @param handle - the specific LCD handle.
 * @param str    - the string buffer.
 */
void LCD_WriteString(lcdHandle_t handle, char *str);

/**
 * @brief Send the big numbers bit map to the HD44780 controller.
 *
 * @note This function must be called before to use the LCD_WriteBigNum
 *       function.
 *
 * @param handle - the specific LCD handle.
 */
void LCD_CreateBigNumsChars(lcdHandle_t handle);

/**
 * @brief Print a number in the "big number" format.
 *
 * @note This function can be called only after the call of the
 *       LCD_CreateBigNumsChars function.
 *
 * @param handle - the specific LCD handle.
 * @param col    - the column where to print the number.
 * @param num    - the number from 0 to 9 to be printed.
 */
void LCD_WriteBigNum(lcdHandle_t handle, uint8_t col, uint8_t num);

/**
 * @brief Send a command to the HD44780 controller.
 *
 * This function is used used inside API. It should
 * be avoided in favor of other API functions. The user
 * must call this function only if it is necessary to
 * make another kind of communication not covered to
 * the API.
 *
 * @param handle - the specific LCD handle.
 * @param value  - the command value.
 */
void LCD_Command(lcdHandle_t handle, uint8_t value);

/**
 * @brief Send a data to the HD44780 controller.
 *
 * This function is used used inside API. It should
 * be avoided in favor of other API functions. The user
 * must call this function only if it is necessary to
 * make another kind of communication not covered to
 * the API.
 *
 * @param handle - the specific LCD handle.
 * @param value  - the data value.
 */
void LCD_Write(lcdHandle_t handle, uint8_t value);

#ifdef __cplusplus
}  /* extern "C" */
#endif

/*! @}*/

#endif /* LCD_HD44780_H_ */
