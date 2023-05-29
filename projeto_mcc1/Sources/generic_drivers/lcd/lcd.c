/**
 * @file	lcd.h
 * @author  Matheus Leitzke Pinto <matheus.pinto@ifsc.edu.br>
 * @version 1.0
 * @date	2021
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
 *	 - MCU_PortSet(portPinsRegister, portPinMask) and
 *	 - MCU_PortClear(portPinsRegister, portPinMask).
 * It is also necessary the definition of types:
 *	 - portPinsRegister_t - the register type used by "MCU_PortSet"
 *							and "MCU_PortClear";
 *	 - portPinMask_t - the mask type used by "MCU_PortSet"
 *					   and "MCU_PortClear" to indicate the pin
 *					   position to be referred.
 *
 * Supported OSes:
 *
 *   - FreeRTOS.
 *
 */

#include "lcd.h"
#include "../../libraries/delay/delay.h"
#include "../../libraries/emb_util/emb_util.h"

#ifdef __FREERTOS_H
#include "FreeRTOS.h"
#include "semphr.h"
#endif /* __FREERTOS_H */

/*!< Enumeration to identify objects creation.*/
enum
{
	LCD_OBJECT_IS_HANDLE,
	LCD_OBJECT_IS_CONFIG
};

#ifdef LCD_STATIC_OBJECTS_CREATION

/*!< The static list of configuration structures that is returned to the LCD API.*/
static lcdConfig_t g_lcdConfigList[LCD_MAX_STATIC_OBJECTS];

/*!< The static list of handle structures that is returned to the LCD API.*/
static lcdHandle_t g_lcdHandleList[LCD_MAX_STATIC_OBJECTS];

/*!< The number of configuration and handle structures created using the LCD API.*/
static uint8_t g_staticConfigsCreated;
static uint8_t g_staticHandlesCreated;

#endif

/* waiting macros */
#define Waitns(x) \
		Delay_Waitns(x)				 /* Wait x ns */
#define Waitus(x) \
		Delay_Waitus(x)				 /* Wait x us */
#define Waitms(x) \
		Delay_Waitms(x)				 /* Wait x ms */

#ifndef __FREERTOS_H
#define LcdEnterMutex(x) (void)0
#define LcdExitMutex(x) (void)0
#else
#ifdef LCD_REENTRANT_ACCESS
/* macros for mutex access and release */
#define LcdEnterMutex(x) xSemaphoreTake(x->lcdAccessMutex, portMAX_DELAY)
#define LcdExitMutex(x) xSemaphoreGive(x->lcdAccessMutex)
#endif /* LCD_REENTRANT_ACCESS */
#endif /* __FREERTOS_H */

/*******************************************************************************
 * Private Prototypes
 ******************************************************************************/

/**
 * @brief Creates a pulse to transfer data and/or commands.
 *
 * @param handle - the specific LCD handle.
 *
 */
static void EnablePulse(lcdHandle_t* handle);

/**
 * @brief Set the row offset, which is based from the LCD module used.
 *
 * @param handle - the specific LCD handle.
 *
 */
static void SetRowOffsets(lcdHandle_t* handle);

/**
 * @brief Writes data to display module using correct dispatcher
 *
 * @param handle - the specific LCD handle.
 * @param value - Byte of data to be sent to LCD.
 *
 */
static void WriteBits(lcdHandle_t* handle, uint8_t value);

/**
 * @brief Create an specific object used by an LCD instance.
 *
 * @param objectType - \a K_LCD_OBJECT_IS_HANDLE, if want to create a LCD handle;
 * 					   \a K_LCD_OBJECT_IS_CONFIG, if want to create a LCD configuration structure.
 *
 */
static void* CreateObject(uint8_t objectType);

/*******************************************************************************
 * Code
 ******************************************************************************/

static void* CreateObject(uint8_t objectType)
{
	void* objectCreated = NULL;
#ifdef LCD_STATIC_OBJECTS_CREATION
	switch (objectType)
	{
	case LCD_OBJECT_IS_HANDLE:
		if(g_staticHandlesCreated < LCD_MAX_STATIC_OBJECTS)
		{
			objectCreated = (void*)&g_lcdHandleList[g_staticHandlesCreated++];
		}
		break;
	case LCD_OBJECT_IS_CONFIG:
		if(g_staticConfigsCreated < LCD_MAX_STATIC_OBJECTS)
		{
			objectCreated = (void*)&g_lcdConfigList[g_staticConfigsCreated++];
		}
		break;
	}
#else
	switch (objectType)
	{
	case LCD_OBJECT_IS_HANDLE:
		objectCreated = embUtil_Malloc(sizeof(lcdHandle_t));
		break;
	case LCD_OBJECT_IS_CONFIG:
		objectCreated = embUtil_Malloc(sizeof(lcdConfig_t));
		break;
	}
#endif
	return objectCreated;
}

static void EnablePulse(lcdHandle_t* handle)
{
	handle->config->bus.clrEn(handle);
	Waitus(1);
	handle->config->bus.setEn(handle);
	Waitus(1); // enable pulse must be >450ns
	handle->config->bus.clrEn(handle);
	Waitus(100); // commands need > 37us to settle
}

static void SetRowOffsets(lcdHandle_t* handle)
{
	handle->row_offsets[0] = 0x00;
	handle->row_offsets[1] = 0x40;
	handle->row_offsets[2] = 0x00 + handle->config->cols;
	handle->row_offsets[3] = 0x40 + handle->config->cols;
}

lcdHandle_t LCD_Init(lcdConfig_t *config, lcdHardwareAdapters_t adapter)
{
	EmbUtil_Assert(config);

	struct lcdHandle_t *handle = (lcdHandle_t*)CreateObject(LCD_OBJECT_IS_HANDLE);
	if(!handle)
	{
		return NULL;
#ifdef __FREERTOS_H
		handle->lcdAccessMutex = xSemaphoreCreateMutex();
		if(!handle->lcdAccessMutex)
		{
			DestroyObject(handle, K_LCD_OBJECT_IS_HANDLE);
		}
#endif /* __FREERTOS_H */
	}



	handle->config = config;
#ifdef LCD_4_BIT_MODE
	handle->display_function = LCD_4_BIT_MODE | LCD_1_LINE | LCD_5x8_DOTS;
#else
	handle->display_function = LCD_8_BIT_MODE | LCD_1_LINE | LCD_5x8_DOTS;
#endif

	if (config->lines > 1)
	{
		handle->display_function |= LCD_2_LINE;
	}

	SetRowOffsets(handle);

	// for some 1 line displays you can select a 10 pixel high font
	if ((config->char_size != LCD_5x8_DOTS) && (config->lines == 1)) {
		handle->display_function |= LCD_5x10_DOTS;
	}

	// SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
	// according to datasheet, we need at least 40ms after power rises above 2.7V
	// before sending commands. So we'll wait 50ms.
	Waitms(50);
	ClrRS(handle);

	ClrEN(handle);

	//put the LCD into 4 bit or 8 bit mode
#ifdef LCD_4_BIT_MODE
	// this is according to the hitachi HD44780 datasheet
	// figure 24, pg 46
	// we start in 8bit mode, try to set 4 bit mode
	WriteBits(handle, 0x03);
	Waitus(4500); // wait min 4.1ms
	// second try
	WriteBits(handle, 0x03);
	Waitus(4500); // wait min 4.1ms
	// third go!
	WriteBits(handle, 0x03);
	Waitus(150);
	// finally, set to 4-bit interface
	WriteBits(handle, 0x02);
#else
	// this is according to the hitachi HD44780 datasheet
	// page 45 figure 23

	// Send function set command sequence
	LCD_Command((lcdHandle_t)handle, LCD_FUNCTION_SET | display_function);
	Waitus(4500); // wait min 4.1ms

	// second try
	LCD_Command((lcdHandle_t)handle, LCD_FUNCTION_SET | display_function);
	Waitus(150);

	// third go
	LCD_Command((lcdHandle_t)handle, LCD_FUNCTION_SET | display_function);
#endif
	// finally, set # lines, font size, etc.
	LCD_Command((lcdHandle_t)handle, LCD_FUNCTION_SET | handle->display_function);

	// turn the display on with no cursor or blinking default
	handle->display_control = LCD_DISPLAY_ON | LCD_CURSOR_OFF | LCD_BLINK_OFF;
	LCD_Display((lcdHandle_t)handle);

	// clear it off
	LCD_Clear((lcdHandle_t)handle);

	// Initialize to default text direction (for romance languages)
	handle->display_mode = LCD_ENTRY_LEFT | LCD_ENTRY_SHIFT_DECREMENT;
	// set the entry mode
	LCD_Command((lcdHandle_t)handle, LCD_ENTRY_MODE_SET | handle->display_mode);

	Waitus(400);

	return (lcdHandle_t)handle;
}


/********** high level commands, for the user! */


/*********** mid level commands, for sending data/cmds */

inline void LCD_Command(lcdHandle_t handle, uint8_t value)
{
	lcdHandle_t* lcdHandle = (lcdHandle_t*)handle;
	lcdHandle->config->bus.clrRs(lcdHandle);
#ifdef LCD_8_BIT_MODE
	WriteBits(lcdHandle, value);
#else
	WriteBits(lcdHandle, value >> 4);
	WriteBits(lcdHandle, value);
#endif
}

inline void LCD_Write(lcdHandle_t handle, uint8_t value)
{
	lcdHandle_t* lcdHandle = (lcdHandle_t*)handle;
	lcdHandle->config->bus.setRs(lcdHandle);
#ifdef LCD_8_BIT_MODE
	WriteBits(lcdHandle, value);
#else
	WriteBits(lcdHandle, value >> 4);
	WriteBits(lcdHandle, value);
#endif
}

static inline void WriteBits(lcdHandle_t* handle, uint8_t value)
{
	handle->config->bus.dispatch(handle, value);
}
