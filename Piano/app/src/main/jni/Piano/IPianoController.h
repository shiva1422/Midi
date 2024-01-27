//
// Created by shivaaz on 12/18/22.
//

#ifndef KALASOFT_IPIANOCONTROLLER_H
#define KALASOFT_IPIANOCONTROLLER_H

#include "PianoCore.h"

class IPianoController {

public:

    virtual ~IPianoController(){}

    //TODO later combine all keyInfo into one
    virtual void setKeysStates(const std::vector<KeyStateInfo> states) = 0;

    void setKeyState(const KeyStateInfo state){ setKeyState(state.keyName,state.keyState);}

    virtual void setKeyState(const EKeyName name,const EKeyState state) = 0;

    IPianoController *getController(){return this;}

     void connectController(IPianoController *pianoController){this->controller = pianoController;}

protected:
    //connected controller
    IPianoController* controller = nullptr;
};


#endif //KALASOFT_IPIANOCONTROLLER_H
