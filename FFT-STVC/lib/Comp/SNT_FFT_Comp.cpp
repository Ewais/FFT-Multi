#include "../../inlcude/Comp/SNT_FFT_Comp.h"

//#define DEBUG
#ifdef DEBUG
    #include <iostream>
	using namespace std;
#endif

void SNT_FFT_Comp(void)
{
    /** Alias for SNT_FFT_Comp(complex<float>* InArray,complex<float>* OutArray,int ElementSize,int ArraySize)
    * Inputs:  None
    * Output:  None
    */
    SNT_FFT_Comp(Decomposed,Spectrum,1,SIZE);
}

void SNT_FFT_Comp(complex<float>* InArray,complex<float>* OutArray,int ElementSize,int ArraySize)
{
    /** Implements composition of Signal in a single thread
    * uses radix-2 butterfly
    * Inputs:  InArray: signal to be analyzed
    * 		   OutArray : Array to write the output
    *          ElementSize : Determines the current stage of the recurrence
    *          ArraySize : the length of the required signal
    * Output:  Spectrum is saved on OutArray
    */
    if(ElementSize == ArraySize)                     /// Last stage of recursion
        return;
    BFOut TempBF;                                    /// Define a butterfly element
        for (int i = 0;i < ArraySize;i+= 2*ElementSize)   /// Divide the input array according to the given stage
        {
            for (int j = i;j < ElementSize+i;j++)   /// For each devision apply the butterfly
            {
                TempBF = S_ButterFly(*(InArray+j),*(InArray+j+ElementSize),2*ElementSize,j);  /// Copmute the butterfly of 2 elelments (the position of these elemnts can be verified from the diagram attached)
                OutArray[j] = TempBF.first;                  /// Save the BF output on the spectrum
                OutArray[j+ElementSize] = TempBF.second;
            }
        }
#ifdef DEBUG
        cout << endl << "temp :" << endl;
        for(int i = 0;i < ArraySize;i++)
        {
                cout << real(OutArray[i]) << ' ';
        }
#endif
        SNT_FFT_Comp(OutArray,OutArray,2*ElementSize,ArraySize); /// Recurrence on the next stage
}

BFOut S_ButterFly(complex<float> first,complex<float> second,int size,int harmonic)
{
    /** This function implements radix-2 butterfly (BF)
    * Inputs : first, second : the two complex numbers to be butterflied
    *          size: number of samples of the signal "N"
    *          harmonic: index of the frequency domain "K"
    * Outputs : first and second outputs are saved in the BFOut struct.
    */
        BFOut Temp;                                /// Define the a BF element
        float x = -(2*Pi*harmonic)/size;          /// Compute the exponent of the sinosoid
        complex<float> i (0,1);                   /// Define i (sqrt(-1))
        complex<float> w;
        w = exp(i*x);                              /// The sinusoide
        Temp.first = first + second*w;             /// First output of the BF
        Temp.second = first - second*w;            /// Second output of the BF
        return Temp;
}
