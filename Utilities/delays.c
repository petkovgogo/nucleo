/*
 * utils.c
 *
 *  Created on: Jun 11, 2021
 *      Author: petkovgogo
 */

#include "Utils.h"

void DWT_Init (void)
{
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

void DWT_Delay_us (uint32_t delay)
{
    uint32_t currCycle = DWT->CYCCNT;
    delay *= HAL_RCC_GetHCLKFreq() / 1000000U; // convert from [us] to clock cycles

    while ((DWT->CYCCNT - currCycle) < delay);
}
