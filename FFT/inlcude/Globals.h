#ifndef GLOBALS_H_INCLUDED
#define GLOBALS_H_INCLUDED

#include <complex>

#define SIZE 16              /// Size of time domain signal
#define THREAD_NUM 4    /// To be made arbitrary
#define pi 3.14159265359

using namespace std;

extern complex<float> Decomposed[SIZE];         /// Decomposed time domain signal
extern int Position;                             /// Keeps track of the "Decomposed" element that is being calculated

extern complex<float> Spectrum[SIZE];           /// Spectrum to be calculated
struct BFOut                              /// Holds the two outputs of the radix-2 butter fly
{
        complex<float> first;            /// First output
        complex<float> second;           /// Second output
};

#endif // GLOBALS_H_INCLUDED
