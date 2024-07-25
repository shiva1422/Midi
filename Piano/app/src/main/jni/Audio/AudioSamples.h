//
// Created by shivaaz on 12/26/22.
//

#ifndef KALASOFT_AUDIOSAMPLES_H
#define KALASOFT_AUDIOSAMPLES_H

#include <memory>
#include "AudioInterface.h"
#include "KSMemory.h"

/*
 Provides safe way to reuse the samples ones the samples are used like after rendered to player ,give back to the allocator;
 */
class AudioSamples;
class ISamplesAllocator{
    
public:
    
    virtual AudioSamples* allocateSamples(const ks::AudioFormat audioFormat,const int sampleCount) = 0;
    
    //generally to queue the used samples for reuse.
    virtual void reuse(AudioSamples *samples) = 0 ;//
};

class AudioSamples {

public:


    AudioSamples(const ks::AudioFormat audioFormat,const int sampleCnt = 512);
    
    ~AudioSamples();

    int getSampleCnt() const {return sampleCnt;};

    int getSampleRate() const {return audioFormat.sampleRate;}

    ks::EChannelFormat getChannelFormat() const {return audioFormat.channelFmt;}

    ks::ESampleFormat getSampleFormat() const {return audioFormat.sampleFmt;}

    ks::AudioFormat getAudioFormat()const{return audioFormat;}

    ks::Buffer* getAudioBuffer() const{return audioBuffer;}

    uint8_t *data(){return audioBuffer->data[0];}//TODO NO_OP.

    //size in bytes of data()
    int getBufferSize() const {return bufSize;}
    
    bool isPlanar();


    void setSampleCnt(int cnt){sampleCnt = cnt;}
    

    void muteSamples()
    {
       
        if(audioBuffer)
        {
            audioBuffer->setToZero();
        }
    }
    
    

    void setAllocator(ISamplesAllocator *allocator){this->allocator = allocator;}
    
    //if returned false ,allocator was not set,so delete samples;
    bool yield()
    {
        if(allocator){allocator->reuse(this);return true;}
        return false;
    }
    
    
protected:
    
    //to give back the samples to its allocator(allocator->reuse(this));never delete allocator when freeing samples.
    ISamplesAllocator *allocator = nullptr;
    
private:
   

protected:

    ks::AudioFormat audioFormat;
    
    //This is 
    int64_t pts = 0;

    int sampleCnt = 0;
    
    ks::Buffer *audioBuffer = nullptr;
    

    int bufSize = 0;


};




#endif //KALASOFT_AUDIOSAMPLES_H
