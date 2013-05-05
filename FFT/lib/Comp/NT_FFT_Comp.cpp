#include "../../inlcude/Comp/NT_FFT_Comp.h"

//#define DEBUG
#include <iostream>
#ifdef DEBUG
    #include <iostream>
#endif

void NT_FFT_Comp(void)
{
    NT_FFT_Comp(Decomposed,Spectrum,1,SIZE);
}

void NT_FFT_Comp(complex<float>* InArray,complex<float>* OutArray,int ElementSize,int ArraySize)
{
    /** This Function is the main FFT algorithm
    * Implimented as a single thread
    * uses radix-2 butterfly
    * Inputs:  array: signal to be analyzed
    *          ElementSize: Determines the current stage of the reccurrence
    * Output:  Spectrum is saved on a global variable
    * Diagram for the algorithm can be found in the documentation
    */
    if(ElementSize == ArraySize)                     /// Last stage of reccursion
        return;
    BFOut TempBF;                                    /// Define a butterfly element
        for (int i = 0;i < ArraySize;i+= 2*ElementSize)   /// Devide the input array according to the given stage
        {
            for (int j = i;j < ElementSize+i;j++)   /// For each devision apply the butterfly
            {
                TempBF = ButterFly(*(InArray+j),*(InArray+j+ElementSize),2*ElementSize,j);  /// Copmute the butterfly of 2 elelments (the position of these elemnts can be verified from the diagram attached)
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
        NT_FFT_Comp(OutArray,OutArray,2*ElementSize,ArraySize); /// Recurrence on the next stage
}

BFOut ButterFly(complex<float> first,complex<float> second,int size,int harmonic)
{
    /** This function impliments radix-2 butterfly (BF)
    * BF operation can be found in the documentation
    * Inputs : first, second : the two comples numbers to be butterflied
    *          size: number of samples of the signal "N"
    *          harmonic: index of the frequency domain "K"
    * Outputs : fisrt and second outputs are saved in the BF stuct.
    */
        BFOut Temp;                                /// Definr the a BF element
        float x = -(2*pi*harmonic)/size;          /// Compute the exponent of the sinisoud
        complex<float> i (0,1);                   /// Define i (sqrt(-1))
        complex<float> w;
        w = exp(i*x);                              /// The sinusoide
        Temp.first = first + second*w;             /// First output of the BF
        Temp.second = first - second*w;            /// Second output of the BF
        return Temp;
}
