//
// Created by shivaaz on 11/12/22.
//

#include <Logger/KSLog.h>
#include "SLESAudioPlayer.h"

#include <sys/system_properties.h>
#include <cstdio>
#include <pthread.h>
#include <cassert>
#include <cstdlib>
#include <cstring>

#define TAG "SLESAudioPlayer"




static pthread_mutex_t  audioEngineLock = PTHREAD_MUTEX_INITIALIZER;
int getSdkVersion()
{
    static int sCachedSdkVersion = -1;
    if (sCachedSdkVersion == -1) {
        int result = -1;
        char valueText[PROP_VALUE_MAX] = {0};
        if (__system_property_get("ro.build.version.sdk", valueText) != 0) {
            result = atoi(valueText);
        }
        sCachedSdkVersion = result;
    }
    return sCachedSdkVersion;
}

bool SLESAudioPlayer::prepare()
{

    return SL_RESULT_SUCCESS == open();

    if (getSdkVersion() < __ANDROID_API_L__) // && mFormat == AudioFormat::Float){TODO
    {
        // TODO: Allow floating point format on API <21 using float->int16 converter
       KSLOGE(TAG,"invalid Sample FMT should update");
        // API 21+: FLOAT
        // API <21: INT16
        return false;
    }





}

SLresult SLESAudioPlayer :: open()
{

    SLresult res = createEngine();
    assert(SL_RESULT_SUCCESS == res);
    (void)res;

    res = createBufferQPlayer();
    assert(SL_RESULT_SUCCESS == res);
    (void)res;

    //if(failed)close();

    return res;

}

bool SLESAudioPlayer::start()
{
    // set the player's state to playing
   SLresult result = (*bqPlayerPlay)->SetPlayState(bqPlayerPlay, SL_PLAYSTATE_PLAYING);
    assert(SL_RESULT_SUCCESS == result);
    (void)result;

    return true;
}
// These will wind up in <SLES/OpenSLES_Android.h>
constexpr int SL_ANDROID_SPEAKER_STEREO = (SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT);

constexpr int SL_ANDROID_SPEAKER_QUAD = (SL_ANDROID_SPEAKER_STEREO
                                         | SL_SPEAKER_BACK_LEFT | SL_SPEAKER_BACK_RIGHT);

constexpr int SL_ANDROID_SPEAKER_5DOT1 = (SL_ANDROID_SPEAKER_QUAD
                                          | SL_SPEAKER_FRONT_CENTER  | SL_SPEAKER_LOW_FREQUENCY);

constexpr int SL_ANDROID_SPEAKER_7DOT1 = (SL_ANDROID_SPEAKER_5DOT1 | SL_SPEAKER_SIDE_LEFT
                                          | SL_SPEAKER_SIDE_RIGHT);

static SLuint32  channelCountToChannelMask(int channelCount) {
    SLuint32 channelMask = 0;

    switch (channelCount) {
        case 1:
            channelMask = SL_SPEAKER_FRONT_CENTER;
            break;

        case 2:
            channelMask = SL_ANDROID_SPEAKER_STEREO;
            break;

        case 4: // Quad
            channelMask = SL_ANDROID_SPEAKER_QUAD;
            break;

        case 6: // 5.1
            channelMask = SL_ANDROID_SPEAKER_5DOT1;
            break;

        case 8: // 7.1
            channelMask = SL_ANDROID_SPEAKER_7DOT1;
            break;

        default:
            channelMask = channelCountToChannelMask(
                    2);//TODO channelCountToChannelMaskDefault(channelCount);//AudioStreamOpenSLES
            break;
    }
    return channelMask;
}

static bool s_isLittleEndian()
{
    static uint32_t value = 1;
    return (*reinterpret_cast<uint8_t *>(&value) == 1);  // Does address point to LSB?
}

static SLuint32 getDefaultByteOrder() {
    return s_isLittleEndian() ? SL_BYTEORDER_LITTLEENDIAN : SL_BYTEORDER_BIGENDIAN;
}
#include <iostream>
#include "thread"

// this callback handler is called every time a buffer finishes playing
#define TWO_PI 6.28
void bqPlayerCallback(SLAndroidSimpleBufferQueueItf bq, void *context)
{
    //KSLOGD(TAG,"SLES calling back");
    SLresult result;
    static float phase = 0;
    SLESAudioPlayer *player = (SLESAudioPlayer *)context;
    assert(bq == player->bqPlayerBufferQueue);
    // for streaming playback, replace this test by logic to find and fill the next buffer
    bool bExit = false;
    if (!bExit)
    {
        SLAndroidSimpleBufferQueueState bufferQueueState;
        (*bq)->GetState(bq,&bufferQueueState);
       KSLOGI(TAG,"bq state size %ud index %ud",bufferQueueState.count,bufferQueueState.index);

        int sampleCnt = 1024;
        void *buf = malloc(sampleCnt * 2 * 2);
       /* int n = 1;
        if(bufferQueueState.count < 3)
            n =1;*/


            int16_t *audio = (int16_t *) buf;
            for (int i = 0; i < sampleCnt * 2; i += 2) {

                phase += TWO_PI * 440.0 / 44100;
                if (phase > TWO_PI)
                    phase -= TWO_PI;
                audio[i] = INT16_MAX * sin(phase);
                audio[i + 1] = audio[i];
            }

            // enqueue another buffer
            result = (*bq)->Enqueue(bq, buf, sampleCnt * 2 * 2);

        free(buf);
        // the most likely other result is SL_RESULT_BUFFER_INSUFFICIENT,
        // which for this code example would indicate a programming error
        if (SL_RESULT_SUCCESS != result)
        {
            //pthread_mutex_unlock(&audioEngineLock);
        }
        (void)result;
    } else
    {
       // releaseResampleBuf();
        pthread_mutex_unlock(&audioEngineLock);
    }
}

SLresult SLESAudioPlayer::createBufferQPlayer() {

    // bitsPerSample = calculate();//TODO

    SLresult result = SL_RESULT_SUCCESS;

    // configure audio source
    // bufferQueueLength = calculateOptimalBufferQueueLength();AudioStreamOpenSLES
    SLDataLocator_AndroidSimpleBufferQueue loc_bufq = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE,
                                                       static_cast<SLuint32>(bufferQueueLength)};

    // Define the audio data format.
    SLDataFormat_PCM format_pcm = {
            SL_DATAFORMAT_PCM,       // formatType
            static_cast<SLuint32>(channelCnt),           // numChannels
            static_cast<SLuint32>(sampleRate * 1000),    // milliSamplesPerSec
            bitsPerSample,                      // bitsPerSample
            bitsPerSample,                      // containerSize;
            channelCountToChannelMask(channelCnt), // channelMask
            getDefaultByteOrder(),
    };
    /*above with sl specific
     *  SLDataFormat_PCM format_pcm = {SL_DATAFORMAT_PCM, 1, SL_SAMPLINGRATE_8,
        SL_PCMSAMPLEFORMAT_FIXED_16, SL_PCMSAMPLEFORMAT_FIXED_16,
        SL_SPEAKER_FRONT_CENTER, SL_BYTEORDER_LITTLEENDIAN};
     */

    /*
     * Enable Fast Audio when possible:  once we set the same rate to be the native, fast audio path
     * will be triggered
     */
   /* if(bqPlayerSampleRate) {
        format_pcm.samplesPerSec = bqPlayerSampleRate; //sample rate in mili second (obtained from audiomanager native sampleRate*1000
    }*/

    SLDataSource audioSrc = {&loc_bufq, &format_pcm};
    /**
     * API 21 (Lollipop) introduced support for floating-point data representation and an extended
     * data format type: SLAndroidDataFormat_PCM_EX. If running on API 21+ use this newer format
     * type, creating it from our original format.
     */
   /* SLAndroidDataFormat_PCM_EX format_pcm_ex;
    if (getSdkVersion() >= __ANDROID_API_L__) {
        SLuint32 representation = OpenSLES_ConvertFormatToRepresentation(getFormat());
        // Fill in the format structure.
        format_pcm_ex = OpenSLES_createExtendedFormat(format_pcm, representation);
        // Use in place of the previous format.
        audioSrc.pFormat = &format_pcm_ex;
    }*/

    // configure audio sink
    SLDataLocator_OutputMix loc_outmix = {SL_DATALOCATOR_OUTPUTMIX, outputMixObject};
    SLDataSink audioSnk = {&loc_outmix, NULL};

    /*
    * create audio player:
    *     fast audio does not support when SL_IID_EFFECTSEND is required, skip it
    *     for fast audio case
    */
    //TODO .....Check this in bug android simple bufferqueue not SL_IID_BUFFERQUEUE
    const SLInterfaceID ids[4] = {SL_IID_ANDROIDSIMPLEBUFFERQUEUE,SL_IID_ANDROIDCONFIGURATION, SL_IID_VOLUME, SL_IID_EFFECTSEND/*,SL_IID_MUTESOLO,*/};
    const SLboolean req[4] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE/*,SL_BOOLEAN_TRUE,*/ };

    result = (*engineInterface)->CreateAudioPlayer(engineInterface, &bqPlayerObject, &audioSrc, &audioSnk,/* not fast audio path using divice specific sample rate no SL_IID_EFFECTSEND*/false? 3 : 4, ids, req);
    //????(*engineInterface)->CreateAudioPlayer(engineInterface,&bqPlayerObject, &audioSrc,&audioSnk,sizeof(ids) / sizeof(ids[0]), ids, req);
    assert(SL_RESULT_SUCCESS == result);
    (void)result;

    // Configure the stream.
    result = (*bqPlayerObject)->GetInterface(bqPlayerObject,SL_IID_ANDROIDCONFIGURATION,(void *)&configItf);

    if(SL_RESULT_SUCCESS != result ||  configItf == nullptr)
    {

       KSLOGW("%s() GetInterface(SL_IID_ANDROIDCONFIGURATION) failed with %s or nullptr config",__func__, getSLErrStr(result));
        (void)result;
    }
    else
    {

        //if(c)
        if (getSdkVersion() < __ANDROID_API_N_MR1__)
        {
           KSLOGW("%s() not supported until N_MR1", __func__);
           // mPerformanceMode = PerformanceMode::None;
            result = SL_RESULT_SUCCESS;
        }
        else
        {
            SLuint32 performanceMode = SL_ANDROID_PERFORMANCE_LATENCY;//AudioStreamOpenslses
            result = (*configItf)->SetConfiguration(configItf,SL_ANDROID_KEY_PERFORMANCE_MODE, &performanceMode,sizeof(performanceMode));
            if (SL_RESULT_SUCCESS != result)
            {
               KSLOGW(TAG,"SetConfiguration(PERFORMANCE_MODE, SL %u) returned %s",performanceMode, getSLErrStr(result));
               // mPerformanceMode = PerformanceMode::None;
               result = SL_RESULT_SUCCESS;//ignore as this is not optional
            }
        }

    }


    SLuint32 presetValue = SL_ANDROID_STREAM_VOICE;//AudioOutputStreamSLES //TODO
    result = (*configItf)->SetConfiguration(configItf,SL_ANDROID_KEY_STREAM_TYPE,&presetValue,sizeof(presetValue));
    if (SL_RESULT_SUCCESS != result)
    {
       KSLOGW(TAG,"Stream type Could not be set");
        result = SL_RESULT_SUCCESS;//ignoring err
    }


    // realize the player
    result = (*bqPlayerObject)->Realize(bqPlayerObject, SL_BOOLEAN_FALSE);
    assert(SL_RESULT_SUCCESS == result);
    (void)result;

    // get the start interface
    result = (*bqPlayerObject)->GetInterface(bqPlayerObject, SL_IID_PLAY, &bqPlayerPlay);
    assert(SL_RESULT_SUCCESS == result);
    (void)result;


    // register callback on the buffer queue

    // get the buffer queue interface
    result = (*bqPlayerObject)->GetInterface(bqPlayerObject, SL_IID_ANDROIDSIMPLEBUFFERQUEUE,
                                             &bqPlayerBufferQueue);
    assert(SL_RESULT_SUCCESS == result);
    (void)result;


    // register callback on the buffer queue
    result = (*bqPlayerBufferQueue)->RegisterCallback(bqPlayerBufferQueue, bqPlayerCallback, this);
    assert(SL_RESULT_SUCCESS == result);
    (void)result;


    int size = 256;
    void *buf = malloc(size);
    memset(buf,0,size);
    //enque silence to trigger callback on start
    (*bqPlayerBufferQueue)->Enqueue(bqPlayerBufferQueue,buf,size);
    free(buf);
    //getConfiguration for performance mode android oboe

    //ndk get volume,effect send ,and mute solo interfaces;
    return result;
}
SLresult SLESAudioPlayer::createEngine()
{
    SLresult  result = SL_RESULT_SUCCESS;

    //create Engine
    result = slCreateEngine(&engineObject, 0, NULL, 0, NULL, NULL);
    assert(SL_RESULT_SUCCESS == result);
    (void)result;

    //realize Engine
    result = (*engineObject)->Realize(engineObject, SL_BOOLEAN_FALSE);
    assert(SL_RESULT_SUCCESS == result);
    (void)result;

    // get the engine interface, which is needed in order to create other objects
    result = (*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &engineInterface);
    assert(SL_RESULT_SUCCESS == result);
    (void)result;


    //create Output Mix
    // create output mix, with environmental reverb specified as a non-required interface
    const SLInterfaceID ids[1] = {SL_IID_ENVIRONMENTALREVERB};//oboe didn't use this
    const SLboolean req[1] = {SL_BOOLEAN_TRUE};
    result = (*engineInterface)->CreateOutputMix(engineInterface, &outputMixObject, 1, ids, req);
    assert(SL_RESULT_SUCCESS == result);
    (void)result;

    // realize the output mix
    result = (*outputMixObject)->Realize(outputMixObject, SL_BOOLEAN_FALSE);
    assert(SL_RESULT_SUCCESS == result);
    (void)result;

    // get the environmental reverb interface
    // this could fail if the environmental reverb effect is not available,
    // either because the feature is not present, excessive CPU load, or
    // the required MODIFY_AUDIO_SETTINGS permission was not requested and granted
    result = (*outputMixObject)->GetInterface(outputMixObject, SL_IID_ENVIRONMENTALREVERB,
                                              &outputMixEnvironmentalReverb);
    // ignore unsuccessful result codes for environmental reverb, as it is optionale

    if (SL_RESULT_SUCCESS == result)
    {
        result = (*outputMixEnvironmentalReverb)->SetEnvironmentalReverbProperties(
                outputMixEnvironmentalReverb, &reverbSettings);
        if(result != SL_RESULT_SUCCESS)
        {
           KSLOGW(TAG,"SetEnvironmentalReverbProperties failed");
            result = SL_RESULT_SUCCESS;//ignore this as not required;
        }
    }
    else
    result = SL_RESULT_SUCCESS;//ignore;

    return result;
}
const char *getSLErrStr(SLresult code)
{
    switch (code)
    {
            case SL_RESULT_SUCCESS:
                return "SL_RESULT_SUCCESS";
            case SL_RESULT_PRECONDITIONS_VIOLATED:
                return "SL_RESULT_PRECONDITIONS_VIOLATED";
            case SL_RESULT_PARAMETER_INVALID:
                return "SL_RESULT_PARAMETER_INVALID";
            case SL_RESULT_MEMORY_FAILURE:
                return "SL_RESULT_MEMORY_FAILURE";
            case SL_RESULT_RESOURCE_ERROR:
                return "SL_RESULT_RESOURCE_ERROR";
            case SL_RESULT_RESOURCE_LOST:
                return "SL_RESULT_RESOURCE_LOST";
            case SL_RESULT_IO_ERROR:
                return "SL_RESULT_IO_ERROR";
            case SL_RESULT_BUFFER_INSUFFICIENT:
                return "SL_RESULT_BUFFER_INSUFFICIENT";
            case SL_RESULT_CONTENT_CORRUPTED:
                return "SL_RESULT_CONTENT_CORRUPTED";
            case SL_RESULT_CONTENT_UNSUPPORTED:
                return "SL_RESULT_CONTENT_UNSUPPORTED";
            case SL_RESULT_CONTENT_NOT_FOUND:
                return "SL_RESULT_CONTENT_NOT_FOUND";
            case SL_RESULT_PERMISSION_DENIED:
                return "SL_RESULT_PERMISSION_DENIED";
            case SL_RESULT_FEATURE_UNSUPPORTED:
                return "SL_RESULT_FEATURE_UNSUPPORTED";
            case SL_RESULT_INTERNAL_ERROR:
                return "SL_RESULT_INTERNAL_ERROR";
            case SL_RESULT_UNKNOWN_ERROR:
                return "SL_RESULT_UNKNOWN_ERROR";
            case SL_RESULT_OPERATION_ABORTED:
                return "SL_RESULT_OPERATION_ABORTED";
            case SL_RESULT_CONTROL_LOST:
                return "SL_RESULT_CONTROL_LOST";
            default:
                return "Unknown SL error";
        }
}
