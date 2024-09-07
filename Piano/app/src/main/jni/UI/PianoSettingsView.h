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


class PianoSettingsView : public ViewGroup{


public:

    PianoSettingsView(){};

    ~PianoSettingsView() override;

     void draw() override;

     //Do after the bounds are set;
     void prepare();

     void setBounds(float startX, float startY,float width,float height) override;

     void setPianoControl(IPianoSettingsControl*  control){ this->settingsControl = control;}

private:

    void incrementKeyCount(int incCount);

    void decrementKeyCount(int decCount);

    void moveKeysRight(float whiteKeyFactor);

    void moveKeysLeft(float whiteKeyFactor);


private:

    GLImageView keyCountDecView,keyCountIncView,keyPositionRightView,keyPositionLeftView,settingFrameView,allKeysView;

    RectView keyPositionModifierView;

    IPianoSettingsControl *settingsControl = nullptr;

    friend class PianoSettingsItemClickListener;


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

};

#endif //KALASOFT_PIANOSETTINGSVIEW_H
