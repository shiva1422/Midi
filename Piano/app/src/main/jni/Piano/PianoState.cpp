//
// Created by shivaaz on 12/2/22.
//

#include "PianoState.h"
#include "vector"

PianoState::PianoState()
{

}

void PianoState::updateKeySStates(const std::vector<KeyStateInfo> &states)
{
    std::lock_guard<std::mutex> lock(stateMutex);
    for(auto state : states)
    {
        keyStates[state.keyName] = state.keyState;
    }

    stateChanged = true;
    //update stateversion later if required;
}

void PianoState::updateKeyState(const EKeyName &name, const EKeyState &state)
{
    std::lock_guard<std::mutex> lock(stateMutex);
    keyStates[name] = state;
    stateChanged = true;
}

