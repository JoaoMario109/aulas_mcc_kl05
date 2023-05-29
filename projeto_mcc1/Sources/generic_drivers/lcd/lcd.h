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

/* LCD hardware adapters */
#include "./adapters/adapters.h"

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

/*!< Hardware configuration structure describing current display hardware connections */
typedef void* lcdHardwareConfig_t;

/*!< Structure that holds the LCD bus information.*/
typedef struct
{
	/*!< Hardware connection configuration */
	lcdHardwareConfig_t* config;

	/*!< Bus dispatcher callback */
	void (*dispatch)(lcdHandle_t* handle, uint8_t value);

	/*!< Bus set reset callback */
	void (*setRs)(lcdHandle_t* handle);

	/*!< Bus clear reset callback */
	void (*clrRs)(lcdHandle_t* handle);

	/*!< Bus set enable callback */
	void (*setEn)(lcdHandle_t* handle);

	/*!< Bus clear enable callback */
	void (*clrEn)(lcdHandle_t* handle);
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

/*!
 * @brief LCD handle structure used internally
 */
typedef struct
{
	/*!< The pointer to the configuration structure passed by the user.*/
	lcdConfig_t* config;

	/*!< The display function command that will be send to the LCD controller.*/
	uint8_t display_function;

	/*!< The display control command that will be send to the LCD controller.*/
	uint8_t display_control;

	/*!< The display mode command that will be send to the LCD controller.*/
	uint8_t display_mode;

	/*!< The row offsets list determined by the number of LCD rows and columns.*/
	uint8_t row_offsets[4];

#ifdef __FREERTOS_H
#ifdef LCD_REENTRANT_ACCESS
	/*!< The mutex used for mutual exclusion in API calls.*/
	xSemaphoreHandle lcdAccessMutex;
#endif /* LCD_REENTRANT_ACCESS */
#endif /* __FREERTOS_H */
} lcdHandle_t;

/**
 * @brief Initialize the LCD module.
 *
 * @param config - the variable with the configurations defined.
 * @param adapter - Hardware adapter to use when initializing this handle.
 *
 * @return - The specific LCD module handle that must be passed
 *           to the API for communication or;
 *         - NULL, if was not possible to create the handle.
 */
lcdHandle_t LCD_Init(lcdConfig_t *config, lcdHardwareAdapters_t adapter);

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
