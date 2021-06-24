/*
 * Display.h
 *
 *  Created on: Apr 22, 2021
 *      Author: petkovgogo
 */

#ifndef INC_DISPLAY_H_
#define INC_DISPLAY_H_

#include "Segment.h"

#define SEGMENT_COUNT 8
#define CLEAR_DISPLAY -1

enum BrightnessLevel
{
    MIN_BRIGHTNESS,
    BL_TWO,
    BL_THREE,
    BL_FOUR,
    BL_FIVE,
    BL_SIX,
    BL_SEVEN,
    BL_EIGHT,
    BL_NINE,
    BL_TEN,
    BL_ELEVEN,
    BL_TWELVE,
    BL_THIRTEEN,
    BL_FOURTEEN,
    BL_FIFTEEN,
    BL_SEVENTEEN,
    BL_EIGHTEEN,
    MAX_BRIGHTNESS
};

typedef struct
{
    Segment segments[SEGMENT_COUNT];
    uint8_t lastDisplayedDigit;
} Display;

/**
 * @brief Display Initialization Function
 * @param Receives a pointer to the Display instance to initialize
 * @param Array of the ports of the segments
 * @param Array of the pins of the segments
 * @retval None
 *
 * @note Example: Initialize two displays with the HAL
 *
 GPIO_TypeDef *d0Ports[] =
 { Ports for the first display go here };

 uint16_t d0Pins[] =
 { Pins for the first display go here };

 GPIO_TypeDef *d1Ports[] =
 { Ports for the second display go here };

 uint16_t d1Pins[] =
 { Pins for the second display go here };

 Display d0; // Create the first display
 Display d1; // Create the second display

 displayFactory (&d0, d0Ports, d0Pins); // Initialize the first display
 displayFactory (&d1, d1Ports, d1Pins); // Initialize the second display

 Display *displays[] =
 { &d0, &d1 }; // Create an array of displays, used to display a number
 */
void displayFactory (Display *display, GPIO_TypeDef *ports[], uint16_t pins[]);

/**
 * @brief Displays a whole number on dispayCount displays and updates delayTime to the given delay
 * @param An array of pointers to Display instances
 * @param The count of the displays
 * @param The number to display
 * @retval None
 */
void displayNumber (Display *display[], int displayCount, float number);

#endif /* INC_DISPLAY_H_ */
