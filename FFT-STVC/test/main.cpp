#include <iostream>
#include <fstream>
#include "../inlcude/FFT.h"

//#define FILE

int main()
{
//We start by taking the input signal .. as a File or Predefined
#ifdef FILE
        complex<float> signal[SIZE];
        ifstream InputSignalFile;
        InputSignalFile.open("Input.txt");
        for(int i = 0;i<SIZE;i++)
        {
            InputSignalFile >> signal[i];
        }
        InputSignalFile.close();
#endif
#ifndef FILE
        complex<float> signal[SIZE] = {0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0};
#endif


//////////////////////////////////////////////////////////////////////////////////////////////
//The signal is decomposed																	//
//this happens in two ways:																	//
//		1-Non threaded																		//
//		2-Threaded																			//
//the out put is only printed out when using the small predefined signal					//
//////////////////////////////////////////////////////////////////////////////////////////////
        Position[0] = 0;
        NT_FFT_Decomp (signal, SIZE , 0);                     /// Decompose the signal
#ifndef FILE
        cout << "Non threaded decomposed : "<<endl;
        for(int i = 0;i < SIZE;i++)
        {
                cout << Decomposed[i] << ' ';
        }
#endif

        for(int i = 0;i < THREAD_NUM;i++)
        {
            Position[i] = 0;
        }
        T_FFT_Decomp (signal, SIZE);                     /// Decompose the signal
#ifndef FILE
        cout << endl << "Threaded decomposed : "<<endl;
        for(int i = 0;i < SIZE;i++)
        {
                cout << Decomposed[i] << ' ';
        }
#endif


//////////////////////////////////////////////////////////////////////////////////////////////
//The decomposed signal is then composed in Frequency domain using butterflies				//
//this happens in 5 ways :																	//
//		1-Non threaded																		//
//		2-Threaded																			//
//		3-Using Vector extensions (SSE)														//
//		4-Threaded with SSE																	//
//		5-CUDA																				//
//////////////////////////////////////////////////////////////////////////////////////////////

        SNT_FFT_Comp();                                  /// FFT the signal (single thread)
#ifndef FILE
        cout << endl <<endl << "Non Threaded Spectrum : " << endl;   /// Print the spectrum
        for(int i = 0;i < SIZE;i++)
        {
              cout << real(Spectrum[i]) << "   " ;
        }
#endif
#ifdef FILE
        ofstream SNT_File;
        SNT_File.open("SNT.txt");
        for(int i = 0;i<SIZE;i++)
        {
            SNT_File << real(Spectrum[i]) << '\t';
        }
        SNT_File.close();
#endif
        ST_FFT_Comp();
#ifndef FILE
        cout << endl <<endl << "Threaded Spectrum : " << endl;   /// Print the spectrum
        for(int i = 0;i < SIZE;i++)
        {
              cout << real(Spectrum[i]) << "   " ;
        }
#endif
#ifdef FILE
        ofstream ST_File;
        ST_File.open("ST.txt");
        for(int i = 0;i<SIZE;i++)
        {
            ST_File << real(Spectrum[i]) << '\t';
        }
        ST_File.close();
#endif
        VNT_FFT_Comp();
#ifndef FILE
        cout << endl <<endl << "NonThreaded Vector Spectrum : " << endl;   /// Print the spectrum
        for(int i = 0;i < SIZE;i++)
        {
              cout << real(Spectrum[i]) << "   " ;
        }
#endif
#ifdef FILE
        ofstream VNT_File;
        VNT_File.open("VNT.txt");
        for(int i = 0;i<SIZE;i++)
        {
            VNT_File << real(Spectrum[i]) << '\t';
        }
        VNT_File.close();
#endif
        VT_FFT_Comp();
#ifndef FILE
                cout << endl <<endl << "Threaded Vector Spectrum : " << endl;   /// Print the spectrum
                for(int i = 0;i < SIZE;i++)
                {
                      cout << real(Spectrum[i]) << "   " ;
                }
#endif
#ifdef FILE
        ofstream VT_File;
        VT_File.open("VT.txt");
        for(int i = 0;i<SIZE;i++)
        {
            VT_File << real(Spectrum[i]) << '\t';
        }
        VT_File.close();
#endif
        CUDA_FFT_Comp();
#ifndef FILE
        cout << endl << endl << "CUDA Spectrum : " << endl;   /// Print the spectrum
        for(int i = 0;i < SIZE;i++)
        {
          	cout << real(Spectrum[i]) << "   " ;
        }
#endif
#ifdef FILE
        ofstream CUDA_File;
        CUDA_File.open("CUDA.txt");
        for(int i = 0;i<SIZE;i++)
        {
            CUDA_File << real(Spectrum[i]) << '\t';
        }
        CUDA_File.close();
#endif
        return 0;
}


