//
// Created by shivaaz on 11/12/22.
//

#ifndef DIGIBANDPIANO_SLESAUDIOPLAYER_H
#define DIGIBANDPIANO_SLESAUDIOPLAYER_H

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <SLES/OpenSLES_AndroidConfiguration.h>

const char *getSLErrStr(SLresult code);

class SLESAudioPlayer {

public:

    bool start();

    bool prepare();

    friend void bqPlayerCallback(SLAndroidSimpleBufferQueueItf bq, void *context);


private:

    SLresult createEngine();

    SLresult createBufferQPlayer();

    SLresult open();

    void close(){}//TODO release objects even when fails

private:

    // engine interfaces
    SLObjectItf                   engineObject = nullptr;
    SLEngineItf                   engineInterface = nullptr;


    //output mix interfaces
    SLObjectItf                   outputMixObject = nullptr;
    SLEnvironmentalReverbItf      outputMixEnvironmentalReverb = NULL;


    // aux effect on the output mix, used by the buffer queue player
    const SLEnvironmentalReverbSettings reverbSettings = SL_I3DL2_ENVIRONMENT_PRESET_STONECORRIDOR;

    SLAndroidSimpleBufferQueueItf simpleBufferQueueInterface = nullptr;
    int                    framePerCallback = 1024;//default but try use system
    int                     sampleRate = 44100;
    int                     channelCnt = 2;
    SLuint32                channelMask = SL_ANDROID_SPEAKER_USE_DEFAULT;//TODO oboe has but not me android STEREo
    SLuint32                bitsPerSample =  16;//16bit samples//todo
    SLuint32                bufferQueueLength =  4;

    // configure audio source



    //COntent Type = music,usage type = media,Sharing mode shared
    SLAndroidConfigurationItf configItf = nullptr;


private:

    // buffer queue player interfaces
    SLObjectItf bqPlayerObject = NULL;
    SLPlayItf bqPlayerPlay;
    SLAndroidSimpleBufferQueueItf bqPlayerBufferQueue;
    SLEffectSendItf bqPlayerEffectSend;
    SLMuteSoloItf bqPlayerMuteSolo;
    SLVolumeItf bqPlayerVolume;
    //short *resampleBuf = NULL;

};



#endif //DIGIBANDPIANO_SLESAUDIOPLAYER_H
