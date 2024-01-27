//
// Created by shivaaz on 2/11/23.
//

#ifndef DIGIBANDPIANO_MIDIPLAYER_H
#define DIGIBANDPIANO_MIDIPLAYER_H

#include <CMedia/EMedia.h>
#include "MidiFile.h"
#include "../Piano/IPianoController.h"
#include "chrono"
class MidiPlayer {

public:

    int init() ;

    int play() ;

    int pause() ;

    int resume() ;

    int seek(int64_t ms) ;

    int stop() ;

    int refresh() ;

    int setMediaSource(const char *source) ;

    int setMediaSource(const IKSStream *source) ;

    int64_t getCurrentTime(EMediaType mediaType) ;

    int64_t getDuration(EMediaType mediaType) ;


public:

    void setListenerPiano(IPianoController *controller){this->pianoController = controller;}


private:

    void run();

private:

    int64_t previousEventTime = 0;
    std::vector<ks::MidiEvent>::iterator iterator;

    std::chrono::high_resolution_clock m_clock;

    ks::MidiFile file;
    int trackNo = 0;

    //right now only control from UI thread.
    IPianoController *pianoController = nullptr;
    std::shared_ptr<std::thread> playerThread = nullptr;

};


#endif //DIGIBANDPIANO_MIDIPLAYER_H
