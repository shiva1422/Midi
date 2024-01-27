//
// Created by shivaaz on 12/2/22.
//

#ifndef KALASOFT_RAINBOWPIANOVIEW_H
#define KALASOFT_RAINBOWPIANOVIEW_H

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
#include <mutex>

#define MIN_WHITEKEY_COUNT 7
#define MAX_WHITEKEY_COUNT 52
#define DEFAULT_WHITEKEY_COUNT 52


   /*
    * Draw keyboard using openGL instanced rendering with rainbow colors,
    * Total Keys 88 ,whitekeys - 52,blackkeys -36
    * A0 - C8
    */
class RainbowPianoView : public View , public IPianoController{



    //TODO Moves Shaders to here as string rather than reading from assets

public:

    /***VIEW***/

    RainbowPianoView();

    RainbowPianoView(float width, float height);

    RainbowPianoView(float startX, float startY, float width, float height);

    //TODO later make Text engine into common without explicit dependency
    RainbowPianoView(float startX, float startY, float width, float height, TextEngine *textEngine);

    ~RainbowPianoView() override;

    void setBounds(float startX, float startY, float width, float height) override;

    void clearBackground() override;

    void draw() override;

   // void prepare();

    void testDraw();

    //factor should be increment in terms of whitekeyWidth.eg 1.5 implies move 1.5 white keys left,-1.5 implies move 1.5 keys right.
    void translateKeyXPosition(float factor);


public:

    /***IPianoController***/

    void setKeyState(const EKeyName name, const EKeyState state) override;

    void setKeysStates(const std::vector<KeyStateInfo> states) override;


private:

    friend class PianoApplication;
    void setShaderProgram(GLuint shader){this->shader = shader;}//TODO Temporary only later bring shaders into strings from assets

    void prepareShaders(AssetManager *assetManager);

    void setBlackKeyTranslations();

    void setupTextures();

    void setTextEngine(TextEngine *textEngine){ this->textEngine = textEngine;}

    void doPianoLayout();

    void prepare(AssetManager *assetManager);

    //only drawKeys and noteNames
    void drawKeysAndNoteNames();

    //draw all other keyboard frame sliders and any thing required decorations;
    void drawNonKeyViews();


private:

    GLuint keysAndNoteNamesShader = 0;

    //locations for above shader
    GLint whiteKeyVertsLoc,blackKeyVertsLoc,noteNameVertsLoc,octaveNumVertsLoc,keysAreaBoundsLoc,blackKeyTransLationXLoc,paramsLoc,isKeyOnLoc,keyGapXLoc;

    GLuint shader = 0;//Right now a Rainbow PianoSynth,Will change later to Textured One Should




private:

    void resizeKeyCount(int whiteKeysVisibleCnt);

    void initKeyRenderIndices();


private:

    //Textures
    KSImage allKeysImage;

    GLuint allKeyTex = 0 ,noteNamesTex = 0 , noteNamesBuf = 0;

    //TODO move outof this
    TextEngine *textEngine = nullptr;

    PixaMap noteNames[19];//noteName include # //TODO KSImage



private:
    //Piano All Measurements

    //blackKey, whiteKey, keySizeDec, topBackground, keyPositionChanger, keyPosChangerFull,noteNameDims,octaveNumDims;//keyPositionChanger is the total width of the position Bar
    //below Views only for dimension not drawing ,but if needed more complicated rendering extend GLView to create those .
    //All View below are tranlated while rendereing their positions are not absolute;

    //To store bounds for Keys rendering area;
    GLView keysAreaView;

    GLView whiteKeyView,blackKeyView;//first white key bounds.

    GLView keySizeIncView,keyCountDecView;

    GLView keyPositionModifierView ,keyPositionModifierViewFull;

    GLView noteNameView;

    GLView octaveNumView;



    //keyOn-1 off - 0
    int keyOn[PIANO_MAX_KEY_COUNT] = {0};//TODO 89?shader

    //BlackKeys are rendered as tranlated from fixed absolute position of starting key;
    float blackKeyTranslations[5];
    float blackKeyTranslationsGL[5];


    float keyCountModifierTranlations[2];
    //WhiteKeys
    int numWhiteKeysVisible = DEFAULT_WHITEKEY_COUNT;

    float whiteKeyWidth = 50;//px

    float blackKeyWidth = 0.6 * whiteKeyWidth;

    float keyGap = 1.0;//px

    float topNonKeyHeightRatio = 0.1;//percentage of total piano height above keyStarty.

    float globalKeyTranslateX = 0;//in pixels

    //num of times(float) white key width the translate  will be calculated from;like when moving keys using positionmodifier
    float keyTranslateXFactor = 0.0;

    float keyPositionModifierOffset = 0.0;//The offset of KeyPositionModifierView from keyPositionModifierViewFull.startX(); calculated from keyTranslateXFactor

    bool bNoteNamesVisible = false;

    std::array<int,PIANO_MAX_KEY_COUNT> keyRenderIndices;

    std::mutex mutKeyStates;




private:
    //Debug Only
    bool bMovePositionModifierRightDebug = false;



    //Below only touch related
private:

    friend class RainbowPianoTouchListener;

    //x,y in screen coords
    EKeyName getKeyNoAtLoc(float x,float y);





};



#endif //KALASOFT_RAINBOWPIANOVIEW_H
