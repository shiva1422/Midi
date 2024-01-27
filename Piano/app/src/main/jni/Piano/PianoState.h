//
// Created by shivaaz on 12/2/22.
//

#ifndef DIGIBANDPIANO_PIANOSTATE_H
#define DIGIBANDPIANO_PIANOSTATE_H

#include<stdint.h>
#include <mutex>
#include "PianoCore.h"

class PianoState {
/*
 * Current state of piano synth node used to Synthesize Music ,which includes current keys on/off and key properties like gain,blend etc
 */
public:

    PianoState();

    void updateKeySStates(const std::vector<KeyStateInfo> &states);

    void updateKeyState(const EKeyName &name,const EKeyState &state );

private:

    EKeyState keyStates[PIANO_MAX_KEY_COUNT] = {static_cast<EKeyState>(0)};

    std::mutex stateMutex ;//TODO read write locks?

    //To determine if state has changed like when a key is pressed or when properties of keys change ,modify this to determine state change
    int64_t stateVersion = INT64_MIN;

    bool stateChanged = false;
};


#endif //DIGIBANDPIANO_PIANOSTATE_H
