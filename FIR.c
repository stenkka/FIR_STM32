/*
 * FIR.c
 *
 *  Created on: Oct 18, 2023
 *      Author: aaron
 */
#include "FIR.h"

void initFIR_Filter(FIR_Filter *filter)
{
	for (int i = 0;i<TAPS_NUM;i++)
	{
		filter->buffer[i] = 0;
	}
	filter->output = 0;
	filter->index = 0;
	filter->taps = filterTaps;
}


void updateFIR_Filter(FIR_Filter *filter, float input)
{
	if (filter->index == TAPS_NUM) { filter->index = 0; }
	filter->buffer[filter->index] = input;
	for (int i = 0;i<TAPS_NUM;i++)
	{
		filter->output += filter->buffer[(filter->index + i) % 16] * filterTaps[i];
	}
	filter->index++;
}
