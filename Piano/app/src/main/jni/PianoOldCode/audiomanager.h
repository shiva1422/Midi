//
// Created by pspr1 on 7/1/2020.
//

#ifndef DIGIBAND_AUDIOMANAGER_H
#define DIGIBAND_AUDIOMANAGER_H
#include <oboe/AudioStreamBuilder.h>
#include "oboe/Oboe.h"
#include "android/log.h"
#define TAGA "AUDIO"
#define TWO_PI (3.14159 * 2)
//#define LOGE(...)((void)__android_log_print(ANDROID_LOG_INFO,TAG,__VA_ARGS__));
#define AUDIOLOG(...)((void)__android_log_print(ANDROID_LOG_ERROR,TAGA,__VA_ARGS__))



using namespace oboe;
class AudioEngine : public AudioStreamCallback{
public:
    AudioStream *stream=NULL;
    float frequency[89];//unordered and one key-52 ignored
public:
    AudioEngine();
    void setUp();
    DataCallbackResult onAudioReady(AudioStream *oboeStream, void *audioData, int32_t numFrames);
    DataCallbackResult player(AudioStream *oboeStream, void *audioData, int32_t numFrames);
    DataCallbackResult synthesizer(AudioStream *oboeStream, void *audioData, int32_t numFrames);
};
typedef struct AudioFormatKala{
    short numChannels,bitsPerSample,blockAlign,audioFormat;//blockAlign= NumChannels * BitsPerSample/8,audiioformat=1 for pcm other for compressed
    uint32_t sampleRate,byteRate;//byteRate= SampleRate * NumChannels * BitsPerSample/8
}AudioFormatKala;
#endif //DIGIBAND_AUDIOMANAGER_H
