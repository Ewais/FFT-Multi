#include "../../inlcude/Comp/CUDA_FFT_Comp.h"

__global__ void CUDA_ButterFly(cuFloatComplex* Spectrum_GPU,int Size)
{
    /** This function implements radix-2 butterfly (BF)
    * Executed by each thread in GPU
    * Inputs : Spectrum_GPU : Complex array in GPU memory
    *          Size : number of samples of the signal "N"
    * Outputs : saved in the same places in Spectrum_GPU
    */
	int Index = 2*Size*((int)((512*blockIdx.x + threadIdx.x)/Size))+((512*blockIdx.x + threadIdx.x)%Size);
	cuFloatComplex First = Spectrum_GPU[Index];
	cuFloatComplex Second = Spectrum_GPU[Index+Size];
	float Harmonic = -(2*Pi*Index)/(2*Size);
	cuFloatComplex Sin;
	Sin.x = cos(Harmonic);
	Sin.y = -sin(Harmonic);
	Spectrum_GPU[Index] = cuCaddf(First,cuCmulf(Second,Sin));
	Spectrum_GPU[Index+Size] = cuCsubf(First,cuCmulf(Second,Sin));
}

void CUDA_FFT_Comp(void)
{
    /** Implements composition of Signal in CUDA
    * uses radix-2 butterfly
    * Inputs:  None.
    * 	Input is global Array Decomposed
    * Output:  None.
    * 	Output is global array Spectrum
    */
	cuFloatComplex* Spectrum_GPU;
	cudaMalloc(&Spectrum_GPU,sizeof(Decomposed));
	cudaMemcpy(Spectrum_GPU,&Decomposed,sizeof(Decomposed),cudaMemcpyHostToDevice);
	for(int Size = 1;Size <SIZE;Size*=2)
		//CUDA_ButterFly<<<Blocks,MaxThread>>>(Spectrum_GPU,Size);
		CUDA_ButterFly<<<1,32>>>(Spectrum_GPU,Size);
	cudaMemcpy(&Spectrum,Spectrum_GPU,sizeof(Decomposed),cudaMemcpyDeviceToHost);
	cudaFree(Spectrum_GPU);
}
