/*
 * GPIOReconfig.h
 *
 *  Created on: Jun 24, 2021
 *      Author: petkovgogo
 */

#ifndef INC_GPIORECONFIG_H_
#define INC_GPIORECONFIG_H_

void GPIOReconf_changeToInput (GPIO_TypeDef *port, uint16_t pin, uint32_t pull);

void GPIOReconf_changeToOutput (GPIO_TypeDef *port,
                                uint16_t pin,
                                uint32_t outputMode,
                                uint32_t pull,
                                uint32_t speed);

#endif /* INC_GPIORECONFIG_H_ */
