#include "../../inlcude/Decomp/NT_FFT_Decomp.h"

//extern complex<float> Decomposed[SIZE];         /// Decomposed time domain signal

void NT_FFT_Decomp (complex<float>* array,int size)
{
    /** This Function decomposes the time domain signal into it's even and odd compoents
    * Explaind more in the documentation
    * Inputs: array: signal to be decomposed
    *         size : size of input array, determine sthe level of recurrence
    *Outputs : saves the decomposed signal on the global variable "Decomposed"
    */
    if(size == 1)                            /// At the last level of recurrsion (size of inpu array = 1)
    {
        Decomposed[Position] = *array;   /// Save the decopsed signal
        Position++;
        return;
    }
    complex<float> *even = new complex<float> [size/2];   /// Define two arrays to hold even and odd samples of the signal
    complex<float> *odd = new complex<float> [size/2];
    for(int i = 0;i < size;i++)   /// Split the array into even and odd samples
    {
        if(i%2 == 0)
            even[i/2] = *(array + i);
        else
            odd[(i-1)/2] = *(array + i);
    }
    NT_FFT_Decomp (even , size/2);     /// Do recurrence on the even and odd arrays
    NT_FFT_Decomp (odd , size/2);
    delete even;
    delete odd;
}
