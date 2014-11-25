#include <portaudio.h>
#include <stdio.h>
#include <fftw3.h>
#include <complex>


#define SAMPLE_RATE (44100)
#define NUM_SECONDS (1)


typedef struct
{
	float leftChannel[SAMPLE_RATE*NUM_SECONDS];
	float rightChannel[SAMPLE_RATE*NUM_SECONDS];
	unsigned long left_phase;
	unsigned long right_phase;
}   
paRecData;


static paRecData dataRec;


/* This routine will be called by the PortAudio engine when audio is needed.
It may called at interrupt level on some machines so don't do anything
that could mess up the system like calling malloc() or free().
*/ 
int patestRecCallback( const void *inputBuffer, void *outputBuffer,
							 unsigned long framesPerBuffer,
							 const PaStreamCallbackTimeInfo* timeInfo,
							 PaStreamCallbackFlags statusFlags,
							 void *userData );

int patestPlayCallback( const void *inputBuffer, void *outputBuffer,
							  unsigned long framesPerBuffer,
							  const PaStreamCallbackTimeInfo* timeInfo,
							  PaStreamCallbackFlags statusFlags,
							  void *userData );


int process_samples( paRecData data );

double get_max_module( fftw_complex *in, int nc );