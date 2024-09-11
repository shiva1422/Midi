//
// Created by shivaaz on 12/18/22.
//

#include "PianoController.h"
#include "Piano/PianoSynthNode.h"
#include "Codec/WavDecoder.h"

#define TAGLOG "pianoController"

//TODO check if better to SetSynth first and UI next for latency
PianoController::PianoController()
{
    synthController = new PianoSynthNode();
}

void PianoController::setKeysStates(const std::vector<KeyStateInfo> states)
{

    uiController->setKeysStates(states);
    synthController->setKeysStates(states);
}
void PianoController::setKeyState(const EKeyName name, const EKeyState state)
{
  uiController->setKeyState(name,state);
  synthController->setKeyState(name,state);
}

int PianoController::prepare()
{

    //TODO AudioLoadTesting

    /*
     * Load Instrument get Audio Format;
     * create OboeRenderer set AudioFormat and prepare
     * getFramesPerCallback from OboeRender and set To PianoSynth
     * in synth  create Pipelines with same AudioSamples Size(framesPerCallback);
     */
    audioRender = std::make_shared<OboeAudioRenderNode>();
    PianoSynthNode *pSynthNode = reinterpret_cast<PianoSynthNode *>(synthController);
    pSynthNode ->setNextNode(audioRender.get());
    pSynthNode->prepare();
    ks::AudioFormat audioFormat = pSynthNode->getAudioFormat();
    audioRender->setAudioFormat(audioFormat);
    audioRender->setFramesPerCallback(128);
    audioRender->prepare();
    pSynthNode->setFramesPerCallback(128);

    pSynthNode->start();
    audioRender->start();
    return 0;
}

void PianoController::setAssetManager(AssetManager *manager)
{
  this->assetManager = manager;
  PianoSynthNode *pSynthNode = reinterpret_cast<PianoSynthNode *>(synthController);
  pSynthNode->setAssetManager(manager);

}

int PianoController::release() {
    return 0;
}

int PianoController::pause() {


    audioRender->pause();
    //pianoSynthNode->pause();

    return 0;
}

int PianoController::exit() {
    return 0;
}

int PianoController::resume() {

    audioRender->start();
    return 0;

}

