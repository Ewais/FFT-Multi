/*
 * CUDA_FFT_Comp.h
 *
 *  Created on: May 7, 2013
 *      Author: wiso
 */

#ifndef CUDA_FFT_COMP_H_
#define CUDA_FFT_COMP_H_

#include <cuda.h>
#include <cuComplex.h>
#include "../Globals.h"

#define MaxThread 512
#define Blocks SIZE/1024

void CUDA_FFT_Comp(void);

#endif /* CUDA_FFT_COMP_H_ */
