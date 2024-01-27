//
// Created by shivaaz on 1/5/23.
//

#include "SoundBank.h"
#include "../Codec/WavDecoder.h"
#define LOGTAG "SoundBank"

int SoundBank::loadInstrument(AssetManager *assetManager, std::string instrumentName)
{
    //TODO later do based on instrument name and iteration ,also unload previous instrument;
    //For now piano so 88 but based on app modify aptly
    int count = 88;
    this->instrumentName = instrumentName;
    instrumentSounds.resize(count);
    std::string instrumentDir = instrumentsPath + "/" + instrumentName + "/";
    std::string samplesPath;
    for(int i = 0 ;i < count; ++i)
    {
        samplesPath = instrumentDir + std::to_string(i) + ".wav";
        instrumentSounds[i].samples = WavDecoder::decodeSimpleAudio(assetManager->openAsset(samplesPath.c_str()));
        assert(instrumentSounds[i].samples != nullptr);
        //if one fails Delete all ? and return fail?
    }

    audioFormat = instrumentSounds[0].samples != nullptr ? instrumentSounds[0].samples->getAudioFormat() : audioFormat;
    return 0;
}


void SoundBank::unloadInstrument()
{
  //TODO delete current instrumentSounds;
}

int SoundBank::generateSamples(AudioSamples *samples)
{
    //MIXER
    std::lock_guard<std::mutex> lock(mutNotes);
    if(!notes.size())//TODO wait sometime with conditional?
    {
        KSLOGD(LOGTAG,"No notes are on");
        return 0;
    }

    //16 bit mixer only now ;
    int frameCnt = samples->getSampleCnt();
    //samples are set to silence before this
    int16_t *outBuf =(int16_t *) samples->data();
    int noteId;
    int16_t *inputBuf;
    int mixCnt = 1;
    int maxFillCnt = 0;
    int32_t currentVal = 0;

    int frameNo = 0;
    //assumes samples required and loaded sound samples are same format;
    for(auto iter = notes.begin() ; iter != notes.end() ;)
    {
        KSLOGD(LOGTAG,"boom size %d",notes.size());
        noteId = *iter;
        int16_t *inBuf = (int16_t *) instrumentSounds[noteId].samples->data();
        auto &in = instrumentSounds[noteId];

        bool bIter = true;
        for (frameNo = 0; frameNo < samples->getSampleCnt() * 2; frameNo += 2)
        {
            outBuf[frameNo] += (inBuf[in.curSampleNo] / (int16_t )notes.size());
            outBuf[frameNo + 1] += (inBuf[in.curSampleNo + 1] /(int16_t) notes.size());
            in.curSampleNo += 2;
            if (in.curSampleNo >= (in.samples->getSampleCnt() * 2)-1)//TODO verify
            {
                KSLOGD(LOGTAG,"boom notes");
                in.resetPosition();
                iter = notes.erase(iter);
                bIter = false;
                KSLOGD(LOGTAG,"boom notes");
                break;
            }
        }

        if(bIter)
            ++iter;


        maxFillCnt = std::max(maxFillCnt, frameNo + 1);
        KSLOGD(LOGTAG,"boom notes2");

    }
    return maxFillCnt;

}

void SoundBank::setNoteOn(NoteId noteId)
{
    //TODO noteOn noteOff will block during generateSamples //maybe use msgQueue;
    KSLOGD(LOGTAG,"setNoteOn %d",noteId);

    //TODO if already On?bounds check
    std::lock_guard<std::mutex> lock(mutNotes);
    instrumentSounds[noteId].resetPosition();
    notes.insert(noteId);
}

void SoundBank::setNoteOff(NoteId noteId)
{
    KSLOGD(LOGTAG,"setNoteOff %d",noteId);
     std::lock_guard<std::mutex> lock(mutNotes);
     if(notes.size() > 3)
     {
         //if > 3 the first Msg whose off was set should be turned off not the current note
         instrumentSounds[noteId].resetPosition();
         notes.erase(noteId);
     }
    //instrumentSounds[noteId].resetPosition();
   // notes.erase(noteId);
}
