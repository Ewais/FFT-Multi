#include "../../inlcude/Comp/VT2_FFT_Comp.h"

//#define DEBUG

pthread_t ThreadV[THREAD_NUM];
pthread_barrier_t BarrierStartV;
pthread_rwlock_t DebugLockV;

void VT2_FFT_Comp(void)
{
    int Return;
    VT2_FFT_Comp_Input ThreadInput[THREAD_NUM];
    Return = pthread_rwlock_init(&DebugLockV,NULL);
    if (Return)
    {
        cout << endl << "Error while creating lock ";
    }
    pthread_barrier_init(&BarrierStartV,NULL,THREAD_NUM);
    for(int i = 0;i < THREAD_NUM;i++)
    {
        ThreadInput[i].Start =  i*ThreadDataSize;
        ThreadInput[i].ThreadNum = i;
        Return = pthread_create(&ThreadV[i],NULL,VT2_FFT_Comp_ThreadFn,(void *)&ThreadInput[i]);
#ifdef DEBUG
        pthread_rwlock_wrlock(&DebugLockV);
        cout << endl << "creating thread number " << i; /*<<" with "<< Decomposed[ThreadInput.Start];*/
        pthread_rwlock_unlock(&DebugLockV);
#endif
        if (Return)
        {
            cout << endl << "Error while creating thread #" << i;
        }
    }
#ifdef DEBUG
    pthread_rwlock_wrlock(&DebugLockV);
    cout << endl << "Joining Thread Number : 0";
    pthread_rwlock_unlock(&DebugLockV);
#endif
    Return = pthread_join(ThreadV[0],NULL);
    if (Return)
    {
        pthread_rwlock_wrlock(&DebugLockV);
        cout << endl << "Error while joining thread Number :  0 ";
        pthread_rwlock_unlock(&DebugLockV);
    }
    pthread_rwlock_destroy(&DebugLockV);
    return;
}

void *VT2_FFT_Comp_ThreadFn(void *input)
{
    int InputStart = ((VT2_FFT_Comp_Input *)input)->Start;
    int ThreadID = ((VT2_FFT_Comp_Input *)input)->ThreadNum;
    int Return;
#ifdef DEBUG
    pthread_rwlock_wrlock(&DebugLockV);
    cout << endl << "Thread number : " << ThreadID << " created";
    pthread_rwlock_unlock(&DebugLockV);
#endif
    pthread_barrier_wait(&BarrierStartV);
#ifdef DEBUG
    pthread_rwlock_wrlock(&DebugLockV);
    cout << endl << "Thread number : " << ThreadID << " started";
    pthread_rwlock_unlock(&DebugLockV);
#endif
    V_FFT_Comp(&(Decomposed[InputStart]),&(Spectrum[InputStart]),1,ThreadDataSize);
    if(ThreadID%2)
    {
#ifdef DEBUG
        pthread_rwlock_wrlock(&DebugLockV);
        cout << endl << "Exit Thread Number : " <<ThreadID;
        pthread_rwlock_unlock(&DebugLockV);
#endif
        pthread_exit(NULL);
    }
    else
    {
#ifdef DEBUG
        pthread_rwlock_wrlock(&DebugLockV);
        cout << endl << "Joining Thread Number : " <<ThreadID + 1;
        pthread_rwlock_unlock(&DebugLockV);
#endif
        Return = pthread_join(ThreadV[ThreadID+1],NULL);
        if (Return)
        {
            pthread_rwlock_wrlock(&DebugLockV);
            cout << endl << "Error while joining thread Number : "<<ThreadID + 1;
            pthread_rwlock_unlock(&DebugLockV);
        }
    }
    for(int i = ThreadDataSize*2,j = 2;i < SIZE;i*=2,j*=2)
    {
        V_FFT_Comp(&(Spectrum[InputStart]),&(Spectrum[InputStart]),i/2,i);
        if(ThreadID%(2*j))
        {
#ifdef DEBUG
            pthread_rwlock_wrlock(&DebugLockV);
            cout << endl << "Exit Thread Number : " <<ThreadID;
            pthread_rwlock_unlock(&DebugLockV);
#endif
            pthread_exit(NULL);
        }
        else
        {
#ifdef DEBUG
            pthread_rwlock_wrlock(&DebugLockV);
            cout << endl << "Joining Thread Number : " <<ThreadID + j;
            pthread_rwlock_unlock(&DebugLockV);
#endif
            Return = pthread_join(ThreadV[ThreadID+j],NULL);
            if (Return)
            {
                pthread_rwlock_wrlock(&DebugLockV);
                cout << endl << "Error while joining thread Number : "<<ThreadID + j;
                pthread_rwlock_unlock(&DebugLockV);
            }
        }
    }
    V_FFT_Comp(&(Spectrum[InputStart]),&(Spectrum[InputStart]),SIZE/2,SIZE);
#ifdef DEBUG
    pthread_rwlock_wrlock(&DebugLockV);
    cout << endl << "Exit Thread Number : " <<ThreadID;
    pthread_rwlock_unlock(&DebugLockV);
#endif
    pthread_exit(NULL);
}
