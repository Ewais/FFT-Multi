#include "../../inlcude/Decomp/T1_FFT_Decomp.h"

pthread_t Threads[THREAD_NUM];
pthread_rwlock_t Lock;

void T1_FFT_Decomp (complex<float>* array,int size)
{
    int Return;
    Return = pthread_rwlock_init(&Lock,NULL);
    if (Return)
    {
        cout << endl << "Error while creating lock ";
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
    T1_FFT_Decomp_Input In1,In2;
    In1 = {even,size/2};
    In2 = {odd,size/2};
    Return = pthread_create(&Threads[0],NULL,T1_FFT_Decomp_ThreadFn,(void *) &In1);
    if (Return)
    {
        cout << endl << "Error while creating thread #0";
    }
    Return = pthread_create(&Threads[THREAD_NUM/2],NULL,T1_FFT_Decomp_ThreadFn,(void *) &In2);
    if (Return)
    {
        cout << endl << "Error while creating thread #" << THREAD_NUM/2;
    }
    delete even;
    delete odd;
}

void *T1_FFT_Decomp_ThreadFn (void *input)
{
    /** This Function decomposes the time domain signal into it's even and odd compoents
    * Explaind more in the documentation
    * Inputs: array: signal to be decomposed
    *         size : size of input array, determine sthe level of recurrence
    *Outputs : saves the decomposed signal on the global variable "Decomposed"
    */
    complex<float> *array = ((T1_FFT_Decomp_Input *)input)->Array;
    int size = ((T1_FFT_Decomp_Input *)input)->Size;
    complex<float> *even = new complex<float> [size/2];   /// Define two arrays to hold even and odd samples of the signal
    complex<float> *odd = new complex<float> [size/2];
    for(int i = 0;i < size;i++)   /// Split the array into even and odd samples
    {
        if(i%2 == 0)
            even[i/2] = *(array + i);
        else
            odd[(i-1)/2] = *(array + i);
    }
//    T1_FFT_Decomp_ThreadFn(even , size/2);     /// Do recurrence on the even arrays
//    NT_FFT_Decomp (odd , size/2);
//    delete even;
//    delete odd;
}
