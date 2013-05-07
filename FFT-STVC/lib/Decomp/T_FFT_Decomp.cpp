#include "../../inlcude/Decomp/T_FFT_Decomp.h"

//#define ThreadDEBUG
//#define DataDEBUG
//#define BAR

pthread_t Threads[THREAD_NUM];
pthread_rwlock_t Lock;
#ifdef BAR
pthread_barrier_t ThreadStart;
#endif

void T_FFT_Decomp (complex<float>* array,int size)
{
    int Return;
    Return = pthread_rwlock_init(&Lock,NULL);
    if (Return)
    {
        cout << endl << "Error while creating lock ";
    }
#ifdef BAR
    Return = pthread_barrier_init(&ThreadStart,NULL,THREAD_NUM+1);
    if (Return)
    {
        cout << endl << "Error while creating lock ";
    }
#endif
    complex<float> *even = new complex<float> [size/2];   /// Define two arrays to hold even and odd samples of the signal
    complex<float> *odd = new complex<float> [size/2];
    for(int i = 0;i < size;i++)   /// Split the array into even and odd samples
    {
        if(i%2 == 0)
            even[i/2] = *(array + i);
        else
            odd[(i-1)/2] = *(array + i);
    }
    T_FFT_Decomp_Input In1,In2;
    In1 = {even,size/2,THREAD_NUM/2,0};
    In2 = {odd,size/2,THREAD_NUM/2,THREAD_NUM/2};
    Return = pthread_create(&Threads[0],NULL,T_FFT_Decomp_ThreadFn,(void *) &In1);
    if (Return)
    {
        cout << endl << "Error while creating thread #0";
    }
    Return = pthread_create(&Threads[THREAD_NUM/2],NULL,T_FFT_Decomp_ThreadFn,(void *) &In2);
    if (Return)
    {
        cout << endl << "Error while creating thread #" << THREAD_NUM/2;
    }
#ifdef BAR
    pthread_barrier_wait(&ThreadStart);
#endif
    for(int i = 0;i < THREAD_NUM;i++)
    {
#ifdef ThreadDEBUG
        pthread_rwlock_wrlock(&Lock);
        cout << endl << "Joining Thread Number : " <<i;
        pthread_rwlock_unlock(&Lock);
#endif
        pthread_join(Threads[i],NULL);
    }
//    delete even;
//    delete odd;
}

void *T_FFT_Decomp_ThreadFn (void *input)
{
    /** This Function decomposes the time domain signal into it's even and odd compoents
    * Explaind more in the documentation
    * Inputs: array: signal to be decomposed
    *         size : size of input array, determine sthe level of recurrence
    *Outputs : saves the decomposed signal on the global variable "Decomposed"
    */
    complex<float> *array = ((T_FFT_Decomp_Input *)input)->Array;
    int size = ((T_FFT_Decomp_Input *)input)->Size;
    int ThreadSize = ((T_FFT_Decomp_Input *)input)->ThreadSize;
    int ThreadID = ((T_FFT_Decomp_Input *)input)->ThreadID;
    int Return;
#ifdef BAR
    pthread_barrier_wait(&ThreadStart);
#endif
#ifdef DataDEBUG
    pthread_rwlock_wrlock(&Lock);
    cout << endl << "Thread Number : " <<ThreadID<<" Created";
    cout << endl << "Array:";
    for (int i = 0;i < size;i++)
        cout << ' '<<array[i];
    pthread_rwlock_unlock(&Lock);
#endif
    complex<float> *even = new complex<float> [size/2];   /// Define two arrays to hold even and odd samples of the signal
    complex<float> *odd = new complex<float> [size/2];
    for(int i = 0;i < size;i++)   /// Split the array into even and odd samples
    {
        if(i%2 == 0)
            even[i/2] = *(array + i);
        else
            odd[(i-1)/2] = *(array + i);
    }
#ifdef ThreadDEBUG
    pthread_rwlock_wrlock(&Lock);
    cout << endl << "Thread Number : " <<ThreadID;
    cout << endl << "CheckPoint 1";
    pthread_rwlock_unlock(&Lock);
#endif
    delete array;
    if(ThreadSize > 1)
    {
#ifdef ThreadDEBUG
    pthread_rwlock_wrlock(&Lock);
    cout << endl << "Thread Number : " <<ThreadID;
    cout << endl << "CheckPoint 2";
    pthread_rwlock_unlock(&Lock);
#endif
        T_FFT_Decomp_Input In1,In2;
        In1 = {even,size/2,ThreadSize/2,ThreadID};
        In2 = {odd,size/2,ThreadSize/2,ThreadID+(ThreadSize/2)};
        Return = pthread_create(&Threads[ThreadID+(ThreadSize/2)],NULL,T_FFT_Decomp_ThreadFn,(void *) &In2);
        T_FFT_Decomp_ThreadFn((void *)&In1);
        if (Return)
        {
            cout << endl << "Error while creating thread #" << ThreadID+(ThreadSize/2);
        }
    }
    else
    {
#ifdef ThreadDEBUG
    pthread_rwlock_wrlock(&Lock);
    cout << endl << "Thread Number : " <<ThreadID;
    cout << endl << "CheckPoint 3";
    pthread_rwlock_unlock(&Lock);
#endif
        pthread_rwlock_wrlock(&Lock);
        NT_FFT_Decomp(even,size/2,ThreadID);
        NT_FFT_Decomp(odd,size/2,ThreadID);
        pthread_rwlock_unlock(&Lock);
        delete even;
        delete odd;
    }
//    delete even;
//    delete odd;
    pthread_exit(NULL);
}
