//
//  Created by shivaaz on 12/26/22.
//

#include <stdio.h>
#include "AudioSamples.h"
using namespace ks;

AudioSamples::AudioSamples(const ks::AudioFormat audioFormat,const int sampleCnt/* = 512 */)
{
    this->audioFormat = audioFormat;
    this->sampleCnt = sampleCnt;
            
    int planeSize = getBytesPerSample(audioFormat.sampleFmt) * sampleCnt;
    
    int planeCnt = 1;
        
    if(isPlanar())
    {
        planeCnt = audioFormat.channelFmt;////TODO later get channel count
    }
    else
    {
        planeSize *= audioFormat.channelFmt;//TODO later get channel count

    }
    

    KSLOGI("AudioSamples","allocating planeCnt %d ,plane Size %d,sampleCount %d",planeCnt,planeSize,sampleCnt);
    
    audioBuffer = MemAllocator::allocate(planeCnt,planeSize);
    
}

AudioSamples::~AudioSamples()
{
    delete audioBuffer;
}


bool AudioSamples::isPlanar()
{
    //TODO use this from audio interface ,for some reason this file couldn't recognize isPlanar from AudioInterface.h for now.
   if((int)audioFormat.sampleFmt >= 5 && (int)audioFormat.sampleFmt <= 11)
   {
       return true;
   }
    
  return false;
}
