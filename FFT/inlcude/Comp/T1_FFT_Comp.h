#ifndef T1_FFT_COMP_H_INCLUDED
#define T1_FFT_COMP_H_INCLUDED

#include <iostream>
#include <pthread.h>
#include "../Globals.h"
#include "NT_FFT_Comp.h"

struct BFInput
{
        int size;
        int harmonic;
        int index_first;   /// Index of inputs and outputs of the BF
        int index_second;  ///  ///
};

void *ButterFly_Thread1(void* input);
void T1_FFT_Comp(complex<float>* array,int size);

#endif // T1_FFT_COMP_H_INCLUDED
