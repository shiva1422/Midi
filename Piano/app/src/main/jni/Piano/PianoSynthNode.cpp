//
// Created by shivaaz on 11/14/22.
//

#include "PianoSynthNode.h"
#define TAGLOG "PianoSynthNode"
#include "../Midi/MidiFile.h"
#include "../GN/IMediaNode.h"
#include "../GN/MediaEvent.h"


PianoSynthNode::PianoSynthNode()
{

}

PianoSynthNode::~PianoSynthNode()
{
  //clear piplelines;audio samples
  synthThread.join();
}
void PianoSynthNode::setKeysStates(const std::vector<KeyStateInfo> states)
{
    //todo
}
void PianoSynthNode::setKeyState(const EKeyName name, const EKeyState state)
{
  if(state == EKeyState::KEYSTATE_ON)
      soundBank.setNoteOn(name);
  else
      soundBank.setNoteOff(name);
}

/**IMediaNode Impl*/

ENodeRes PianoSynthNode::prepare()
{
    ENodeRes res = static_cast<ENodeRes>(soundBank.loadInstrument(assetManager, "default"));
    audioFormat = soundBank.getAudioFormat();
    for(int i = 0 ; i< pipelineCnt ;++i)
    {
        onPipelineEnd(allocate());
    }
    return res;
}

ENodeRes PianoSynthNode::start()
{
    if(!bThreadCreated)
    {
        synthThread = std::thread(&PianoSynthNode::run, this);
        bThreadCreated = true;
        KSLOGD(TAGLOG,"synthThread Created");

    }
    KSLOGD(TAGLOG,"synthStarted");
    return ENodeRes_OK;
}

ENodeRes PianoSynthNode::pause()
{
    MediaEvent event;
    event.mEvent = EMediaEvent::PAUSE;
    std::lock_guard<std::mutex> guard(mutEvents);
    events.push(event);
    return ENodeRes_OK;
}

ENodeRes PianoSynthNode::stop()
{
    bExitThread = true;
    return ENodeRes_OK;
}

ENodeRes PianoSynthNode::release()
{
   return ENodeRes_OK;
}

ENodeRes PianoSynthNode::onPipelineInput(MediaPipeline *pipleline)
{

    /*TODO this Does
     * mix audio of framesPerCallback from pianoState and SoundBank/instrument
     * forward to next node of
     */

    if(!pipleline->samples)
    {
        KSLOGD(TAGLOG,"oboe %d framesPerCallback",framesPerCallback);
        pipleline->samples = new AudioSamples(audioFormat,framesPerCallback);
    }
    pipleline->samples->muteSamples();
    soundBank.generateSamples(pipleline->samples);

 /*
  const float TWO_PI 6.28;

  static float phase = 0;
    int16_t *audio = (int16_t *) pipleline->samples->data();
    for (int i = 0; i < framesPerCallback * 2; i += 2) {

        phase += TWO_PI * 6000.0 / 44100;
        if (phase > TWO_PI)
            phase -= TWO_PI;
        audio[i] =   INT16_MAX * sin(phase);
        audio[i + 1] = audio[i];
    }
    */
   // pipleline->samples->setToSilence();
   return onPipelineOutput(pipleline);
}

ENodeRes PianoSynthNode::onPipelineOutput(MediaPipeline *pipleline)
{
   return nextNode->onPipelineInput(pipleline);
}

/**IMediaNode Impl end*/


MediaPipeline *PianoSynthNode::allocate()
{
   return new MediaPipeline(this);
}

void PianoSynthNode::onPipelineEnd(MediaPipeline *pipeline)
{
  std::lock_guard<std::mutex> lock(mutPipeline);
  pipelineQ.push(pipeline);
  condPipeline.notify_one();
}

void PianoSynthNode::run()
{
    /**
    * What does this do?
    * Read and process any Events,Where do Events Come from(Midi/Piano Controller/player?
    * getNewPipeline and onPipelineInput
    */
    MediaPipeline *p = nullptr;
    bPaused = false;

    while(!bExitThread)
    {
        processEvent();
        {
            std::unique_lock<std::mutex> lock{mutPipeline};
            condPipeline.wait(lock, [&] { return !pipelineQ.empty(); });//Timed:there might be events?
            p = pipelineQ.front();
            pipelineQ.pop();
        }
        onPipelineInput(p);
    }

}

void PianoSynthNode::processEvent()
{
    //TODO multiple events;
    MediaEvent event;//cache
    event.mEvent = EMediaEvent::NONE;
    {
        std::lock_guard<std::mutex> guard(mutEvents);
        if (events.size()) {
            event = events.front();
            events.pop();
        }
    }

    if(event.mEvent != EMediaEvent::NONE)
    {
        switch (event.mEvent)
        {
            case EMediaEvent::PAUSE:
                bPaused = true;
                KSLOGD(TAGLOG,"Pausing");
                break;

            default:
                KSLOGW(TAGLOG,"Unknown Event");



        }
    }


}

int64_t PianoSynthNode::getCurrentTime()
{
    return 0;
}



