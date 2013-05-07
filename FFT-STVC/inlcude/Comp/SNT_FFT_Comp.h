#ifndef NT_FFT_COMP_H_INCLUDED
#define NT_FFT_COMP_H_INCLUDED

#include "../Globals.h"

BFOut S_ButterFly(complex<float> first,complex<float> second,int ElementSize,int harmonic);
void SNT_FFT_Comp(complex<float>* InArray,complex<float>* OutArray,int ElementSize,int ArraySize);
void SNT_FFT_Comp(void);

#endif // NT_FFT_COMP_H_INCLUDED
