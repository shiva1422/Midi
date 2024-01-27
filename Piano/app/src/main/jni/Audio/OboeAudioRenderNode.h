//
// Created by shivaaz on 1/3/23.
//

#ifndef KALASOFT_OBOEAUDIORENDERNODE_H
#define KALASOFT_OBOEAUDIORENDERNODE_H


#include <memory>
#include "../GN/IMediaNode.h"
#include "AudioSamples.h"
#include "oboe/Oboe.h"
#include "list"

//TODO headset Switching
class OboeAudioRenderNode : public IMediaNode, public ks::AudioInterface , public oboe::AudioStreamCallback{

public:

    //TODO below methods are not thread safe at this point need to be synchronized.

    //IMediaNode Impl

    ENodeRes prepare() override;

    ENodeRes start() override;

    ENodeRes pause() override;

    ENodeRes stop() override;

    ENodeRes release() override;

    ENodeRes onPipelineInput(MediaPipeline *pipleline) override;

    ENodeRes onPipelineOutput(MediaPipeline *pipleline) override;


    //Oboe::AudioStreamCallbackImpl

    oboe::DataCallbackResult onAudioReady(oboe::AudioStream *audioStream, void *audioData, int32_t numFrames) override;

    ~OboeAudioRenderNode() override = default;    //TODO destructor;

    bool onError(oboe::AudioStream *stream, oboe::Result result) override;

    void onErrorBeforeClose(oboe::AudioStream *stream, oboe::Result result) override;

    void onErrorAfterClose(oboe::AudioStream *stream, oboe::Result result) override;

    int64_t getCurrentTime() override;


private:

    std::shared_ptr<oboe::AudioStream> stream = nullptr;

    std::queue<MediaPipeline*> pipelineQ;
    std::mutex mutPipelineQ;
    MediaPipeline *curPipeline = nullptr;

private:
    AudioSamples *curBuf ;



};


#endif //KALASOFT_OBOEAUDIORENDERNODE_H
