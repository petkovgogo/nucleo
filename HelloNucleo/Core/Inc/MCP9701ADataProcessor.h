/*
 * MCP9701ADataProcessor.h
 *
 *  Created on: May 20, 2021
 *      Author: petkovgogo
 */

#ifndef INC_MCP9701A_DATA_PROCESSOR_H
#define INC_MCP9701A_DATA_PROCESSOR_H

#include "stm32f4xx_hal.h"

#define V_ZERO_DEG_C 0.395      // [V]
#define TEMPERATURE_COEF 0.0195 // [V]
#define MAX_V_OUT 3.3           // [V]
#define MAX_ADC_IN 4095
#define ADC_VOLTG_COEF MAX_V_OUT / MAX_ADC_IN
#define Ts 0.001                // [s]

float readTemperature (ADC_HandleTypeDef *hadc);

#endif /* INC_MCP9701A_DATA_PROCESSOR_H */
