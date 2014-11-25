#include "my_portaudio.h"


/* This routine will be called by the PortAudio engine when audio is needed.
It may called at interrupt level on some machines so don't do anything
that could mess up the system like calling malloc() or free().
*/ 
int patestRecCallback( const void *inputBuffer, void *outputBuffer,
							 unsigned long framesPerBuffer,
							 const PaStreamCallbackTimeInfo* timeInfo,
							 PaStreamCallbackFlags statusFlags,
							 void *userData )
{
	/* Cast data passed through stream to our structure. */
	paRecData *data = (paRecData*)userData; 
	float *in = (float*)inputBuffer;

	for (int i = 0 ; i < framesPerBuffer ; ++ i)
	{
		if(data->left_phase < SAMPLE_RATE*NUM_SECONDS)
		{
			data->leftChannel[data->left_phase++] = *in++;
			data->rightChannel[data->right_phase++] = *in++;
		}

	}
	return 0;
}


int patestPlayCallback( const void *inputBuffer, void *outputBuffer,
							  unsigned long framesPerBuffer,
							  const PaStreamCallbackTimeInfo* timeInfo,
							  PaStreamCallbackFlags statusFlags,
							  void *userData )
{
	/* Cast data passed through stream to our structure. */
	paRecData *data = (paRecData*)userData; 
	float *out = (float*)outputBuffer;

	for(int i = 0 ; i < framesPerBuffer ; ++ i){
		if(data->left_phase < SAMPLE_RATE*NUM_SECONDS){
			*out++ = data->leftChannel[data->left_phase++];
			*out++ = data->rightChannel[data->right_phase++];
		}

	}
	return 0;
}


int policz_srednia( paRecData data )
{
	float wynik = 0.0f;
	for ( int i = 0; i < SAMPLE_RATE*NUM_SECONDS; i++ )
	{
		wynik += data.leftChannel[i];
	}
	wynik = wynik/(SAMPLE_RATE*NUM_SECONDS);

	printf("%f\n", wynik);

	return 0;
}
