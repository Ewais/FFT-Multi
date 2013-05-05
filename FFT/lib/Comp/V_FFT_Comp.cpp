#include "../../inlcude/Comp/V_FFT_Comp.h"

void V_FFT_Comp(void)
{
    V_FFT_Comp(Decomposed,Spectrum,1,SIZE);
}

void V_FFT_Comp(complex<float>* InArray,complex<float>* OutArray,int ElementSize,int ArraySize)
{
    VBFOut TempVBF;
    for (int i = 0;i < ArraySize;i+= 2*ElementSize)   /// Devide the input array according to the given stage
    {
        for (int j = i;j < ElementSize+i;j++)   /// For each devision apply the butterfly
        {
              //TempVBF = V_ButterFly(&real(*(InArray+j)),&imag(*(InArray+j)),&real(*(InArray+j+ElementSize)),&imag(*(InArray+j+ElementSize)),&(2*ElementSize),j);
              for(int k = 0;i < 4;i++)
              {

              }
//            float x = -(2*pi*harmonic)/size;          /// Compute the exponent of the sinisoud
//            complex<float> i (0,1);                   /// Define i (sqrt(-1))
//            complex<float> w;
//            w = exp(i*x);                              /// The sinusoide
//            cout << endl << "w = "<<w;
//            OutArray[j] = TempBF.first;                  /// Save the BF output on the spectrum
//            OutArray[j+ElementSize] = TempBF.second;
        }
    }
    V_FFT_Comp(OutArray,OutArray,2*ElementSize,ArraySize); /// Recurrence on the next stage
}

VBFOut V_ButterFly(float *FirstReal,float *FirstImag,float *SecondReal,float *SecondImag,int *Size,int *Harmonic)
{
    VBFOut Return;
    float *Pi;
    *Pi = (-2*pi);
    float Temp[4],ExpReal[4],ExpImag[4];
    __m128 InFirstRealV = _mm_load_ps(FirstReal);
    __m128 InFirstImagV = _mm_load_ps(FirstImag);
    __m128 InSecondRealV = _mm_load_ps(SecondReal);
    __m128 InSecondImagV = _mm_load_ps(SecondImag);
    __m128 HarmonicV = _mm_load1_ps((float *)Harmonic);
    __m128 SizeV = _mm_load1_ps((float *)Size);
    __m128 PiV = _mm_load1_ps(Pi);
    HarmonicV = _mm_mul_ps(PiV,HarmonicV);
    HarmonicV = _mm_div_ps(HarmonicV,SizeV);
    _mm_store_ps(&Temp[0],HarmonicV);
    for(int i = 0;i < 4;i++)
    {
        ExpReal[i] = cos(Temp[i]);
        ExpImag[i] = -sin(Temp[i]);
    }
    __m128 ExpRealV = _mm_load_ps(ExpReal);
    __m128 ExpImagV = _mm_load_ps(ExpImag);
    __m128 TempRealV = _mm_sub_ps(_mm_mul_ps(InSecondRealV,ExpRealV),_mm_mul_ps(InSecondImagV,ExpImagV));
    __m128 TempImagV = _mm_add_ps(_mm_mul_ps(InSecondImagV,ExpRealV),_mm_mul_ps(InSecondRealV,ExpImagV));
    __m128 OutFirstRealV = _mm_add_ps(TempRealV,InFirstRealV);
    __m128 OutFirstImagV = _mm_add_ps(TempImagV,InFirstImagV);
    __m128 OutSecondRealV = _mm_sub_ps(InFirstRealV,TempRealV);
    __m128 OutSecondImagV = _mm_sub_ps(InFirstImagV,TempImagV);
    _mm_store_ps(FirstReal,OutFirstRealV);
    _mm_store_ps(FirstImag,OutFirstImagV);
    _mm_store_ps(SecondReal,OutSecondRealV);
    _mm_store_ps(SecondImag,OutSecondImagV);
    Return.FirstReal = FirstReal;
    Return.FirstImag = FirstImag;
    Return.SecondReal = SecondReal;
    Return.SecondImag = SecondImag;
    return Return;
}
