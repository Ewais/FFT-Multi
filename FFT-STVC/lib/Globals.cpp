#include "../inlcude/Globals.h"

complex<float> Decomposed[SIZE];         /// Decomposed time domain signal
int Position[THREAD_NUM];                             /// Keeps track of the "Decomposed" element that is being calculated

complex<float> Spectrum[SIZE];           /// Spectrum to be calculated
