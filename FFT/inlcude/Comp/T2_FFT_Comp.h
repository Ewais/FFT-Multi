#ifndef T2_FFT_COMP_H_INCLUDED
#define T2_FFT_COMP_H_INCLUDED

#include <pthread.h>
#include <iostream>
#include "../Globals.h"
#include "NT_FFT_Comp.h"

#define ThreadDataSize SIZE/THREAD_NUM

struct T2_FFT_Comp_Input
{
    int Start;
    int ThreadNum;
};

void T2_FFT_Comp(void);
void *T2_FFT_Comp_ThreadFn(void *input);

#endif // T2_FFT_COMP_H_INCLUDED
