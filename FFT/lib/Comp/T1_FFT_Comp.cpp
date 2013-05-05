#include "../../inlcude/Comp/T1_FFT_Comp.h"

void *ButterFly_Thread1(void* Input)
{
     BFInput input = *((BFInput *)Input);
     int index_first = input.index_first;
     int index_second = input.index_second;
     int size = input.size;
     int harmonic = input.harmonic;
     BFOut BF ;
     BF = ButterFly( Decomposed[index_first],Decomposed[index_second],size,harmonic);
     Decomposed[index_first]  = BF.first;
     Decomposed[index_second] = BF.second;
     pthread_exit(NULL);
}

void T1_FFT_Comp(complex<float>* array,int size)
{
    pthread_t thread_handle[THREAD_NUM];
    for (int i = 0; i < THREAD_NUM; i++)
    {
        BFInput *temp;
        int rc = pthread_create(&thread_handle[i],NULL,ButterFly_Thread1,(void *)temp);
        if (rc)
        {
            cout << endl << "Error while creating thread #" << i;
        }
    }
    pthread_exit(NULL);
}
