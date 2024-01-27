//
// Created by shivaaz on 1/7/23.
//

#ifndef KALASOFT_AUDIOINTERFACE_H
#define KALASOFT_AUDIOINTERFACE_H

#include "Commons.h"
#include "KSLog.h"

namespace ks{

    enum ESampleFormat{INT16,FLOAT};

    enum EChannelFormat{MONO = 1,STEREO};

    class AudioFormat{

    public:

        ESampleFormat sampleFmt;

        EChannelFormat channelFmt;

        int sampleRate;
    };


    class AudioInterface{

    public:

        void setAudioFormat(const AudioFormat &fmt){audioFormat = fmt;}

        AudioFormat getAudioFormat()const{return audioFormat;}

        void setFramesPerCallback(int framCnt){framesPerCallback = framCnt;}

        int getFramesPerCallback()const{return framesPerCallback;}

        //get current audio time in US
        virtual int64_t getCurrentTime()=0;

    protected:

        AudioFormat audioFormat;

        int framesPerCallback = 512;

    };


   KSFORCEINLINE int getBytesPerSample(ESampleFormat sampleFormat)
   {
       switch (sampleFormat)
       {
           case ESampleFormat::INT16:return 2;
           case ESampleFormat::FLOAT:return 4;
           default:
               KSLOGE("AudioInterface","Invalid sampleFmt");
               return 4;
       }
   }


}
#endif //KALASOFT_AUDIOINTERFACE_H
