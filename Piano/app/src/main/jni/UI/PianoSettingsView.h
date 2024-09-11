//
// Created by shivaaz on 9/2/24.
//

#ifndef KALASOFT_PIANOSETTINGSVIEW_H
#define KALASOFT_PIANOSETTINGSVIEW_H

#include <KSUI/GL/GLView.h>
#include <KSUI/ViewGroup.h>
#include "PianoView.h"
#include <KSUI/ClickListener.h>
#include <string>


class PianoSettingsView : public ViewGroup,public IPianoSettingsListener{


public:

    PianoSettingsView(){};

    ~PianoSettingsView() override;

     //void draw() override;

     //Do after the bounds are set;
     void prepare();

     void setBounds(float startX, float startY,float width,float height) override;

     void setPianoControl(IPianoSettingsControl*  control){
         this->settingsControl = control;
         control->setSettingsListener(this);
     }

     void onKeyGeometryChanged() override;

private:



private:

    GLImageView keyCountDecView,keyCountIncView,keyPositionRightView,keyPositionLeftView,settingFrameView;

    GLAlphaBlockImageView allKeysView;

    RectView keyPositionModifierView;//This is not needed just forward touch events to allKeysView;

    IPianoSettingsControl *settingsControl = nullptr;

    friend class PianoSettingsItemClickListener;
    friend class KeyPositionModifierTouchListener;

    float positionModifierMovedDistance;


};


class PianoSettingsItemClickListener : public ClickListener{


public:

    PianoSettingsItemClickListener(std::string psettingName, IPianoSettingsControl *control): settingName(psettingName), setting(control){}

    std::string getSettingName(){return settingName;}

protected:


    bool onClick() override;

    std::string settingName = "none";

    IPianoSettingsControl *setting = nullptr;



};


class KeyPositionModifierTouchListener : public  View::TouchListener
{

public:

    KeyPositionModifierTouchListener(IPianoSettingsControl *settingCtrl){this->settingsControl = settingCtrl; }

protected:

    ~KeyPositionModifierTouchListener() override;

    bool onTouch(const ks::MotionEvent &event, View *view) override;

    bool onTouchDown(const float &x, const float &y, const ks::TouchID &id,
                     const bool &isPrimary) override;

    bool
    onTouchUp(const float &x, const float &y, const ks::TouchID &id, const bool &isLast) override;

    bool onMove(const float &x, const float &y, const ks::TouchID &id) override;

    bool onHoverExit(const ks::TouchID &id, const float &x, const float &y) override;

    bool onHoverEnter(const float &d, const float &d1, const ks::TouchID &i) override;


private:

    float initialX, initialY;
    ks::TouchID id = INT_MIN;
    bool bPointerActive = false;
    IPianoSettingsControl *settingsControl = nullptr;

};

#endif //KALASOFT_PIANOSETTINGSVIEW_H
