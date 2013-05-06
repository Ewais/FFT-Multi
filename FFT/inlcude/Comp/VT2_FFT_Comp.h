#ifndef VT2_FFT_COMP_H_INCLUDED
#define VT2_FFT_COMP_H_INCLUDED

#include <pthread.h>
#include <iostream>
#include "../Globals.h"
#include "V_FFT_Comp.h"

#define ThreadDataSize SIZE/THREAD_NUM

struct VT2_FFT_Comp_Input
{
    int Start;
    int ThreadNum;
};

void VT2_FFT_Comp(void);
void *VT2_FFT_Comp_ThreadFn(void *input);

#endif // VT2_FFT_COMP_H_INCLUDED
