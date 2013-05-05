#include "../../inlcude/Decomp/NT_FFT_Decomp.h"

//extern complex<float> Decomposed[SIZE];         /// Decomposed time domain signal
//#define DataDEBUG
//#define WriteDEBUG

#ifdef DataDEBUG
    #include <iostream>
    using namespace::std;
#endif // DataDEBUG
#ifdef WriteDEBUG
    #include <iostream>
    using namespace::std;
#endif // DataDEBUG

void NT_FFT_Decomp (complex<float>* array,int size,int ThreadNum)
{
    /** This Function decomposes the time domain signal into it's even and odd compoents
    * Explaind more in the documentation
    * Inputs: array: signal to be decomposed
    *         size : size of input array, determine sthe level of recurrence
    *Outputs : saves the decomposed signal on the global variable "Decomposed"
    */
#ifdef DataDEBUG
    cout << endl << "Thread Number : " <<ThreadNum;
    cout << endl << "Array:";
    for (int i = 0;i < size;i++)
        cout << ' '<<array[i];
#endif // ThreadDEBUG
    if(size == 1)                            /// At the last level of recurrsion (size of inpu array = 1)
    {
#ifdef WriteDEBUG
        cout << endl << "This is Thread Number = " << ThreadNum << " Writing on position : "<< Position[ThreadNum]+(ThreadNum*(SIZE/THREAD_NUM));
#endif // WriteDEBUG
        Decomposed[(Position[ThreadNum])+(ThreadNum*(SIZE/THREAD_NUM))] = *array;   /// Save the decomposed signal
        (Position[ThreadNum])++;
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
    NT_FFT_Decomp (even , size/2,ThreadNum);     /// Do recurrence on the even and odd arrays
    NT_FFT_Decomp (odd , size/2,ThreadNum);
    delete even;
    delete odd;
}
