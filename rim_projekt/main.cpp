#include "my_portaudio.h"



int main()
{
	//inicjalizacja portAudio
	PaError err = Pa_Initialize();
	if( err != paNoError ) 
	{
		printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );
	}

	PaStream *stream;
	/* Open an audio I/O stream. */

	while (1) 
	{
		err = Pa_OpenDefaultStream( &stream,
			2,          /* no input channels */
			0,          /* stereo output */
			paFloat32,  /* 32 bit floating point output */
			SAMPLE_RATE,
			256,        /* frames per buffer, i.e. the number
						of sample frames that PortAudio will
						request from the callback. Many apps
						may want to use
						paFramesPerBufferUnspecified, which
						tells PortAudio to pick the best,
						possibly changing, buffer size.*/
						patestRecCallback, /* this is your callback function */
						&dataRec ); /*This is a pointer that will be passed to
									your callback*/
		if( err != paNoError )
		{
			printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );
		}

		err = Pa_StartStream( stream );
		if( err != paNoError ) 
			printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );

		printf("Nagrywanie.......\n");

		/* Sleep for several seconds. */
		Pa_Sleep(NUM_SECONDS*1000);
		err = Pa_StopStream( stream );
		if( err != paNoError )
			printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );

		printf("KONIEC NAGRYWANIA!!!!\n");

	
		dataRec.left_phase = 0;
		dataRec.right_phase = 0;

		process_samples ( dataRec );

	}
	
	err = Pa_Terminate();
		if( err != paNoError )
			printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );

	return 0;
}