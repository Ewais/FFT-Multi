#include "../../inlcude/Comp/VNT_FFT_Comp.h"

//#define DEBUG
#ifdef DEBUG
    #include <iostream>
    using namespace std;
#endif // DEBUG

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
    VBFOut TempVBF;
    int Index[4];
    float FirstNumReal[4];
    float FirstNumImag[4];
    float SecondNumReal[4];
    float SecondNumImag[4];
    complex<float> TempFirst[4];
    complex<float> TempSecond[4];
    int count = 0;
    for (int i = 0;i < ArraySize;i+= 2*ElementSize)   /// Divide the input array according to the given stage
    {
        for (int j = i;j < ElementSize+i;j++)   /// For each devision apply the butterfly
        {
            TempFirst[count] = *(InArray+j);									  /// Copmute the butterfly of 2 elelments (the position of these elemnts can be verified from the diagram attached)
            TempSecond[count] = *(InArray+j+ElementSize);
            Index[count] = j;
#ifdef DEBUG
    cout << endl << "J = "<<j;
    cout << "\tCount = " << count;
#endif // DEBUG
            count++;
            if(count == 4)
        	{
#ifdef DEBUG
    for(int q = 0;q < 4;q++)
    {
        cout << endl << "first = " << TempFirst[q];
        cout << endl << "Second = " << TempSecond[q];
    }
#endif // DEBUG
        		for(int k = 0;k < 4;k++)
        		{
        			FirstNumReal[k] = real(TempFirst[k]);
        			FirstNumImag[k] = imag(TempFirst[k]);
        			SecondNumReal[k] = real(TempSecond[k]);
        			SecondNumImag[k] = imag(TempSecond[k]);
        		}
                TempVBF = V_ButterFly(FirstNumReal,FirstNumImag,SecondNumReal,SecondNumImag,2*ElementSize,Index);
#ifdef DEBUG
    cout << endl << "Returned First Real = ";
    for(int x = 0; x < 4;x++)
    {
        cout <<  '\t' << TempVBF.FirstReal[x];
    }
    cout << endl << "Returned Second Real = ";
    for(int x = 0; x < 4;x++)
    {
        cout <<  '\t' << TempVBF.SecondReal[x];
    }
#endif // DEBUG
        		for(int g = 0;g < 4;g++)
        		{
#ifdef DEBUG
    cout << endl << "Index[g] = " << Index[g];
    cout << endl << "Index[g]+ElementSize = " << Index[g]+ElementSize;
#endif // DEBUG
                    OutArray[Index[g]].real() = TempVBF.FirstReal[g];
                    OutArray[Index[g]].imag() = TempVBF.FirstImag[g];
        			OutArray[Index[g]+ElementSize].real() = TempVBF.SecondReal[g];
        			OutArray[Index[g]+ElementSize].imag() = TempVBF.SecondImag[g];
                }
        		count = 0;
        	}
        }
    }
#ifdef DEBUG
        cout << endl << "temp :" << endl;
        for(int i = 0;i < ArraySize;i++)
        {
                cout << real(OutArray[i]) << ' ';
        }
#endif
    VNT_FFT_Comp(OutArray,OutArray,2*ElementSize,ArraySize); /// Recurrence on the next stage
}

VBFOut V_ButterFly(float *FirstReal,float *FirstImag,float *SecondReal,float *SecondImag,int Size,int *Harmonic)
{
    /** This function implements 4 radix-2 butterflies
    * Inputs : FirstReal,FirstImag : two arrays including real and imaginary parts of 4 first elements
    * 		   SecondReal,SecondImag : two arrays including real and imaginary parts of 4 first elements
    * 		   Size: number of samples of the signal "N"
    *          Harmonic: array of index of the frequency domain "K"
    * Outputs : first and second outputs are saved in the VBFOut struct.
    */
#ifdef DEBUG
    cout << endl << "Harmonic = ";
    for(int i = 0;i < 4;i++)
        cout <<endl<<Harmonic[i];
#endif // DEBUG
    VBFOut Return;
    float x = (-2*Pi);
    float *DoublePi = &x;
    float Temp[4],ExpReal[4],ExpImag[4];
    __m128 InFirstRealV = _mm_load_ps(FirstReal);
    __m128 InFirstImagV = _mm_load_ps(FirstImag);
    __m128 InSecondRealV = _mm_load_ps(SecondReal);
    __m128 InSecondImagV = _mm_load_ps(SecondImag);
    float _Harmonic[4];
    for(int i = 0;i < 4;i++)
        _Harmonic[i] = Harmonic[i];
    __m128 HarmonicV = _mm_load_ps(_Harmonic);
    float _Size = Size;
    __m128 SizeV = _mm_load1_ps(&_Size);
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
