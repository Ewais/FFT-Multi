#include "../../inlcude/Comp/ST_FFT_Comp.h"

//#define DEBUG

pthread_t Thread[THREAD_NUM];
pthread_barrier_t BarrierStart;
pthread_rwlock_t DebugLock;

void ST_FFT_Comp(void)
{
    /** Creates THREAD_NUM threads that implement Signal composition
    * THREAD_NUM can be found in Globals.h
    * Inputs:  None
    * Output:  None
    */
    int Return;
    ST_FFT_Comp_Input ThreadInput[THREAD_NUM];
    Return = pthread_rwlock_init(&DebugLock,NULL);
    if (Return)
    {
        cout << endl << "Error while creating lock ";
    }
    pthread_barrier_init(&BarrierStart,NULL,THREAD_NUM);
    for(int i = 0;i < THREAD_NUM;i++)
    {
        ThreadInput[i].Start =  i*ThreadDataSize;
        ThreadInput[i].ThreadNum = i;
        Return = pthread_create(&Thread[i],NULL,ST_FFT_Comp_ThreadFn,(void *)&ThreadInput[i]);
#ifdef DEBUG
        pthread_rwlock_wrlock(&DebugLock);
        cout << endl << "creating thread number " << i; /*<<" with "<< Decomposed[ThreadInput.Start];*/
        pthread_rwlock_unlock(&DebugLock);
#endif
        if (Return)
        {
            cout << endl << "Error while creating thread #" << i;
        }
    }
#ifdef DEBUG
    pthread_rwlock_wrlock(&DebugLock);
    cout << endl << "Joining Thread Number : 0";
    pthread_rwlock_unlock(&DebugLock);
#endif
    Return = pthread_join(Thread[0],NULL);
    if (Return)
    {
        pthread_rwlock_wrlock(&DebugLock);
        cout << endl << "Error while joining thread Number :  0 ";
        pthread_rwlock_unlock(&DebugLock);
    }
    pthread_rwlock_destroy(&DebugLock);
    return;
}

void *ST_FFT_Comp_ThreadFn(void *input)
{
    /** Implements composition of a ThreadDataSize part of the original Signal
    * Then every stage have threads terminate and leave their workloads for the other half
    * Executed by each thread
    * ThreadDataSize can be found in ST_FFT_Comp.h
    * uses SNT_FFT_Comp
    * Inputs:  ST_FFT_Comp_Input struct casted in void *
    * 				struct contains: Start : start of the thread's part of the signal
    * 								 ThreadNum : the thread ID
    * Output:  None
    */

    int InputStart = ((ST_FFT_Comp_Input *)input)->Start;
    int ThreadID = ((ST_FFT_Comp_Input *)input)->ThreadNum;
    int Return;
#ifdef DEBUG
    pthread_rwlock_wrlock(&DebugLock);
    cout << endl << "Thread number : " << ThreadID << " created";
    pthread_rwlock_unlock(&DebugLock);
#endif
    pthread_barrier_wait(&BarrierStart);
#ifdef DEBUG
    pthread_rwlock_wrlock(&DebugLock);
    cout << endl << "Thread number : " << ThreadID << " started";
    pthread_rwlock_unlock(&DebugLock);
#endif
    SNT_FFT_Comp(&(Decomposed[InputStart]),&(Spectrum[InputStart]),1,ThreadDataSize);
    if(ThreadID%2)
    {
#ifdef DEBUG
        pthread_rwlock_wrlock(&DebugLock);
        cout << endl << "Exit Thread Number : " <<ThreadID;
        pthread_rwlock_unlock(&DebugLock);
#endif
        pthread_exit(NULL);
    }
    else
    {
#ifdef DEBUG
        pthread_rwlock_wrlock(&DebugLock);
        cout << endl << "Joining Thread Number : " <<ThreadID + 1;
        pthread_rwlock_unlock(&DebugLock);
#endif
        Return = pthread_join(Thread[ThreadID+1],NULL);
        if (Return)
        {
            pthread_rwlock_wrlock(&DebugLock);
            cout << endl << "Error while joining thread Number : "<<ThreadID + 1;
            pthread_rwlock_unlock(&DebugLock);
        }
    }
    for(int i = ThreadDataSize*2,j = 2;i < SIZE;i*=2,j*=2)
    {
        SNT_FFT_Comp(&(Spectrum[InputStart]),&(Spectrum[InputStart]),i/2,i);
        if(ThreadID%(2*j))
        {
#ifdef DEBUG
            pthread_rwlock_wrlock(&DebugLock);
            cout << endl << "Exit Thread Number : " <<ThreadID;
            pthread_rwlock_unlock(&DebugLock);
#endif
            pthread_exit(NULL);
        }
        else
        {
#ifdef DEBUG
            pthread_rwlock_wrlock(&DebugLock);
            cout << endl << "Joining Thread Number : " <<ThreadID + j;
            pthread_rwlock_unlock(&DebugLock);
#endif
            Return = pthread_join(Thread[ThreadID+j],NULL);
            if (Return)
            {
                pthread_rwlock_wrlock(&DebugLock);
                cout << endl << "Error while joining thread Number : "<<ThreadID + j;
                pthread_rwlock_unlock(&DebugLock);
            }
        }
    }
    SNT_FFT_Comp(&(Spectrum[InputStart]),&(Spectrum[InputStart]),SIZE/2,SIZE);
#ifdef DEBUG
    pthread_rwlock_wrlock(&DebugLock);
    cout << endl << "Exit Thread Number : " <<ThreadID;
    pthread_rwlock_unlock(&DebugLock);
#endif
    pthread_exit(NULL);
}
