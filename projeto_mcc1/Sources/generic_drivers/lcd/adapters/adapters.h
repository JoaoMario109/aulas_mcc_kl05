#ifndef LCD_HD44780_HARDWARE_ADAPTERS_H_
#define LCD_HD44780_HARDWARE_ADAPTERS_H_

/*!< Defines all possible hardware adapters when creating a new lcd */
typedef enum
{
	LCD_PARALLEL_HARD_ADAPTER,
	LCD_I2C_HARD_ADAPTER
} lcdHardwareAdapters_t;

#endif /* !LCD_HD44780_HARDWARE_ADAPTERS_H_ */
