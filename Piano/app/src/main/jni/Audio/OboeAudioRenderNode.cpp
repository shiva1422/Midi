//
// Created by shivaaz on 1/3/23.
//

#include <Logger/KSLog.h>
#include <CMedia/EMedia.h>
#include "OboeAudioRenderNode.h"
#define TAGLOG "OboeAudioRenderer"

using namespace oboe;

//TODO use custom bufQueu with devicespecific callbacksize;
oboe::AudioFormat audioFormatFromKS(ks::ESampleFormat fmt)
{
    switch (fmt)
    {
        case ks::ESampleFormat::INT16:return AudioFormat::I16;
        case ks::ESampleFormat::FLOAT:return AudioFormat::Float;
        default:
            assert(false);//todo
    }
}


/** IMediaNode Impl */
ENodeRes OboeAudioRenderNode::prepare()
{
    AudioStreamBuilder builder;
    builder.setDirection(Direction::Output)->setChannelCount(audioFormat.channelFmt)->setSampleRate(audioFormat.sampleRate)->setFormat(
            audioFormatFromKS(audioFormat.sampleFmt))->setPerformanceMode(PerformanceMode::LowLatency)->setCallback(this);
    if(framesPerCallback > 0)
        builder.setFramesPerDataCallback(framesPerCallback);

    Result res = builder.openStream(stream);
    //TODO getBurst size and setBuffSize according to requirements
    if(res != Result::OK)
    {
        KSLOGE(TAGLOG,"error create Stream");
        assert(false);//TODO
        return ENodeRes_FAIL;
    }

    framesPerCallback = stream->getFramesPerBurst();
    stream->setBufferSizeInFrames(2 * framesPerCallback);
    if(audioFormat.sampleRate != stream->getSampleRate() || audioFormat.channelFmt != stream->getChannelCount() || stream->getFormat() !=
                                                                                                                           audioFormatFromKS(audioFormat.sampleFmt))
    {
        KSLOGE(TAGLOG,"Error outpustream created with unmatched properties audio may distort");
        assert(false);
    }
    return ENodeRes_OK;
}

ENodeRes OboeAudioRenderNode::start()
{
    if(stream)
        stream->requestStart();
    return ENodeRes_OK;
}

ENodeRes OboeAudioRenderNode::pause()
{
    if(stream)
        stream->requestPause();
    return ENodeRes_OK;
}

ENodeRes OboeAudioRenderNode::stop() {
    if(stream)
    {
        stream->requestFlush();
        stream->requestStop();
    }
    return ENodeRes_OK;
}

ENodeRes OboeAudioRenderNode::release() {
    return ENodeRes_OK;
}

ENodeRes OboeAudioRenderNode::onPipelineInput(MediaPipeline *pipleline)
{
        std::lock_guard<std::mutex> lock(mutPipelineQ);
        pipelineQ.push(pipleline);
        return ENodeRes_OK;
}
ENodeRes OboeAudioRenderNode::onPipelineOutput(MediaPipeline *pipleline)
{
    return ENodeRes_OK;
}

/** IMediaNode Impl end */


/** AudioStreamCallback Impl*/
DataCallbackResult OboeAudioRenderNode::onAudioReady(AudioStream *audioStream, void *audioData, int32_t numFrames)
{
    KSLOGD(TAGLOG,"Filling audio samplesCnt %d",numFrames);
    int bufSize = numFrames * audioStream->getBytesPerFrame();
    curPipeline = nullptr;
    curBuf = nullptr;
    {
        std::lock_guard<std::mutex> lock(mutPipelineQ);
        if(pipelineQ.size())
        {
            curPipeline = pipelineQ.front();
            pipelineQ.pop();
        }
    }
    if(curPipeline && curPipeline->samples)
    {
        curBuf = curPipeline->samples;
    }
    else
    {
        KSLOGV(TAGLOG,"empty samples");
        memset(audioData,0,bufSize);
        if(curPipeline)
            curPipeline->end();
        return oboe::DataCallbackResult::Continue;
    }
    KSLOGD(TAGLOG,"audio input Sc %d bufSize %d output sc %d bufsize %d",curBuf->getSampleCnt(),curBuf->getBufferSize(),numFrames,bufSize);
   //assert(curBuf->getBufferSize() == bufSize);
    memcpy(audioData,curBuf->data(),numFrames*audioStream->getBytesPerFrame());
    curPipeline->end();
    return oboe::DataCallbackResult::Continue;
}

bool OboeAudioRenderNode::onError(AudioStream *stream, Result result)
{
    //TODO
    return AudioStreamErrorCallback::onError(stream, result);
}

void OboeAudioRenderNode::onErrorBeforeClose(AudioStream *stream, Result result)
{
    AudioStreamErrorCallback::onErrorBeforeClose(stream, result);
}

void OboeAudioRenderNode::onErrorAfterClose(AudioStream *stream, Result result)
{
    AudioStreamErrorCallback::onErrorAfterClose(stream, result);
}

int64_t OboeAudioRenderNode::getCurrentTime()
{
    return 0;
}


/** AudioStreamCallback Impl end*/
