#ifndef T1_FFT_DECOMP_H_INCLUDED
#define T1_FFT_DECOMP_H_INCLUDED

#include "../Globals.h"
#include <iostream>

struct T1_FFT_Decomp_Input
{
    complex<float>* Array;
    int Size;
};

void T1_FFT_Decomp (complex<float>* array,int size);
void *T1_FFT_Decomp_ThreadFn (void *input);

#endif // T1_FFT_DECOMP_H_INCLUDED
