//
// Created by shivaaz on 12/15/22.
//

#include "RainbowPianoTouchListener.h"
#include "vector"

#define LOGTAG "RainbowTouch"

//TODO extend this as PianoController instead of PianoView
//so most of below can be combined into one

bool RainbowPianoTouchListener::onTouch(const ks::MotionEvent &event, View *view)
{
//TODO not using this now
    int32_t index = event.getPointerIndex();
    int32_t id  = event.getPointerId(index);
    float pointX = event.getX(index);
    float pointY = event.getY(index);
    KSLOGD(LOGTAG,"PianoSynthNode on Touch x %f y %f ",pointX,pointY);
    RainbowPianoView *piano = static_cast<RainbowPianoView *>(view);


    //TODO maybe need to check touch event history cause when swipes speed across screen some ponts may be missiong
    //therfore some keys may not be played or events may be repeated check;
    assert(piano);
    if(piano->keysAreaView.isPointInside(pointX,pointY))
    {    //Touch action on keys
        KSLOGD(LOGTAG,"touching keysArea");
        EKeyName keyName = piano->getKeyNoAtLoc(pointX,pointY);
        piano->controller->setKeyState(keyName,EKeyState::KEYSTATE_ON);

        return true;
    }
    return false;
}

bool RainbowPianoTouchListener::onTouchDown(const float &x, const float &y, const ks::TouchID &id,const bool &isPrimary)
{
    RainbowPianoView* piano = reinterpret_cast<RainbowPianoView *>(this->view);
    if(piano->keysAreaView.isPointInside(x,y))
    {    //Touch action on keys
        KSLOGD(LOGTAG,"touching keysArea");
        EKeyName keyName = piano->getKeyNoAtLoc(x,y);
        piano->controller->setKeyState(keyName,EKeyState::KEYSTATE_ON);
        previousPointedKey[id] = keyName;

        return true;
    }
    else
    {//TODO

    }
    return false;

    //return TouchListener::onTouchDown(x, y, id);
}

bool RainbowPianoTouchListener::onTouchUp(const float &x, const float &y, const ks::TouchID &id,const bool &isLast)
{

    RainbowPianoView* piano = reinterpret_cast<RainbowPianoView *>(this->view);
    if(piano->keysAreaView.isPointInside(x,y))
    {    //Touch action on keys
        KSLOGD(LOGTAG,"touching keysArea action up");
        EKeyName keyName = piano->getKeyNoAtLoc(x,y);
        piano->controller->setKeyState(keyName,EKeyState::KEYSTATE_OFF);

        //what if touchdown/up not same key?
        previousPointedKey[id] = EKeyName::INVALID;

        return true;
    }
    else
    {//TODO
        KSLOGE(LOGTAG,"error");
    }
    return false;
    //return TouchListener::onTouchUp(x, y, id, isLast);
}

bool RainbowPianoTouchListener::onMove(const float &x, const float &y, const ks::TouchID &id)
{
    RainbowPianoView* piano = reinterpret_cast<RainbowPianoView *>(this->view);

    if(piano->keysAreaView.isPointInside(x,y))
    {
        //For some reasoin this is > 87 check with surface dimentions and dispmetric for now handled in getKeyNoAtLoc
        EKeyName keyName = piano->getKeyNoAtLoc(x,y);
        if(previousPointedKey[id] != keyName)
        {
            //swiping from one key to other
            //combine setKeyStates to one call,also better setKey on first and off next for Audio latency
            if(previousPointedKey[id] != EKeyName::INVALID)
            piano->controller->setKeyState(previousPointedKey[id],EKeyState::KEYSTATE_OFF);

            previousPointedKey[id] = keyName;
            piano->controller->setKeyState(keyName,EKeyState::KEYSTATE_ON);
            return true;

        }
        else
        {
            //same key already on,so nothing to do
            return true;
        }
    }
    else
    {
        //TODO

        //Move out of the keys area but same finger touch caused keyon before;
        if(previousPointedKey[id] != EKeyName::INVALID)
        {
            piano->controller->setKeyState(previousPointedKey[id],EKeyState::KEYSTATE_OFF);
            previousPointedKey[id] = EKeyName::INVALID;
            //TODO should return //false or handle other slider for this touch?hover?
            //shoudl remove id from touchIds[] ?
            //
        }
    }

    return false;
}


bool RainbowPianoTouchListener::onHoverExit(const ks::TouchID &id, const float &x, const float &y)
{
    //TODO also check aptly inside keys area of sliders
   //Moved out of piano

    RainbowPianoView* piano = reinterpret_cast<RainbowPianoView *>(this->view);

    if(previousPointedKey[id] != EKeyName::INVALID)
   {
       piano->controller->setKeyState(previousPointedKey[id],EKeyState::KEYSTATE_OFF);
       previousPointedKey[id] = EKeyName::INVALID;
   }

    return false;
}



    //  return TouchListener::onMove(x, y, id);


