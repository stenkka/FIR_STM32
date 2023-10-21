/*
 * FIR.c
 *
 *  Created on: Oct 18, 2023
 *      Author: aaron
 */
#include "FIR.h"
/*

FIR filter designed with
http://t-filter.appspot.com

sampling frequency: 100 Hz

* 0 Hz - 10 Hz
  gain = 1
  desired ripple = 5 dB
  actual ripple = 23.215569233114024 dB

* 11 Hz - 50 Hz
  gain = 0
  desired attenuation = -40 dB
  actual attenuation = -28.65474091653491 dB

*/

#define TAPS_NUM 16

static double filterTaps[TAPS_NUM] = {
  0.0009503977575911028,
  0.04249140894756561,
  0.06299075175271135,
  0.09912917122586677,
  0.1377186027260431,
  0.17370622809640995,
  0.2015808426207485,
  0.2168344910820332,
  0.2168344910820332,
  0.2015808426207485,
  0.17370622809640995,
  0.1377186027260431,
  0.09912917122586677,
  0.06299075175271135,
  0.04249140894756561,
  0.0009503977575911028
};

void initFIR_Filter(FIR_Filter_t *filter)
{
	for (int i = 0;i<TAPS_NUM;i++)
	{
		filter->buffer[i] = 0;
	}
	filter->output = 0;
	filter->index = 0;
	filter->taps = filterTaps;
}


void updateFIR_Filter(FIR_Filter_t *filter, float input)
{
    filter->output = 0;
	if (filter->index == TAPS_NUM) { filter->index = 0; }
	filter->buffer[filter->index] = input;
	for (int i = 0;i<TAPS_NUM;i++)
	{
		filter->output += filter->buffer[(filter->index + i) % TAPS_NUM] * filterTaps[i];
	}
	filter->index++;
}
