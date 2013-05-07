#include "../../inlcude/Comp/VT_FFT_Comp.h"

//#define DEBUG

pthread_t ThreadV[THREAD_NUM];
pthread_barrier_t BarrierStartV;
pthread_rwlock_t DebugLockV;

void VT_FFT_Comp(void)
{
    /** Creates THREAD_NUM threads that implement Signal composition in SSE
    * THREAD_NUM can be found in Globals.h
    * Inputs:  None
    * Output:  None
    */
    int Return;
    VT_FFT_Comp_Input ThreadInput[THREAD_NUM];
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
        Return = pthread_create(&ThreadV[i],NULL,VT_FFT_Comp_ThreadFn,(void *)&ThreadInput[i]);
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

void *VT_FFT_Comp_ThreadFn(void *input)
{
    /** Implements composition of a ThreadDataSize part of the original Signal using SSE
    * Then every stage have threads terminate and leave their workloads for the other half
    * Executed by each thread
    * ThreadDataSize can be found in VT_FFT_Comp.h
    * uses VNT_FFT_Comp
    * Inputs:  VT_FFT_Comp_Input struct casted in void *
    * 				struct contains: Start : start of the thread's part of the signal
    * 								 ThreadNum : the thread ID
    * Output:  None
    */
    int InputStart = ((VT_FFT_Comp_Input *)input)->Start;
    int ThreadID = ((VT_FFT_Comp_Input *)input)->ThreadNum;
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
    VNT_FFT_Comp(&(Decomposed[InputStart]),&(Spectrum[InputStart]),1,ThreadDataSize);
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
        VNT_FFT_Comp(&(Spectrum[InputStart]),&(Spectrum[InputStart]),i/2,i);
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
    VNT_FFT_Comp(&(Spectrum[InputStart]),&(Spectrum[InputStart]),SIZE/2,SIZE);
#ifdef DEBUG
    pthread_rwlock_wrlock(&DebugLockV);
    cout << endl << "Exit Thread Number : " <<ThreadID;
    pthread_rwlock_unlock(&DebugLockV);
#endif
    pthread_exit(NULL);
}
