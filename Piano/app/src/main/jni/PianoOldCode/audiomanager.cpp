//
// Created by pspr1 on 8/6/2020.
//
#include "audiomanager.h"
#include "piano.h"
extern PianoSynthNode piano;
AudioEngine::AudioEngine()
{
    for(int octaveNo=0;octaveNo<8;octaveNo++) {
        frequency[52 + octaveNo * 5] = keyFrequencies[12 * octaveNo];//g#
        frequency[52 + octaveNo * 5 + 1] = keyFrequencies[12 * octaveNo + 2];//a#
        frequency[52 + octaveNo * 5 + 2] = keyFrequencies[12 * octaveNo + 5];//c#
        frequency[52 + octaveNo * 5 + 3] = keyFrequencies[12 * octaveNo + 7];//d#
        frequency[52 + octaveNo * 5 + 4] = keyFrequencies[12 * octaveNo + 10];//f#
        frequency[7 * octaveNo] = keyFrequencies[12 * octaveNo + 1];//a
        frequency[7 * octaveNo + 1] = keyFrequencies[12 * octaveNo + 3];//b
        frequency[7 * octaveNo + 2] = keyFrequencies[12 * octaveNo + 4];//c
        frequency[7 * octaveNo + 3] = keyFrequencies[12 * octaveNo + 6];//d
        frequency[7 * octaveNo + 4] = keyFrequencies[12 * octaveNo + 8];//e
        frequency[7 * octaveNo + 5] = keyFrequencies[12 * octaveNo + 9];//f
        frequency[7 * octaveNo + 6] = keyFrequencies[12 * octaveNo + 11];//g
    }
    frequency[87]=3951.066;
    frequency[88]= 4186.009;
       for(int i=0;i<90;i++)
       {
          // AUDIOLOG("the frequencies are %f",frequency[i]);
       }
}
void AudioEngine::setUp() {
    AudioStreamBuilder builder;
    builder.setCallback(this);
    builder.setSampleRate(11025);
    builder.setFormat(AudioFormat::I16);
    builder.setContentType(ContentType::Music);
    builder.setPerformanceMode(PerformanceMode::LowLatency);
    builder.setSharingMode(SharingMode::Exclusive);
    builder.setChannelCount(2);
    Result result = builder.openStream(&stream);
    if (result != Result::OK) {
        AUDIOLOG("THE STREAM COULD NOT BE CREATES");
    }
    if (result != Result::OK) {
        AUDIOLOG("THE STREAM COULD NOT BE started %s", convertToText(result));
    }
    auto bufferSizeResult = stream->setBufferSizeInFrames(stream->getFramesPerBurst() *2);//buffersize result twice so that glitch does not occur even if we miss onec callback in time
    if (bufferSizeResult) {
        AUDIOLOG("the Buffersize size is %d frames", bufferSizeResult.value());


    }

    result = stream->requestStart();
    // stream->close(); after finish



}
DataCallbackResult AudioEngine::onAudioReady(AudioStream *oboeStream, void *audioData, int32_t numFrames)
{
    piano.onPlay(audioData,numFrames);
   //piano.onPlayNew(audioData,numFrames);
   return DataCallbackResult ::Continue;
}

////player

DataCallbackResult AudioEngine:: player(AudioStream *oboeStream, void *audioData, int32_t numFrames)
{
  piano.onPlay(audioData,numFrames);
      return DataCallbackResult ::Continue;
}

/*DataCallbackResult AudioEngine::synthesizer(AudioStream *oboeStream, void *audioData, int32_t numFrames)
{
    static long int sampleNo=0;////////sinewave
    static float  phase=0;
    float *audioDat=(float *)audioData;
    static  uint8_t audioMsg[2];
    read(pipeFd,audioMsg,sizeof(audioMsg));
    // AUDIOLOG("the key is %d and the status is %d",audioMsg[0],audioMsg[1]);
    if(audioMsg[1]==1)
        for(int i=0;i<numFrames*2;i+=2)
        {
            phase+=TWO_PI*keyFrequencies[audioMsg[0]]/40000;
            if(phase>TWO_PI)
                phase-=TWO_PI;
            audioDat[i]=(float)sin(phase);
            audioDat[i+1]=audioDat[i];
            sampleNo++;
        }
    else
    {phase=0;
        for(int i=0;i<numFrames*2;i+=2)
        {
            phase+=TWO_PI*440/40000;
            if(phase>TWO_PI)
                phase-=TWO_PI;
            audioDat[i]=-1;//(float)sin(phase);
            audioDat[i+1]=audioDat[i];
            sampleNo++;
        }
    }
    //  AUDIOLOG("filled the b uffer %d",pthread_gettid_np(pthread_self()));

    return DataCallbackResult ::Continue;
}
*/
