//
// Created by shivaaz on 2/11/23.
//

#include <unistd.h>
#include <thread>
#include "MidiPlayer.h"

int MidiPlayer::init()
{
    return -1;
}

int MidiPlayer::play()
{
    playerThread = std::make_shared<std::thread>(&MidiPlayer::run, this);
    return playerThread == nullptr ? -1: 0;
}

int MidiPlayer::pause()
{
    return -1;
}

int MidiPlayer::resume()
{
    return -1;
}

int MidiPlayer::seek(int64_t ms)
{
    return -1;
}

int MidiPlayer::stop()
{
    return -1;
}

int MidiPlayer::refresh()
{
 return -1;
}

int MidiPlayer::setMediaSource(const char *source)
{
    return 0;
}

int MidiPlayer::setMediaSource(const IKSStream *source)
{
    //TODO time consu

    file = ks::MidiFile(source);
    iterator = file.trackInfos[trackNo].events.begin();
    previousEventTime = std::chrono::duration_cast<std::chrono::microseconds>(m_clock.now().time_since_epoch()).count();
    return 0;
}

int64_t MidiPlayer::getCurrentTime(EMediaType mediaType)
{
    return 0;
}

int64_t MidiPlayer::getDuration(EMediaType mediaType)
{
    return 0;
}


void MidiPlayer::run()
{
    //while !exit;
    while(true)
    {
        usleep(2000);

        //event deltatime in US = track.tempo  * event.deltatime(deltaTicks) / midifile.tickRate
        //TODO Crash trackNO set coorect if single track 0;
        KSLOGD("MIDIPlayer","refresh");

        if(iterator != file.trackInfos[trackNo].events.end())
            do
            {
                KeyStateInfo stateInfo = iterator->getKeyState();
                if(stateInfo.keyName != INVALID )
                {
                    int32_t deltaTicks = iterator->getEventDeltaTicks();
                   // int64_t waitTimeUS = (500000 * deltaTicks)/file.tickRate;//TODO
                    int64_t waitTimeUS = file.trackInfos[trackNo].tempo  * deltaTicks/file.tickRate;

                    float kMillisecondsPerQuarterNote =  file.trackInfos[trackNo].tempo/ 1000.0f;
                    float kMillisecondsPerTick = kMillisecondsPerQuarterNote / file.tickRate;
                    float deltaTimeInMilliseconds = deltaTicks * file.tickRate;

                    double USPerTick = (file.trackInfos[trackNo].tempo / file.tickRate);
                    //
                    waitTimeUS = USPerTick * deltaTicks;

                    auto now = std::chrono::duration_cast<std::chrono::microseconds>
                            (m_clock.now().time_since_epoch()).count();
                    auto elasped = now - previousEventTime;
                   // waitTimeUS = waitTimeUS - elasped;

                    KSLOGD("MIDIPlayer","wait time %ld waittime %ld",now-previousEventTime,waitTimeUS);
                    if(elasped <= waitTimeUS)
                    {
                        usleep(waitTimeUS - elasped);
                       // break;//any onw
                    }

                    KSLOGD("MIDIPlayer","wait end");
                    pianoController->setKeyState(stateInfo.keyName,stateInfo.keyState);
                    previousEventTime = std::chrono::duration_cast<std::chrono::microseconds>(m_clock.now().time_since_epoch()).count();
                }
                else
                {KSLOGE("MidiPlayer","error invalid event");}
                iterator++;
                if(iterator->getEventDeltaTicks() > 0)
                    break;

            }while(iterator != file.trackInfos[trackNo].events.end());

        KSLOGD("MIDIPlayer","refresh end");

      //  return 0;

    }
}
