//
// Created by shivaaz on 1/5/23.
//

#ifndef DIGIBANDPIANO_SOUNDBANK_H
#define DIGIBANDPIANO_SOUNDBANK_H

#include <mutex>
#include <CMedia/EMedia.h>
#include "KSIO/AssetManager.h"
#include "string"
#include "../Audio/AudioSamples.h"
#include "vector"
#include "unordered_set"

typedef int NoteId;

class RingBufferedSamples{

    friend class SoundBank;

public:

    KSFORCEINLINE void setSamples(AudioSamples *samples){this->samples = samples;curSampleNo = 0;remainingSamples = samples->getSampleCnt();}

    KSFORCEINLINE void resetPosition()
    {
        curSampleNo = 0 ;
        if(samples)
            remainingSamples = samples->getSampleCnt();
        else
            remainingSamples = 0;
    }
private:

    AudioSamples *samples = nullptr;

    int curSampleNo = 0;

    int remainingSamples = 0;
};

//TODOo Move Out

class ISamplesGenerator{

public:

    virtual int generateSamples(AudioSamples *samples) = 0;

};
//TODO convert to Instrument
class SoundBank : public ISamplesGenerator{


public:

    int loadInstrument(AssetManager *assetManager, std::string instrumentName);

    void unloadInstrument();

    ks::AudioFormat getAudioFormat(){return audioFormat;}

    //ISamplesGenerator

    int generateSamples(AudioSamples *samples) override;

    void setNoteOn(NoteId noteId);

    void setNoteOff(NoteId noteId);

private:

    std::string instrumentName;

    //in assets.To load instrument ,the path would would be "instruments/instrumentName/soundNo.wav;
    std::string instrumentsPath = "music/instruments";



    //the vector of n size contains n audio buffers (sounds) of an instrument .eg piano has 88 keys so each element maps to one piano key sound
    std::vector<RingBufferedSamples> instrumentSounds;

    std::unordered_set<NoteId> notes;//set of sounds that are on for now

    //mutex for notes and instrumentSounds;
    std::mutex mutNotes;//TODO read write locks?

    ks::AudioFormat audioFormat ;

};


#endif //DIGIBANDPIANO_SOUNDBANK_H
