//
// Created by shivaaz on 9/2/24.
//

#include "PianoSettingsView.h"

#define TAGLOG "PianoSettingsView"
PianoSettingsView::~PianoSettingsView() {

}



void PianoSettingsView::draw() {

    ViewGroup::draw();
    //TODO can draw all in single ;
}

void PianoSettingsView::prepare() {


    settingFrameView.setImage("icons/midframe.png");
    allKeysView.setImage("icons/allkeys.jpg");
    keyCountDecView.setImage("icons/keycount_dec.png");
    keyCountIncView.setImage("icons/keycount_inc.png");
    keyPositionLeftView.setImage("icons/keymove_left.png");
    keyPositionRightView.setImage("icons/keymove_right.png");
    keyPositionModifierView.setBackgroundColor(0.0,0.0,0.0,0.4);

    keyCountIncView.setClickListener(new PianoSettingsItemClickListener("INCKEYCOUNT",settingsControl));
    keyCountDecView.setClickListener(new PianoSettingsItemClickListener("DECKEYCOUNT",settingsControl));
    keyPositionLeftView.setClickListener(new PianoSettingsItemClickListener("MOVELEFT",settingsControl));
    keyPositionRightView.setClickListener(new PianoSettingsItemClickListener("MOVERIGHT",settingsControl));
    keyPositionModifierView.setTouchListener(new KeyPositionModifierTouchListener());



    addView(&settingFrameView);
    addView(&allKeysView);
    addView(&keyCountDecView);
    addView(&keyPositionLeftView);
    addView(&keyPositionRightView);
    addView(&keyCountIncView);
    addView(&keyPositionModifierView);


    //KeyPositionModifier;//TODO

}

void PianoSettingsView::setBounds(float startX, float startY, float width, float height) {
    ViewGroup::setBounds(startX, startY, width, height);
    //ViewGroup should handle children layout based on the params;

    settingFrameView.setBounds(startX,startY,width,height);
    allKeysView.setBounds(startX + width * 20.0/100, startY, width * 60.0/100, height/2);
    keyPositionModifierView.setBounds(allKeysView.getStartX(),startY,allKeysView.getWidth()/2,allKeysView.getHeight());

    keyPositionLeftView.setBounds(allKeysView.getStartX()-allKeysView.getHeight()-60, allKeysView.getStartY(), allKeysView.getHeight() * 2, height);
    keyPositionRightView.setBounds(allKeysView.getEndX()+10, keyPositionLeftView.getStartY(), keyPositionLeftView.getWidth(), keyPositionLeftView.getHeight());

    keyCountIncView.setBounds(keyPositionRightView.getEndX() + 10,keyPositionRightView.getStartY(),keyPositionRightView.getWidth(),keyPositionRightView.getHeight());
    keyCountDecView.setBounds(keyPositionLeftView.getStartX()-keyPositionLeftView.getWidth()-10,keyCountIncView.getStartY(),keyCountIncView.getWidth(),keyCountIncView.getHeight());


}

void PianoSettingsView::incrementKeyCount(int incCount) {

    KSLOGD("PianoSettings View","inc keyCount");
}

void PianoSettingsView::decrementKeyCount(int decCount) {

}

void PianoSettingsView::moveKeysRight(float whiteKeyFactor) {

}

void PianoSettingsView::moveKeysLeft(float whiteKeyFactor) {

}

//Touch


bool PianoSettingsItemClickListener::onClick() {

    KSLOGD(TAGLOG,"On Click %p %s",this->view,getSettingName().c_str());


    if(settingName == "INCKEYCOUNT"){

        if(setting)
        {
            setting->incrementNumKeysVisible(1.0);//one white key
        }

    }
    else if(settingName == "DECKEYCOUNT"){

        if(setting)
        {
            setting->decrementNumKeysVisible(1.0);//one white key
        }
    }
    else if(settingName == "MOVELEFT")
    {
        if(setting)
        {
            setting->moveKeysToLeft(0.5);//one white key
        }
    }else if(settingName == "MOVERIGHT")
    {
        if(setting)
        {
            setting->moveKeysToRight(-0.5);//one white key
        }
    }

    return false;
}

KeyPositionModifierTouchListener::~KeyPositionModifierTouchListener() {

}

bool KeyPositionModifierTouchListener::onTouch(const ks::MotionEvent &event, View *view) {
    return false;
}

bool
KeyPositionModifierTouchListener::onTouchDown(const float &x, const float &y, const ks::TouchID &id,const bool &isPrimary) {

    if(bPointerActive)return false;

    initialX = x;
    initialY = y;
    this->id = id;
    bPointerActive = true;
    return true;
}

bool KeyPositionModifierTouchListener::onTouchUp(const float &x, const float &y, const ks::TouchID &id,const bool &isLast) {

    if(this->id == id)
    {
        KSLOGD(TAGLOG, "up");
        bPointerActive = false;
        this->id = INT_MIN;
        return true;
    }

    return TouchListener::onTouchUp(x, y, id, isLast);
}

bool KeyPositionModifierTouchListener::onMove(const float &x, const float &y, const ks::TouchID &id) {

    KSLOGD(TAGLOG, "Moving");
    return true;
}

bool KeyPositionModifierTouchListener::onHoverExit(const ks::TouchID &id, const float &x,
                                                   const float &y) {
    KSLOGD(TAGLOG, "hoverExit");

    return true;
}

bool KeyPositionModifierTouchListener::onHoverEnter(const float &d, const float &d1,const ks::TouchID &i) {

    KSLOGD(TAGLOG, "hoverEnter");

    return TouchListener::onHoverEnter(d, d1, i);
}
