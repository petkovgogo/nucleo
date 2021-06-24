/*
 * display.c
 *
 *  Created on: Apr 22, 2021
 *      Author: petkovgogo
 */

#include "Display.h"
#include <math.h>

#define DP_SEGMENT 7

/**
 * @brief These values correspond to the segments needed for each digit,
 *        keeping each segment as a bit in the order HGFEDCBA
 */
enum
{
    OFF = 0,         // 0000 0000
    ZERO = 63,       // 0011 1111 <=> A, B, C, D, E, F
    ONE = 6,         // 0000 0110 <=> B, C
    TWO = 91,        // 0101 1011 <=> A, B, D, E, G
    THREE = 79,      // 0100 1111 <=> A, B, C, D, G
    FOUR = 102,      // 0110 0110 <=> B, C, F, G
    FIVE = 109,      // 0110 1101 <=> A, C, D, F, G
    SIX = 125,       // 0111 1101 <=> A, C, D, E, F, G
    SEVEN = 7,       // 0000 0111 <=> A, B, C
    EIGHT = 127,     // 0111 1111 <=> A, B, C, D, E, F, G
    NINE = 111,      // 0110 1111 <=> A, B, C, D, F, G
    DP = 128,        // 1000 0000 <=> H
    LETTER_E = 121,  // 0111 1001 <=> A, D, E, F, G
    LETTER_R = 80    // 0101 0000 <=> E, G
};

uint8_t digitSegments[] =
    { ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE };

void displayFactory (Display *display, GPIO_TypeDef *ports[], uint16_t pins[])
{
    display->lastDisplayedDigit = 0;

    for (int i = 0; i < SEGMENT_COUNT; ++i)
    {
        display->segments[i].port = ports[i];
        display->segments[i].pin = pins[i];
    }
}

void light (Display *display, uint8_t digit)
{
    for (int i = 0; i < SEGMENT_COUNT; ++i)
    {
        if (digit & (1 << i))
        {
            HAL_GPIO_WritePin (display->segments[i].port,
                               display->segments[i].pin,
                               GPIO_PIN_RESET);
        }
        else
        {
            HAL_GPIO_WritePin (display->segments[i].port,
                               display->segments[i].pin,
                               GPIO_PIN_SET);
        }
    }
}

void displayError (Display *displays[], int displayCount)
{
    light (displays[--displayCount], LETTER_E);

    if (0 <= --displayCount)
    {
        light (displays[displayCount], LETTER_R);
    }

    if (0 <= --displayCount)
    {
        light (displays[displayCount], LETTER_R);
    }
}

int digitCount (int number)
{
    int count = 1;

    while (number / 10 > 0)
    {
        count++;
        number /= 10;
    }

    return count;
}

void displayNumber (Display *displays[], int displayCount, float number)
{
    if (number == CLEAR_DISPLAY)
    {
        for (int i = 0; i < displayCount; ++i)
        {
            light (displays[i], OFF);
        }

        return;
    }

    int digits = digitCount (number);

    if (digits > displayCount)
    {
        displayError (displays, displayCount);
    }
    else
    {
        int displayNum = (int) number;
        uint8_t dpActive = 0;

        if (digits < displayCount
                && (int) (number * pow (10, (displayCount - digits))) % 10 != 0)
        {
            displayNum = (int) (number * pow (10, (displayCount - digits)));

            dpActive = displayCount - digits;
            digits += dpActive;
        }

        uint8_t digit = 0;

        for (int i = 0; i < digits; ++i)
        {
            digit = digitSegments[displayNum % 10];

            if (dpActive && dpActive == i)
            {
                digit ^= DP;
            }

            light (displays[i], digit);
            displayNum /= 10;
        }
    }
}
