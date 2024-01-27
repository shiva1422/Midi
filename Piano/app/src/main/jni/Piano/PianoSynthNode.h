//
// Created by shivaaz on 11/14/22.
//

#ifndef KALASOFT_PIANOSYNTHNODE_H
#define KALASOFT_PIANOSYNTHNODE_H

#include <vector>
#include <thread>
#include "PianoCore.h"
#include "PianoState.h"
#include "IPianoController.h"
#include "../GN/IMediaNode.h"
#include "../Music/SoundBank.h"

class PianoSynthNode : public  IPianoController, public IMediaNode, public IPipelineAllocator, public ks::AudioInterface{

public:

    PianoSynthNode();

    ~PianoSynthNode() override;

    /***IPianoController***/
    void setKeysStates(const std::vector<KeyStateInfo> states) override;

    void setKeyState(const EKeyName name, const EKeyState state) override;


    /**IMediaNode Impl*/

    ENodeRes prepare() override;

    ENodeRes start() override;

    ENodeRes pause() override;

    ENodeRes stop() override;

    ENodeRes release() override;

    ENodeRes onPipelineInput(MediaPipeline *pipleline) override;

    ENodeRes onPipelineOutput(MediaPipeline *pipleline) override;

    /**  IPipelineAllocator*/

    MediaPipeline *allocate() override;

    void onPipelineEnd(MediaPipeline *pipeline) override;

    /** */
    void setAssetManager(AssetManager *assetManager){this->assetManager = assetManager;}


    int64_t getCurrentTime() override;


private:

    void run();

    //TODO convert this to eventProcessor
    void processEvent();

private:

   // ks::Thread synthThread;
   std::thread synthThread;

    bool bExitThread = false;

private:
    SoundBank soundBank;

    AssetManager * assetManager;

    int pipelineCnt = 2;//based on audio rendering might need to incraese to avoid underruns
    std::queue<MediaPipeline *> pipelineQ;
    std::mutex mutPipeline;
    std::condition_variable condPipeline;


};


#endif //KALASOFT_PIANOSYNTHNODE_H
