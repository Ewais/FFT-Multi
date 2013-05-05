#include <iostream>
#include <fstream>
#include "../inlcude/FFT.h"

//#define DecompDEBUG
//#define CompDEBUG

int main()
{
#ifdef CompDEBUG || DecompDEBUG
    complex<float> NT[SIZE];
    int x;
#endif // CompDEBUG
        //complex<float> signal[SIZE] = {0,1,2,3,4,5,6,7};         // Debugging input
        //complex<float> signal[SIZE] = {0,1,1,1,1,1,1,0};     // Debugging input
        complex<float> signal[SIZE] = {0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0};      /// Time domain signal
//        complex<float> signal[SIZE];
//        ifstream InputSignalFile;
//        InputSignalFile.open("Input.txt");
//        for(int i = 0;i<SIZE;i++)
//        {
//            InputSignalFile >> signal[i];
//        }
//        InputSignalFile.close();
        Position[0] = 0;
        NT_FFT_Decomp (signal, SIZE , 0);                     /// Decompose the signal
#ifndef DecompDEBUG
        cout << "Non threaded decomposed : "<<endl;
        for(int i = 0;i < SIZE;i++)
        {
                cout << Decomposed[i] << ' ';
        }
#endif
#ifdef DecompDEBUG
        for (int i = 0;i < SIZE;i++)
            NT[i] = Decomposed[i];
#endif
        for(int i = 0;i < THREAD_NUM;i++)
        {
            Position[i] = 0;
        }
        T1_FFT_Decomp (signal, SIZE);                     /// Decompose the signal
#ifndef DecompDEBUG
        cout << endl << "Threaded decomposed : "<<endl;
        for(int i = 0;i < SIZE;i++)
        {
                cout << Decomposed[i] << ' ';
        }
#endif
#ifdef DecompDEBUG
        x = 0;
        for (int i = 0;i < SIZE;i++)
            if (NT[i] == Decomposed[i])
                x++;
        if (x == SIZE)
            cout << endl << "TRUE";
#endif
        NT_FFT_Comp();                                  /// FFT the signal (singel thread)
#ifdef CompDEBUG
        for (int i = 0;i < SIZE;i++)
            NT[i] = Spectrum[i];
#endif
#ifndef CompDEBUG
        cout << endl <<endl << "Non Threaded Spectrum : " << endl;   /// Print the spectrum
        for(int i = 0;i < SIZE;i++)
        {
              cout << real(Spectrum[i]) << "   " ;
        }
#endif
        T2_FFT_Comp();
#ifndef CompDEBUG
        cout << endl <<endl << "Threaded T2 Spectrum : " << endl;   /// Print the spectrum
        for(int i = 0;i < SIZE;i++)
        {
              cout << real(Spectrum[i]) << "   " ;
        }
#endif
#ifdef CompDEBUG
        x = 0;
        for (int i = 0;i < SIZE;i++)
            if (NT[i] == Spectrum[i])
                x++;
        if (x == SIZE)
            cout << endl << "TRUE2";
#endif
//        V_FFT_Comp();
//        cout << endl <<endl << "NonThreaded Vector Spectrum : " << endl;   /// Print the spectrum
//        for(int i = 0;i < SIZE;i++)
//        {
//              cout << real(Spectrum[i]) << "   " ;
//        }
        return 0;
}


