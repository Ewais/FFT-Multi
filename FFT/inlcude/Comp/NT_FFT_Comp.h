#ifndef NT_FFT_COMP_H_INCLUDED
#define NT_FFT_COMP_H_INCLUDED

#include "../Globals.h"

BFOut ButterFly(complex<float> first,complex<float> second,int ElementSize,int harmonic);
void NT_FFT_Comp(complex<float>* InArray,complex<float>* OutArray,int ElementSize,int ArraySize);
void NT_FFT_Comp(void);

#endif // NT_FFT_COMP_H_INCLUDED
