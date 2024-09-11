//
// Created by shivaaz on 9/3/24.
//

#ifndef KALASOFT_PIANOSETTINGSCONTROL_H
#define KALASOFT_PIANOSETTINGSCONTROL_H


class IPianoSettingsListener{

public:

    virtual void onKeyGeometryChanged() = 0;

};



class IPianoSettingsControl {

public:

   virtual int getNumWhiteKeysVisible() = 0;

   virtual void incrementNumKeysVisible(int incCount) = 0;

   virtual void decrementNumKeysVisible(int decCount) = 0;

   virtual void moveKeysToRight(float whiteKeyFactor) = 0;

   virtual void moveKeysToLeft(float whiteKeyFactor)  = 0;

   virtual void setSettingsListener(IPianoSettingsListener *listener){this->settingsListener = listener;}

protected:

    IPianoSettingsListener *settingsListener = nullptr;

    //WhiteKeys
    int numWhiteKeysVisible = 15;

    float whiteKeyWidth = 50;//px

    float blackKeyWidth = 0.6 * whiteKeyWidth;

    float keyGap = 1.0;//px

    float globalKeyTranslateX = 0;//in pixels

    //num of times(float) white key width the translate  will be calculated from;like when moving keys using positionmodifier
    float keyTranslateXFactor = 20.0;

    float keyPositionModifierOffset = 0.0;//The offset of KeyPositionModifierView from keyPositionModifierViewFull.startX(); calculated from keyTranslateXFactor

    friend class PianoSettingsView;

};



#endif //KALASOFT_PIANOSETTINGSCONTROL_H
