#include "../../inlcude/Comp/VNT_FFT_Comp.h"

void VNT_FFT_Comp(void)
{
    /** Alias for VNT_FFT_Comp(complex<float>* InArray,complex<float>* OutArray,int ElementSize,int ArraySize)
    * Inputs:  None
    * Output:  None
    */
    VNT_FFT_Comp(Decomposed,Spectrum,1,SIZE);
}

void VNT_FFT_Comp(complex<float>* InArray,complex<float>* OutArray,int ElementSize,int ArraySize)
{
    /** Implements composition of Signal in a single thread using Intel's SSE
    * uses 4 radix-2 butterflies
    * Inputs:  InArray: signal to be analyzed
    * 		   OutArray : Array to write the output
    *          ElementSize : Determines the current stage of the recurrence
    *          ArraySize : the length of the required signal
    * Output:  Spectrum is saved on OutArray
    */
    if(ElementSize == ArraySize)                     /// Last stage of reccursion
        return;
    if (ElementSize <= 4)
    {
        for(int i = 0;i < 4/ElementSize;i++)
        {
            for (int k = 0;k < ElementSize;k++)
            {

            }
        }
    }
    VNT_FFT_Comp(OutArray,OutArray,2*ElementSize,ArraySize); /// Recurrence on the next stage
}

VBFOut V_ButterFly(float *FirstReal,float *FirstImag,float *SecondReal,float *SecondImag,int *Size,int *Harmonic)
{
    /** This function implements 4 radix-2 butterflies
    * Inputs : FirstReal,FirstImag : two arrays including real and imaginary parts of 4 first elements
    * 		   SecondReal,SecondImag : two arrays including real and imaginary parts of 4 first elements
    * 		   Size: number of samples of the signal "N"
    *          Harmonic: index of the frequency domain "K"
    * Outputs : first and second outputs are saved in the VBFOut struct.
    */
    VBFOut Return;
    float *DoublePi;
    *DoublePi = (-2*Pi);
    float Temp[4],ExpReal[4],ExpImag[4];
    __m128 InFirstRealV = _mm_load_ps(FirstReal);
    __m128 InFirstImagV = _mm_load_ps(FirstImag);
    __m128 InSecondRealV = _mm_load_ps(SecondReal);
    __m128 InSecondImagV = _mm_load_ps(SecondImag);
    __m128 HarmonicV = _mm_load1_ps((float *)Harmonic);
    __m128 SizeV = _mm_load1_ps((float *)Size);
    __m128 PiV = _mm_load1_ps(DoublePi);
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
