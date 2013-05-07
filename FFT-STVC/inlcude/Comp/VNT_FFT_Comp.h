#ifndef V_FFT_COMP_H_INCLUDED
#define V_FFT_COMP_H_INCLUDED

#include <immintrin.h>
#include "../Globals.h"

struct VBFOut                              /// Holds the complex outputs of the radix-2 butter fly
{
        float *FirstReal;
        float *FirstImag;
        float *SecondReal;
        float *SecondImag;
};

void VNT_FFT_Comp(void);
void VNT_FFT_Comp(complex<float>* InArray,complex<float>* OutArray,int ElementSize,int ArraySize);
VBFOut V_ButterFly(float *FirstReal,float *FirstImag,float *SecondReal,float *SecondImag,int *Size,int *Harmonic);

#endif // V_FFT_COMP_H_INCLUDED
