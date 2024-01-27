//
// Created by shivaaz on 12/15/22.
//

#ifndef DIGIBANDPIANO_RAINBOWPIANOTOUCHLISTENER_H
#define DIGIBANDPIANO_RAINBOWPIANOTOUCHLISTENER_H

#include <KSUI/View.h>
#include "RainbowPianoView.h"


//TODO extend this as PianoController as this controls piano
class RainbowPianoTouchListener : public View::TouchListener{


protected:

    bool onTouchDown(const float &x, const float &y, const ks::TouchID &id,const bool &isPrimary) override;

    bool onTouchUp(const float &x, const float &y, const ks::TouchID &id, const bool &isLast) override;

    bool onMove(const float &x, const float &y, const ks::TouchID &id) override;

    //TODO dont used this
    bool onTouch(const ks::MotionEvent &event, View *view) override;

    bool onHoverExit(const ks::TouchID &id) override;

private:

    //store key pointed by touch pointer id ,//TODO this assumes pointerId is always less than 50 need verify;
    EKeyName previousPointedKey[50];//max of 50 finger movements too many
};


#endif //DIGIBANDPIANO_RAINBOWPIANOTOUCHLISTENER_H
