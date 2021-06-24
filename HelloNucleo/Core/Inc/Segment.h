/*
 * Segment.h
 *
 *  Created on: Apr 22, 2021
 *      Author: petkovgogo
 */

#ifndef INC_SEGMENT_H_
#define INC_SEGMENT_H_

#include "main.h"

typedef struct
{
    GPIO_TypeDef *port;
    uint16_t pin;
} Segment;

#endif /* INC_SEGMENT_H_ */
