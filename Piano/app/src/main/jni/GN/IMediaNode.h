//
// Created by shivaaz on 1/3/23.
//

#ifndef KALASOFT_MEDIANODE_H
#define KALASOFT_MEDIANODE_H


#include "../Audio/AudioSamples.h"
#include "queue"

enum ENodeRes{ENodeRes_UNKNOWN = INT_MIN,ENodeRes_FAIL=-1,ENodeRes_OK = 0
};


class MediaPipeline;
class IPipelineAllocator{

public:

    // set pipeline->allocator to this.
    virtual MediaPipeline* allocate() = 0;

    virtual void onPipelineEnd(MediaPipeline * pipeline) = 0;
};

//allocate only using allocator
class MediaPipeline{

    friend class IPipelineAllocator;

public:

    MediaPipeline() = delete;

    ~MediaPipeline(){delete samples;}

    MediaPipeline(IPipelineAllocator *allocator){ this->allocator = allocator;}

    void end(){ allocator->onPipelineEnd(this);}

    AudioSamples *samples = nullptr;

private:

    IPipelineAllocator* allocator;
};

class IMediaNode{

public:

    virtual ENodeRes prepare() = 0;

    virtual ENodeRes start() = 0;

    virtual ENodeRes pause() = 0;

    virtual ENodeRes stop() = 0;

    virtual ENodeRes release() = 0;

    virtual ENodeRes onPipelineInput(MediaPipeline *pipleline) = 0;

    virtual ENodeRes onPipelineOutput(MediaPipeline *pipleline) = 0;

    void setNextNode(IMediaNode *nextNode){ this->nextNode = nextNode;}

protected:

    IMediaNode* nextNode = nullptr;

};


#endif //KALASOFT_MEDIANODE_H
