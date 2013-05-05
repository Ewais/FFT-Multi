#include "../../inlcude/Comp/T2_FFT_Comp.h"

//#define DEBUG

pthread_t Thread[THREAD_NUM];
pthread_barrier_t Start;
pthread_rwlock_t DebugLock;

void T2_FFT_Comp(void)
{
    int Return;
    T2_FFT_Comp_Input ThreadInput;
    Return = pthread_rwlock_init(&DebugLock,NULL);
    if (Return)
    {
        cout << endl << "Error while creating lock ";
    }
    pthread_barrier_init(&Start,NULL,THREAD_NUM);
    for(int i = 0;i < THREAD_NUM;i++)
    {
        ThreadInput.Start =  i*ThreadDataSize;
        ThreadInput.ThreadNum = i;
        Return = pthread_create(&Thread[i],NULL,T2_FFT_Comp_ThreadFn,(void *)&ThreadInput);
#ifdef DEBUG
        pthread_rwlock_wrlock(&DebugLock);
        cout << endl << "creating thread number " << i <<" with "<< Decomposed[ThreadInput.Start];
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
        cout << endl << "Error while joining thread Number : 0";
        pthread_rwlock_unlock(&DebugLock);
    }
    pthread_rwlock_destroy(&DebugLock);
    return;
}

void *T2_FFT_Comp_ThreadFn(void *input)
{
    int InputStart = ((T2_FFT_Comp_Input *)input)->Start;
    int ThreadID = ((T2_FFT_Comp_Input *)input)->ThreadNum;
    int Return;
#ifdef DEBUG
    pthread_rwlock_wrlock(&DebugLock);
    cout << endl << "Thread number : " << ThreadID << " created";
    pthread_rwlock_unlock(&DebugLock);
#endif
    pthread_barrier_wait(&Start);
#ifdef DEBUG
    pthread_rwlock_wrlock(&DebugLock);
    cout << endl << "Thread number : " << ThreadID << " started";
    pthread_rwlock_unlock(&DebugLock);
#endif
    NT_FFT_Comp(&(Decomposed[InputStart]),&(Spectrum[InputStart]),1,ThreadDataSize);
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
        NT_FFT_Comp(&(Spectrum[InputStart]),&(Spectrum[InputStart]),i/2,i);
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
    NT_FFT_Comp(&(Spectrum[InputStart]),&(Spectrum[InputStart]),SIZE/2,SIZE);
#ifdef DEBUG
    pthread_rwlock_wrlock(&DebugLock);
    cout << endl << "Exit Thread Number : " <<ThreadID;
    pthread_rwlock_unlock(&DebugLock);
#endif
    pthread_exit(NULL);
}
