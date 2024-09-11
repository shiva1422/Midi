//
// Created by shivaaz on 9/2/24.
//

#include "PianoSettingsView.h"

#define TAGLOG "PianoSettingsView"
PianoSettingsView::~PianoSettingsView() {

}



/*void PianoSettingsView::draw() {

    ViewGroup::draw();
    //TODO can draw all in single ;
}*/

void PianoSettingsView::prepare() {


    settingFrameView.setImage("icons/midframe.png");
    allKeysView.setImage("icons/allkeys.jpg");
    keyCountDecView.setImage("icons/keycount_dec.png");
    keyCountIncView.setImage("icons/keycount_inc.png");
    keyPositionLeftView.setImage("icons/keymove_left.png");
    keyPositionRightView.setImage("icons/keymove_right.png");
    keyPositionModifierView.setBackgroundColor(0.0,0.0,0.0,0.0);


    keyCountIncView.setClickListener(new PianoSettingsItemClickListener("INCKEYCOUNT",settingsControl));
    keyCountDecView.setClickListener(new PianoSettingsItemClickListener("DECKEYCOUNT",settingsControl));
    keyPositionLeftView.setClickListener(new PianoSettingsItemClickListener("MOVELEFT",settingsControl));
    keyPositionRightView.setClickListener(new PianoSettingsItemClickListener("MOVERIGHT",settingsControl));
    keyPositionModifierView.setTouchListener(new KeyPositionModifierTouchListener(settingsControl));
    allKeysView.setAlphaBounds(0.0,keyPositionModifierView.getStartX(),keyPositionModifierView.getStartY(),keyPositionModifierView.getWidth(),keyPositionModifierView.getHeight());



    addView(&settingFrameView);
    addView(&allKeysView);
    addView(&keyCountDecView);
    addView(&keyPositionLeftView);
    addView(&keyPositionRightView);
    addView(&keyCountIncView);
    addView(&keyPositionModifierView);//TODO no need to add this just forward touch event to allKeysView, blocks;


    //KeyPositionModifier;//TODO

}

void PianoSettingsView::setBounds(float startX, float startY, float width, float height) {
    ViewGroup::setBounds(startX, startY, width, height);
    //ViewGroup should handle children layout based on the params;

    settingFrameView.setBounds(startX,startY,width,height);
    float akh =  height*2.8/4;
    allKeysView.setBounds(startX + width * 20.0/100, startY + (height - akh)/2.0, width * 60.0/100, akh);
    keyPositionModifierView.setBounds(allKeysView.getStartX(),startY,allKeysView.getWidth()/2,allKeysView.getHeight());
    allKeysView.setAlphaBounds(0.0,keyPositionModifierView.getStartX(),keyPositionModifierView.getStartY(),keyPositionModifierView.getWidth(),keyPositionModifierView.getHeight());


    float availableWidthForButton = 0.1 * width;
    float wdp = View::dispMetrics.pixelToDP(availableWidthForButton,EScreenDirection::X);
    float hdp = View::dispMetrics.pixelToDP(height,EScreenDirection::Y);

    float dpSized = std::min(wdp,hdp);
    float w = View::dispMetrics.DPToPixel(dpSized,EScreenDirection::X);
    float h = View::dispMetrics.DPToPixel(dpSized,EScreenDirection::Y);
    float sY =  startY + (height - h)/2.0;


    keyPositionLeftView.setBounds(allKeysView.getStartX()-w-5, sY, w, h);
    keyPositionRightView.setBounds(allKeysView.getEndX() + 5, sY, w, h);

    keyCountIncView.setBounds(keyPositionRightView.getEndX() + 10,sY,w,h);
    keyCountDecView.setBounds(keyPositionLeftView.getStartX()-w-10,sY,w,h);


}


void PianoSettingsView::onKeyGeometryChanged() {

    if (settingsControl)
    {
        KSLOGD("PianoSettings","KeyGeometryChanged %d",settingsControl->getNumWhiteKeysVisible());
        positionModifierMovedDistance = settingsControl->keyTranslateXFactor * allKeysView.getWidth()/(MAX_WHITEKEY_COUNT);
        keyPositionModifierView.setBounds(allKeysView.getStartX()+positionModifierMovedDistance,allKeysView.getStartY(),(allKeysView.getWidth()*settingsControl->numWhiteKeysVisible)/MAX_WHITEKEY_COUNT,allKeysView.getHeight());
        allKeysView.setAlphaBounds(allKeysView.getBlockAlpha(),keyPositionModifierView.getStartX(),keyPositionModifierView.getStartY(),keyPositionModifierView.getWidth(),keyPositionModifierView.getHeight());

    }else
    {
        KSLOGE("PianoSettingView","settings Control not set");
    }


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

    return true;
}

KeyPositionModifierTouchListener::~KeyPositionModifierTouchListener() {

}

bool KeyPositionModifierTouchListener::onTouch(const ks::MotionEvent &event, View *view) {
    return false;
}

bool
KeyPositionModifierTouchListener::onTouchDown(const float &x, const float &y, const ks::TouchID &id,const bool &isPrimary) {

    KSLOGD(TAGLOG,"TOuchDown");
    if(bPointerActive)return false;

    initialX = x;
    initialY = y;
    this->id = id;
    bPointerActive = true;
    return true;
}

bool KeyPositionModifierTouchListener::onTouchUp(const float &x, const float &y, const ks::TouchID &id,const bool &isLast) {

    KSLOGD(TAGLOG,"TOuchUP");

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

    KSLOGD(TAGLOG,"move");
    if(id != this->id)
    {
        KSLOGE(TAGLOG,"Impossible");
        return false;
    }
    PianoSettingsView *settingsView = dynamic_cast<PianoSettingsView *>(this->view->getParent());
    if(settingsView != nullptr  && settingsControl)
    {
        float keyWidth = settingsView->allKeysView.getWidth()/MAX_WHITEKEY_COUNT;
        float moveDistance = (x-initialX)/ keyWidth;
        settingsControl->moveKeysToRight(-moveDistance);
        initialX = x;

    }else
    { KSLOGE(TAGLOG,"Position Modifier parent or settingCtrl error");}


    return true;
}

//Dragging outside
bool KeyPositionModifierTouchListener::onHoverExit(const ks::TouchID &id, const float &x,
                                                   const float &y) {
    KSLOGD(TAGLOG, "hoverExit");

    return onMove(x,y,id);
}

bool KeyPositionModifierTouchListener::onHoverEnter(const float &d, const float &d1,const ks::TouchID &i) {

    KSLOGD(TAGLOG, "hoverEnter");

    return TouchListener::onHoverEnter(d, d1, i);
}
