/*
 * gpioReconfig.c
 *
 *  Created on: Jun 17, 2021
 *      Author: petkovgogo
 */

#include "main.h"
#include "stdint.h"

#define GPIO_NUMBER 16U

uint8_t getPosition (uint16_t pin)
{
    for (int pos = 0; pos < GPIO_NUMBER; ++pos)
    {
        if (pin & (1U << pos))
        {
            return pos;
        }
    }

    return 0;
}

void GPIOReconf_changeToInput (GPIO_TypeDef *port, uint16_t pin, uint32_t pull)
{
    uint32_t configuration = 0U;
    uint8_t pos = getPosition (pin);

    configuration = port->PUPDR;
    configuration &= ~(GPIO_PUPDR_PUPD0 << (pos * 2U));  // clear the current bits
    configuration |= (pull << (pos * 2U));               // set the new mode
    port->PUPDR = configuration;

    configuration = port->MODER;
    configuration &= ~(GPIO_MODER_MODER0 << (pos * 2U));          // clear the current bits
    configuration |= (GPIO_MODE_INPUT & GPIO_MODE) << (pos * 2U); // set the new mode
    port->MODER = configuration;
}

void GPIOReconf_changeToOutput (GPIO_TypeDef *port,
                                uint16_t pin,
                                uint32_t outputMode,
                                uint32_t pull,
                                uint32_t speed)
{
    uint32_t configuration = 0U;
    uint8_t pos = getPosition (pin);

    configuration = port->PUPDR;
    configuration &= ~(GPIO_PUPDR_PUPD0 << (pos * 2U)); // clear the current bits
    configuration |= pull << (pos * 2U);                // set the new mode
    port->PUPDR = configuration;

    configuration = port->OSPEEDR;
    configuration &= ~(GPIO_OSPEEDER_OSPEEDR0 << (pos * 2U)); // clear the current bits
    configuration |= speed << (pos * 2U);                     // set the new mode
    port->OSPEEDR = configuration;

    configuration = port->OTYPER;
    configuration &= ~(GPIO_OTYPER_OT0 << pos);                      // clear the current bits
    configuration |= ((outputMode & GPIO_OUTPUT_TYPE) >> 3U) << pos; // set the new mode
    port->OTYPER = configuration;

    configuration = port->MODER;
    configuration &= ~(GPIO_MODER_MODER0 << (pos * 2U));     // clear the current bits
    configuration |= (outputMode & GPIO_MODE) << (pos * 2U); // set the new mode
    port->MODER = configuration;

}
