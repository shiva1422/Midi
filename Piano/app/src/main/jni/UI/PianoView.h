//
// Created by shivaaz on 8/6/24.
//

#ifndef KALASOFT_PIANOVIEW_H
#define KALASOFT_PIANOVIEW_H

#include <KSUI/GL/GLView.h>
#include <CMedia/KSImage.h>
#include <KSIO/AssetManager.h>
#include <array>
#include "Graphics/KSUI/View.h"
#include "Logger/KSLog.h"
#include "Graphics/GLContext.h"
#include "../Text/Text.h"
#include "../Piano/PianoCore.h"
#include "../Piano/IPianoController.h"
#include "../Piano/IPianoSettingsControl.h"
#include <mutex>
#include <KSUI/GL/GLImageView.h>
#include <KSUI/GL/RectView.h>
#include <KSUI/View.h>


#define MIN_WHITEKEY_COUNT 7
#define MAX_WHITEKEY_COUNT 52
#define DEFAULT_WHITEKEY_COUNT 52

/*
    * Draw keyboard using openGL instanced rendering.
    * Total Keys 88 ,whitekeys - 52,blackkeys -36
    * A0 - C8
    */

class PianoView : public View, public IPianoController, public IPianoSettingsControl{

    //TODO Moves Shaders to here as string rather than reading from assets

public:
    /***VIEW***/

    PianoView();

    PianoView(float width, float height);

    PianoView(float startX, float startY, float width, float height);

    //TODO later make Text engine into common without explicit dependency
    PianoView(float startX, float startY, float width, float height, TextEngine *textEngine);

    ~PianoView() override;

    void setBounds(float startX, float startY, float width, float height) override;

    void clearBackground() override;

    void draw() override;

    /**
     * @param factor should be increment in terms of whitekey width , eg -1.5 implies move 1.5 white keys left,-.5 implies move 1.5 keys right.
     */
    void translateKeyXPosition(float factor);


public:

    /***IPianoController***/

    void setKeyState(const EKeyName name, const EKeyState state) override;

    void setKeysStates(const std::vector<KeyStateInfo> states) override;


    /**SetttingsControl**/

    IPianoSettingsControl * getSettingsControl(){return  this;}

    int getNumWhiteKeysVisible() override;

    void incrementNumKeysVisible(int incCount) override;

    void decrementNumKeysVisible(int incCount) override;

    void moveKeysToRight(float whiteKeyFactor) override;

    void moveKeysToLeft(float whiteKeyFactor) override;


private:

    friend class PianoApplication;

    void prepareShaders(AssetManager *assetManager);

    void setBlackKeyTranslations();

    void setupTextures();

    void setTextEngine(TextEngine *textEngine){ this->textEngine = textEngine;}

    void doPianoLayout();

    void prepare(AssetManager *assetManager);

    //only drawKeys and noteNames
    void drawKeysAndNoteNames();

private:

    GLuint keysAndNoteNamesShader = 0;

    //locations for above shader
    GLint whiteKeyVertsLoc,blackKeyVertsLoc,noteNameVertsLoc,octaveNumVertsLoc,keysAreaBoundsLoc,blackKeyTransLationXLoc,paramsLoc,isKeyOnLoc,
            textureCoordsLoc,noteNamesTexLoc;

    //2 textures for each one off and one on.
    GLint keyTexLoc;

    GLuint shader = 0;//Right now a Rainbow PianoSynth,Will change later to Textured One Should




private:

    void resizeKeyCount(int whiteKeysVisibleCnt);

    void initKeyRenderIndices();


private:

    //Textures
    KSImage* keyImages[4];//black key, blackKeyTap,whiteKey,whiteKeyTap;

    GLuint keyTextures[4];//

    GLuint noteNamesTex = 0;

    //TODO move outof this
    TextEngine *textEngine = nullptr;

    PixaMap noteNames[19];//noteName include # //TODO KSImage



private:

    //Piano All Measurements

    //To store bounds for Keys rendering area;
    GLView keysAreaView;

    GLView whiteKeyView,blackKeyView;//first white key bounds.

    GLView keySizeIncView;

    GLView  allKeysView;

    GLView noteNameView;

    GLView octaveNumView;

    //keyOn-1 off - 0
    int keyOn[PIANO_MAX_KEY_COUNT] = {0};//TODO 89?shader

    //BlackKeys are rendered as tranlated from fixed absolute position of starting key;
    float blackKeyTranslations[5];
    float blackKeyTranslationsGL[5];


    //WhiteKeys
    int numWhiteKeysVisible = 15;

    float whiteKeyWidth = 50;//px

    float blackKeyWidth = 0.6 * whiteKeyWidth;

    float keyGap = 1.0;//px

    float globalKeyTranslateX = 0;//in pixels

    //num of times(float) white key width the translate  will be calculated from;like when moving keys using positionmodifier
    float keyTranslateXFactor = 20.0;

    float keyPositionModifierOffset = 0.0;//The offset of KeyPositionModifierView from keyPositionModifierViewFull.startX(); calculated from keyTranslateXFactor

    bool bNoteNamesVisible = false;

    std::array<int,PIANO_MAX_KEY_COUNT> keyRenderIndices;

    std::mutex mutKeyStates;




private:
    //Debug Only
    bool bMovePositionModifierRightDebug = false;



    //Below only touch related
private:

    friend class PianoTouchListener;
    friend class PianoSettingsView;

    //x,y in screen coords
    EKeyName getKeyNoAtLoc(float x,float y);


};



//TODO extend this as PianoController as this controls piano
class PianoTouchListener : public View::TouchListener{

protected:

    bool onTouchDown(const float &x, const float &y, const ks::TouchID &id,const bool &isPrimary) override;

    bool onTouchUp(const float &x, const float &y, const ks::TouchID &id, const bool &isLast) override;

    bool onMove(const float &x, const float &y, const ks::TouchID &id) override;

    //TODO dont used this
    bool onTouch(const ks::MotionEvent &event, View *view) override;

    bool onHoverExit(const ks::TouchID &id, const float &x, const float &y) override;

    bool onHoverEnter(const float &d, const float &d1, const ks::TouchID &i) override;

private:

    //store key pointed by touch pointer id ,//TODO this assumes pointerId is always less than 50 need verify;
    EKeyName previousPointedKey[50];//max of 50 finger movements too many
};





#endif //KALASOFT_PIANOVIEW_H
