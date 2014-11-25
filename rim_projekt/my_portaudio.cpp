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


int process_samples( paRecData data )
{
	//alokacja tablicy wejsciowej dla fft
	double *fft_input = NULL;
	int nc = 0;
	fftw_complex *fft_out = NULL;
	fftw_plan plan_forward;

	fft_input = (double*) fftw_malloc( sizeof ( double ) * SAMPLE_RATE*NUM_SECONDS );

	//kopiowanie danych dla fft
	for ( int i = 0; i < SAMPLE_RATE*NUM_SECONDS; i++ )
	{
		fft_input[i] = data.leftChannel[i];
	}

	//alokacja pamieci dla wyniku fft
	nc = ( SAMPLE_RATE*NUM_SECONDS / 2) + 1;
	fft_out = ( fftw_complex* )fftw_malloc( sizeof( fftw_complex ) * nc );

	//wyznaczenie planu oblicznia fft oraz obliczenie fft
	plan_forward = fftw_plan_dft_r2c_1d( SAMPLE_RATE*NUM_SECONDS, fft_input, fft_out, FFTW_ESTIMATE );

    fftw_execute ( plan_forward );

	double wynik = get_max_module( fft_out, nc );

	printf("%f\n", wynik );

	//dealokacja pamieci
	fftw_free( fft_input );
	fftw_free( fft_out );



	return 0;
}


double get_max_module( fftw_complex *in, int nc )
{
	double *modules = NULL;

	modules = (double*) malloc( sizeof(double) * nc);

	double wynik = 0;
	//obliczanie modulow dla kazdego punktu z obliczen fft
	for ( int i = 0; i < nc; i++ )
	{
		modules[i] = in[i][0]*in[i][0] + in[i][1]*in[i][1];

		if ( wynik < modules[i] )
		{
			wynik = modules[i];
		}
	}

	free(modules);

	return wynik;
}