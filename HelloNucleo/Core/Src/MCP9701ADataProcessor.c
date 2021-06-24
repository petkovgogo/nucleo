/*
 * MCP9701ADataProcessor.c
 *
 *  Created on: May 20, 2021
 *      Author: petkovgogo
 */

#include "MCP9701ADataProcessor.h"

#include "string.h"

#define BUFFER_SIZE 100
#define BUFFER_EMPTY 0U
#define BUFFER_FULL 1U
//#define LIVE_VIEW_TEMPERATURE

#ifdef LIVE_VIEW_TEMPERATURE

    float Ta = 0;

#endif // LIVE_VIEW_TEMPERATURE

uint32_t buffer[BUFFER_SIZE];
uint8_t bufferFull = BUFFER_EMPTY;
int currCycle = 0;

float adcAvg (ADC_HandleTypeDef *hadc)
{
    if (BUFFER_SIZE <= currCycle)
    {
        currCycle = 0;
        bufferFull = BUFFER_FULL;
    }

    if (HAL_ADC_PollForConversion (hadc, Ts * 1000) == HAL_OK)
    {
        buffer[currCycle++] = HAL_ADC_GetValue (hadc);
    }

    if (bufferFull)
    {
        uint32_t sum = 0;

        for (int i = 0; i < BUFFER_SIZE; ++i)
        {
            sum += buffer[i];
        }

        return (float) sum / BUFFER_SIZE;
    }

    return 0;
}

float readTemperature (ADC_HandleTypeDef *hadc)
{
    float Vout = adcAvg (hadc) * ADC_VOLTG_COEF;

/*
         Vout - V0℃
     t = -----------
            Tcoef
*/

#ifdef LIVE_VIEW_TEMPERATURE
    Ta = (Vout - V_ZERO_DEG_C) / TEMPERATURE_COEF;

    return Ta;
#else

    return (Vout - V_ZERO_DEG_C) / TEMPERATURE_COEF;

#endif // LIVE_VIEW_TEMPERATURE
}
