//
// Created by shivaaz on 9/3/24.
//

#ifndef KALASOFT_PIANOSETTINGSCONTROL_H
#define KALASOFT_PIANOSETTINGSCONTROL_H


class IPianoSettingsControl {

public:

   virtual int getNumWhiteKeysVisible() = 0;

   virtual void incrementNumKeysVisible(int incCount) = 0;

   virtual void decrementNumKeysVisible(int decCount) = 0;

   virtual void moveKeysToRight(float whiteKeyFactor) = 0;

   virtual void moveKeysToLeft(float whiteKeyFactor)  = 0;


};


#endif //KALASOFT_PIANOSETTINGSCONTROL_H
