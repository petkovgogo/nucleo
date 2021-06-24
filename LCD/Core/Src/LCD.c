/*
 * LCD.c
 *
 *  Created on: Jun 10, 2021
 *      Author: petkovgogo
 */

#include "LCD.h"

#include <string.h>

// PRIVATE CONSTANTS

#define DB_COUNT 8

enum  // xxxx xxxx <=> DB7_DB6_DB5_DB4 - DB3_DB2_DB1_DB0
{
    CLEAR = 1,                        // 0000 0001
    RETURN_HOME = 2,                  // 0000 001-
    ENTRY_MODE_SET = 4,               // 0000 01xx
    DISPLAY_ON_OFF_CONTROL = 8,       // 0000 1xxx
    CURSOR_DISPLAY_SHIFT = 16,        // 0001 xx--
    FUNCTION_SET = 32,                // 001x xx--
    SET_CG_RAM = 64,                  // 01xx xxxx
    SET_DD_RAM = 128,                 // 1xxx xxxx
    CLEAR_OPERATION_TIME = 1650,      // [us]
    RETURN_OPERATION_TIME = 1650,     // [us]
    INSTRUCTION_OPERATION_TIME = 40,  // [us]
    READ_WRITE_OPERATION_TIME = 45    // [us]
};

// GLOBAL DATA

struct
{
    GPIO_TypeDef *port;
    uint16_t pin;
} dbs[DB_COUNT] =
    {
        { LCD_DB0_GPIO_Port, LCD_DB0_Pin },
          { LCD_DB1_GPIO_Port, LCD_DB1_Pin },
          { LCD_DB2_GPIO_Port, LCD_DB2_Pin },
          { LCD_DB3_GPIO_Port, LCD_DB3_Pin },
          { LCD_DB4_GPIO_Port, LCD_DB4_Pin },
          { LCD_DB5_GPIO_Port, LCD_DB5_Pin },
          { LCD_DB6_GPIO_Port, LCD_DB6_Pin },
          { LCD_DB7_GPIO_Port, LCD_DB7_Pin } };

uint32_t scrollDelay = 500;

// PRIVATE IMPLEMENTATION

void RS (GPIO_PinState state)
{
    HAL_GPIO_WritePin (LCD_RS_GPIO_Port, LCD_RS_Pin, state);
}

void RW (GPIO_PinState state)
{
    HAL_GPIO_WritePin (LCD_RW_GPIO_Port, LCD_RW_Pin, state);
}

void enableSignal (GPIO_PinState state)
{
    HAL_GPIO_WritePin (LCD_EnableSignal_GPIO_Port,
    LCD_EnableSignal_Pin,
                       state);
}

void pulse ()
{
    enableSignal (GPIO_PIN_SET);
    DWT_Delay_us (1);
    enableSignal (GPIO_PIN_RESET);
}

void updateDataBuses (uint8_t data)
{
    for (int i = 0; i < DB_COUNT; ++i)
    {
        HAL_GPIO_WritePin (dbs[i].port, dbs[i].pin, data & (1 << i));
    }
}

// PUBLIC INTERFACE

void LCD_Init (void)
{
    DWT_Init ();
    DWT_Delay_us (30000);
    LCD_FunctionSet (GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_RESET);
    LCD_DisplayONOFFControl (GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_RESET);
    LCD_Clear ();
    LCD_EntryModeSet (GPIO_PIN_SET, GPIO_PIN_RESET);
}

void LCD_Clear (void)
{
    RS (GPIO_PIN_RESET);
    RW (GPIO_PIN_RESET);
    updateDataBuses (CLEAR);
    pulse ();
    DWT_Delay_us (CLEAR_OPERATION_TIME);
}

void LCD_ReturnHome (void)
{
    RS (GPIO_PIN_RESET);
    RW (GPIO_PIN_RESET);
    updateDataBuses (RETURN_HOME);
    pulse ();
    DWT_Delay_us (RETURN_OPERATION_TIME);
}

void LCD_EntryModeSet (GPIO_PinState id, GPIO_PinState sh)
{
    RS (GPIO_PIN_RESET);
    RW (GPIO_PIN_RESET);
    updateDataBuses (ENTRY_MODE_SET | (id << 1) | sh);  // sets the parameter bits to the given values
    pulse ();
    DWT_Delay_us (INSTRUCTION_OPERATION_TIME);
}

void LCD_DisplayONOFFControl (GPIO_PinState d, GPIO_PinState c, GPIO_PinState b)
{
    RS (GPIO_PIN_RESET);
    RW (GPIO_PIN_RESET);
    updateDataBuses (DISPLAY_ON_OFF_CONTROL | (d << 2) | (c << 1) | b);  // sets the parameter bits to the given values
    pulse ();
    DWT_Delay_us (INSTRUCTION_OPERATION_TIME);
}

void LCD_CursorOrDisplayShift (GPIO_PinState sc, GPIO_PinState rl)
{
    RS (GPIO_PIN_RESET);
    RW (GPIO_PIN_RESET);
    updateDataBuses (CURSOR_DISPLAY_SHIFT | (sc << 3) | (rl << 2));  // sets the parameter bits to the given values
    pulse ();
    DWT_Delay_us (INSTRUCTION_OPERATION_TIME);
}

void LCD_FunctionSet (GPIO_PinState dl, GPIO_PinState n, GPIO_PinState f)
{
    RS (GPIO_PIN_RESET);
    RW (GPIO_PIN_RESET);
    updateDataBuses (FUNCTION_SET | (dl << 4) | (n << 3) | (f << 2));  // sets the parameter bits to the given values
    pulse ();
    DWT_Delay_us (INSTRUCTION_OPERATION_TIME);
}

void LCD_SetCGRAMAddress (uint8_t address)
{
    RS (GPIO_PIN_RESET);
    RW (GPIO_PIN_RESET);
    updateDataBuses (SET_CG_RAM | address);
    pulse ();
    DWT_Delay_us (INSTRUCTION_OPERATION_TIME);
}

void LCD_SetDDRAMAddress (uint8_t address)
{
    RS (GPIO_PIN_RESET);
    RW (GPIO_PIN_RESET);
    updateDataBuses (SET_DD_RAM | address);
    pulse ();
    DWT_Delay_us (INSTRUCTION_OPERATION_TIME);
}

void LCD_WriteChar (uint8_t ch)
{
    RS (GPIO_PIN_SET);
    RW (GPIO_PIN_RESET);
    updateDataBuses (ch);
    pulse ();
    DWT_Delay_us (READ_WRITE_OPERATION_TIME);
}

void LCD_WriteText (uint8_t *text)
{
    size_t textLength = strlen ((char*) text);

    if (LCD_SIZE < textLength)
    {
        LCD_FunctionSet (GPIO_PIN_SET, GPIO_PIN_RESET, GPIO_PIN_RESET);
        LCD_SetCursor(7, 0);

        for (int i = 0; i < textLength; ++i)
        {
            LCD_WriteChar (text[i]);
            DWT_Delay_us (scrollDelay * 1000);

            LCD_CursorOrDisplayShift (GPIO_PIN_SET, GPIO_PIN_RESET);
        }

        return;
    }

    uint8_t passed = 0;

    LCD_FunctionSet (GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_RESET);

    for (int i = 0; i < textLength; ++i)
    {
        if (text[i] == '\n')
        {
            LCD_SetCursor (0, 1);
            passed++;

            continue;
        }

        LCD_WriteChar (text[i]);

        if (!passed && i == 7)
        {
            LCD_SetCursor (0, 1);
        }
    }
}

void LCD_BuildCustomChar (uint8_t location, uint8_t charmap[])
{
    LCD_SetCGRAMAddress (location << 3);

    for (int i = 0; i < 8; ++i)
    {
        LCD_WriteChar (charmap[i]);
    }
}

void LCD_SetCursor (uint8_t col, uint8_t row)
{
    uint8_t rowOffset[] =
        { 0, 40 };

    LCD_SetDDRAMAddress (col + rowOffset[row]);
}

void LCD_SetScrollDelay (uint32_t delay)
{
    scrollDelay = delay;
}
