/*
 * FIR.h
 *
 *  Created on: Oct 18, 2023
 *      Author: aaron
 */

#ifndef SRC_FIR_H_
#define SRC_FIR_H_

#include <stdint.h>

#define TAPS_NUM 16

typedef struct {
    uint8_t index;
    uint8_t order;
	double *buffer;
	double *taps;
    double output;
}FIR_Filter_t;

void initFIR_Filter(FIR_Filter_t *filter, const uint8_t order);
void updateFIR_Filter(FIR_Filter_t *filter, float input);

#endif /* SRC_FIR_H_ */
