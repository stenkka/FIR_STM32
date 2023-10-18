/*
 * FIR.h
 *
 *  Created on: Oct 18, 2023
 *      Author: aaron
 */

#ifndef SRC_FIR_H_
#define SRC_FIR_H_

#include "stdint.h"

#define TAPS_NUM 16

/*

FIR filter designed with
http://t-filter.appspot.com

sampling frequency: 2000 Hz

* 0 Hz - 400 Hz
  gain = 1
  desired ripple = 5 dB
  actual ripple = 6.258113938045559 dB

* 500 Hz - 1000 Hz
  gain = 0
  desired attenuation = -40 dB
  actual attenuation = -36.68592855270738 dB

*/

double filterTaps[TAPS_NUM] = {
  0.042153588198237606,
  0.09254487085124112,
  0.08627292857696542,
  -0.0066099899662500515,
  -0.09647274861311855,
  -0.03655279492291376,
  0.1889147108950072,
  0.4024647831036765,
  0.4024647831036765,
  0.1889147108950072,
  -0.03655279492291376,
  -0.09647274861311855,
  -0.0066099899662500515,
  0.08627292857696542,
  0.09254487085124112,
  0.042153588198237606
};

typedef struct {
	double buffer[TAPS_NUM];
	double output;
	double *taps;
	uint8_t index;
}FIR_Filter;

void initFIR_Filter(FIR_Filter *filter);
void updateFIR_Filter(FIR_Filter *filter, float input);

#endif /* SRC_FIR_H_ */
