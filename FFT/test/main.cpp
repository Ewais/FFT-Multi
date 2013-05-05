/*
 * FFT_Sequential_V2.cxx  -- Working
 * FFT sequential algorithm done on 18-4-2013
 * Gets the spectrum of a time domain sognal defined in the main function
 * Only accepts signals of length 2^n (n integer)
 * Algorithm based on radix-2 butter fly
 * Each Thread is only 1 BF, number of threads is contant across stages
 *
 */

#include <iostream>
#include "../inlcude/FFT.h"

int main()
{
        //complex<float> signal[SIZE] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};         // Debugging input
        //complex<float> signal[SIZE] = {0,1,1,1,1,1,1,0};     // Debugging input
        complex<float> signal[SIZE] = {0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0};      /// Time domain signal
        Position = 0;
        NT_FFT_Decomp (signal, SIZE);                     /// Decompose the signal
        // Debugging prints
        cout << "decomposed : "<<endl;
        for(int i = 0;i < SIZE;i++)
        {
                cout << Decomposed[i] << ' ';
        }
        //NT_FFT_Comp();                                  /// FFT the signal (singel thread)
        T2_FFT_Comp();
        cout << endl <<endl << "Spectrum : " << endl;   /// Print the spectrum
        for(int i = 0;i < SIZE;i++)
        {
              cout << real(Spectrum[i]) << "   " ;
        }
        return 0;
}
