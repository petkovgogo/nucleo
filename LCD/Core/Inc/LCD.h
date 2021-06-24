/*
 * LCD.h
 *
 *  Created on: Jun 10, 2021
 *      Author: petkovgogo
 */

#ifndef INC_LCD_H_
#define INC_LCD_H_

#include <stdint.h>
#include "main.h"
#include "Utils.h"

#define LCD_SIZE 16

void LCD_Init (void);
void LCD_Clear (void);
void LCD_ReturnHome (void);
void LCD_EntryModeSet (GPIO_PinState id, GPIO_PinState sh);
void LCD_DisplayONOFFControl (GPIO_PinState d,
                              GPIO_PinState c,
                              GPIO_PinState b);
void LCD_CursorOrDisplayShift (GPIO_PinState sc, GPIO_PinState rl);
void LCD_FunctionSet (GPIO_PinState dl, GPIO_PinState n, GPIO_PinState f);
void LCD_SetCGRAMAddress (uint8_t address);
void LCD_SetDDRAMAddress (uint8_t address);
// TODO LCD_ReadBF
void LCD_WriteChar (uint8_t ch);
void LCD_WriteText (uint8_t *text);
void LCD_BuildCustomChar (uint8_t location, uint8_t charmap[]);
void LCD_SetCursor (uint8_t col, uint8_t row);
// TODO LCD_ReadData
void LCD_SetScrollDelay (uint32_t delay);

#endif /* INC_LCD_H_ */
