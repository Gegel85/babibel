#include "Sound.hpp"

Sound::Sound()
{
  data.maxFrameIndex = totalFrames = NUM_SECONDS * SAMPLE_RATE; /* Record for a few seconds. */
  data.frameIndex = 0;
  numSamples = totalFrames * NUM_CHANNELS;
  numBytes = numSamples * sizeof(SAMPLE);
  data.recordedSamples = (SAMPLE *) malloc( numBytes ); /* From now on, recordedSamples is initialised. */
  if( data.recordedSamples == NULL )
  {
      std::cout << "Could not allocate record array.\n" << std::endl;
      terminate();
  }
  for( i=0; i<numSamples; i++ ) data.recordedSamples[i] = 0;

  err = Pa_Initialize();
  if( err != paNoError ) terminate();

  inputParameters.device = Pa_GetDefaultInputDevice(); /* default input device */
  if (inputParameters.device == paNoDevice) {
      std::cout << "Error: No default input device.\n" << std::endl;
      terminate();
  }
  inputParameters.channelCount = 2;                    /* stereo input */
  inputParameters.sampleFormat = PA_SAMPLE_TYPE;
  inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultLowInputLatency;
  inputParameters.hostApiSpecificStreamInfo = NULL;
}

Sound::~Sound()
{
}

void Sound::playAudio()
{

  data.frameIndex = 0;

  outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
  if (outputParameters.device == paNoDevice) {
      std::cout << "Error: No default output device.\n" << std::endl;
      terminate();
  }
  outputParameters.channelCount = 2;                     /* stereo output */
  outputParameters.sampleFormat =  PA_SAMPLE_TYPE;
  outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
  outputParameters.hostApiSpecificStreamInfo = NULL;

  std::cout << "\n=== Now playing back. ===\n" << std::endl;
  err = Pa_OpenStream(
            &stream,
            NULL, /* no input */
            &outputParameters,
            SAMPLE_RATE,
            FRAMES_PER_BUFFER,
            paClipOff,      /* we won't output out of range samples so don't bother clipping them */
            playCallback,
            &data );
  if( err != paNoError ) terminate();

  if( stream )
  {
      err = Pa_StartStream( stream );
      if( err != paNoError ) terminate();

      std::cout << "Waiting for playback to finish.\n" << std::endl;

      while( ( err = Pa_IsStreamActive( stream ) ) == 1 ) Pa_Sleep(100);
      if( err < 0 ) terminate();

      err = Pa_CloseStream( stream );
      if( err != paNoError ) terminate();

      printf("Done.\n"); fflush(stdout);
  }
}

std::vector <unsigned char> Sound::recordAudio()
{
  err = Pa_OpenStream(
            &stream,
            &inputParameters,
            NULL,
            SAMPLE_RATE,
            FRAMES_PER_BUFFER,
            paClipOff,
            recordCallback,
            &data );
  if( err != paNoError ) terminate();

  err = Pa_StartStream( stream );
  if( err != paNoError ) terminate();
  std::cout << "\n=== Now recording!! Please speak into the microphone. ===\n" << std::endl;

  while( ( err = Pa_IsStreamActive( stream ) ) == 1 )
  {
      Pa_Sleep(1000);
      std::cout << "index = " << data.frameIndex << std::endl;
  }
  if( err < 0 ) terminate();

  err = Pa_CloseStream( stream );
  if( err != paNoError ) terminate();
  max = 0;
  average = 0.0;
  for( i=0; i<numSamples; i++ )
  {
      val = data.recordedSamples[i];
      if( val < 0 ) val = -val; /* ABS */
      if( val > max )
      {
          max = val;
      }
      average += val;
  }
  average = average / (double)numSamples;
  std::cout << "sample max amplitude = " << max << std::endl;
  std::cout << "sample average = %lf\n" << average << std::endl;
  std::ofstream fstream{"file.raw"};
  if( fstream.fail() )
  {
      std::cerr << "Could not open file." << std::endl;
  }
  else
  {
    for (int i = 0; i < this->data.maxFrameIndex; i++)
      fstream << this->data.recordedSamples[i];
  }
  fstream.close();
}


int Sound::recordCallback( const void *inputBuffer, void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData )
{
  paTestData *data = (paTestData*)userData;
  const SAMPLE *rptr = (const SAMPLE*)inputBuffer;
  SAMPLE *wptr = &data->recordedSamples[data->frameIndex * NUM_CHANNELS];
  long framesToCalc;
  long i;
  int finished;
  unsigned long framesLeft = data->maxFrameIndex - data->frameIndex;

  (void) outputBuffer;
  (void) timeInfo;
  (void) statusFlags;
  (void) userData;

  if( framesLeft < framesPerBuffer )
  {
      framesToCalc = framesLeft;
      finished = paComplete;
  }
  else
  {
      framesToCalc = framesPerBuffer;
      finished = paContinue;
  }

  if( inputBuffer == NULL )
  {
      for( i=0; i<framesToCalc; i++ )
      {
          *wptr++ = SAMPLE_SILENCE;  /* left */
          if( NUM_CHANNELS == 2 ) *wptr++ = SAMPLE_SILENCE;  /* right */
      }
  }
  else
  {
      for( i=0; i<framesToCalc; i++ )
      {
          *wptr++ = *rptr++;  /* left */
          if( NUM_CHANNELS == 2 ) *wptr++ = *rptr++;  /* right */
      }
  }
  data->frameIndex += framesToCalc;
  return finished;
}

int Sound::playCallback( const void *inputBuffer, void *outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo* timeInfo,
                         PaStreamCallbackFlags statusFlags,
                         void *userData )
{
  paTestData *data = (paTestData*)userData;
  SAMPLE *rptr = &data->recordedSamples[data->frameIndex * NUM_CHANNELS];
  SAMPLE *wptr = (SAMPLE*)outputBuffer;
  unsigned int i;
  int finished;
  unsigned int framesLeft = data->maxFrameIndex - data->frameIndex;

  (void) inputBuffer;
  (void) timeInfo;
  (void) statusFlags;
  (void) userData;

  if( framesLeft < framesPerBuffer )
  {
      for( i=0; i<framesLeft; i++ )
      {
          *wptr++ = *rptr++;
          if( NUM_CHANNELS == 2 ) *wptr++ = *rptr++;
      }
      for( ; i<framesPerBuffer; i++ )
      {
          *wptr++ = 0;
          if( NUM_CHANNELS == 2 ) *wptr++ = 0;
      }
      data->frameIndex += framesLeft;
      finished = paComplete;
  }
  else
  {
      for( i=0; i<framesPerBuffer; i++ )
      {
          *wptr++ = *rptr++;
          if( NUM_CHANNELS == 2 ) *wptr++ = *rptr++;
      }
      data->frameIndex += framesPerBuffer;
      finished = paContinue;
  }
  return finished;
}

PaError Sound::terminate()
{
  Pa_Terminate();
  if( data.recordedSamples )
      free( data.recordedSamples );
  if( err != paNoError )
  {
    std::cout << "An error occured while using the portaudio stream\n" << std::endl;
    std::cout << "Error number: " << err;
    std::cout << "Error message: %s\n" << Pa_GetErrorText( err ) << std::endl;
      err = 1;
  }
  return err;
}

int main()
{
   Sound Sound1;
   Sound1.recordAudio();
   Sound1.playAudio();
}
