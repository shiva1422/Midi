//
// Created by shivaaz on 8/6/24.
//

#include "PianoView.h"


#include <cstdint>
//TODO independent of disp.h
#include <Display.h>
#include <cassert>
#include <Shader.h>
#include <vector>
#include "RainbowPianoView.h"
#include "Media/KSImage/KSImageLoader.h"
#define TAGLOG "PianoView"


/*
 * To modify any postion of keys or others things on piano check directly in setBounds;
 * //check setupTextures done correctly
 */

static GLushort keyIndices[] = {0, 1, 2, 2, 3, 0};
static float textCoods[]={0,1,1,1,1,0,0,0};


PianoView :: PianoView()
{

}
PianoView::PianoView(float width, float height) : PianoView(0, 0, width, height)
{

}

PianoView::PianoView(float startX, float startY, float width, float height) : View(startX, startY, width, height)
{
    setBounds(startX,startY,width,height);//TODO calling virtual method;
}

PianoView::PianoView(float startX, float startY, float width, float height, TextEngine *textEngine) : PianoView(startX, startY, width, height)
{

    this->textEngine = textEngine;
}

PianoView::~PianoView()
{
    //clear Textures,clear Shader,delete allkeysImage
}

void PianoView::clearBackground()
{
    //DOnt use this method
    glEnable(GL_SCISSOR_TEST);
    glScissor(startX,dispMetrics.screenHeight-startY-height,width,height);//if glScissor enable after this then scissor bounds should bes same as viewport bounds;
    glClearColor(r,g,b,a);
    glClear(GL_COLOR_BUFFER_BIT);
    // glDisable(GL_SCISSOR_TEST);//end of drawing TODO
}

   /**
    * draw all in one call using instance rendering
    * 88 keys(52 white + 36 black) ,52 notenames,52 octave numbers
    */

//TODO VAO and UBO,uniform buffers.
void PianoView::drawKeysAndNoteNames()
{

    glUseProgram(keysAndNoteNamesShader);

    glEnableVertexAttribArray(whiteKeyVertsLoc);
    glVertexAttribPointer(whiteKeyVertsLoc, 2, GL_FLOAT, GL_FALSE, 0, (void *) whiteKeyView.getVertices());

    glEnableVertexAttribArray(blackKeyVertsLoc);
    glVertexAttribPointer(blackKeyVertsLoc, 2, GL_FLOAT, GL_FALSE, 0, (void *) blackKeyView.getVertices());

    glEnableVertexAttribArray(textureCoordsLoc);
    glVertexAttribPointer(textureCoordsLoc, 2, GL_FLOAT, GL_FALSE, 0, (void *) textCoods);


    bNoteNamesVisible = true;;//debug only
    if(bNoteNamesVisible)
    {

        glEnableVertexAttribArray(noteNameVertsLoc);
        glVertexAttribPointer(noteNameVertsLoc,2,GL_FLOAT,GL_FALSE,0,(void *)noteNameView.getVertices());
        //   GLContext::getError("drawKeysAndNoteNames pd");

        glEnableVertexAttribArray(octaveNumVertsLoc);
        glVertexAttribPointer(octaveNumVertsLoc,2,GL_FLOAT,GL_FALSE,0,(void *)octaveNumView.getVertices());

    }

    glUniform1fv(blackKeyTransLationXLoc,5,blackKeyTranslationsGL);

    glUniform4f(keysAreaBoundsLoc,keysAreaView.getStartXGL(),keysAreaView.getStartYGL(),keysAreaView.getWidthGL(),keysAreaView.getHeightGL());//TODO enable

    float params[] = {whiteKeyView.getWidthGL(),(float)(keyGap * 2.0)/dispMetrics.screenWidth,static_cast<float>(((whiteKeyView.getWidth() + keyGap) * 2.0)/(float)dispMetrics.screenWidth)};//TODO keyGap in GL helper coords

    glUniform1fv(paramsLoc,3,params);

    std::unique_lock<std::mutex> stateLock(mutKeyStates);
    glUniform1iv(isKeyOnLoc,89,keyOn);//TODO 88
    stateLock.unlock();//TODO keep copy of keyon instead of lock around glfunction.

    //Textures
    for(int i = 0; i < 4; ++i)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D,keyTextures[i]);
        glUniform1i(keyTexLoc+i,i);

    }
    glActiveTexture(GL_TEXTURE0 + 4);
    glBindTexture(GL_TEXTURE_2D_ARRAY,noteNamesTex);
    glUniform1i(noteNamesTexLoc,4);



    // can improve performance by TODO note names and number can be draw in same time as white key,see frag shader
    int instanceCnt = 88  ;//88 keys
    if(bNoteNamesVisible)//TODO this is not yet done
        instanceCnt += 52 + 52;//notename + octaveNum
    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, keyIndices,instanceCnt);


    GLContext::getError("draw Keys AndNoteNames 2");

    glDisableVertexAttribArray(whiteKeyVertsLoc);
    glDisableVertexAttribArray(blackKeyVertsLoc);
    glDisableVertexAttribArray(textureCoordsLoc);
    glDisableVertexAttribArray(noteNameVertsLoc);
    glDisableVertexAttribArray(octaveNumVertsLoc);

}

//can improve performance by TODO instead of drawing every frame, draw to frame buffer once and then draw only when there is a change
void PianoView::draw() {

    // clearBackground();
    drawKeysAndNoteNames();
}

void PianoView::resizeKeyCount(int whiteKeysVisibleCnt)
{

    this->numWhiteKeysVisible = whiteKeysVisibleCnt;
    translateKeyXPosition(0);//because the translateFactor also depends on keyVisible. to scale thre translate aptly;
    doPianoLayout();
}

void PianoView::setBounds(float startX, float startY, float width, float height)
{
    View::setBounds(startX, startY, width, height);//TODO if height falls out of the screen adjust bounds here.
    doPianoLayout();
}

void PianoView::setBlackKeyTranslations()
{
    //5 blackkeys are rendered as translated from blackKeyView(firstone)
    float whiteKeyWidth = whiteKeyView.getWidth();
    float tempDif   =   (blackKeyView.getWidth() + keyGap)/2.0;
    blackKeyTranslations[0] = (whiteKeyWidth + keyGap)-tempDif;//a#
    blackKeyTranslations[1] = 3*(whiteKeyWidth + keyGap)-tempDif;//c#
    blackKeyTranslations[2] = 4*(whiteKeyWidth + keyGap)-tempDif;//d#
    blackKeyTranslations[3] = 6*(whiteKeyWidth + keyGap)-tempDif;//f#
    blackKeyTranslations[4] = 7*(whiteKeyWidth+ keyGap)-tempDif;//g#

    //Convert to GL
    int i = 0;
    for( auto &t : blackKeyTranslations)
    {
        blackKeyTranslationsGL[i] = (t*2.0)/dispMetrics.screenWidth ;
        i++;
    }
}

//TODO clear textures
void PianoView::setupTextures()
{

    assert(textEngine);

    //TODO clear
    keyImages[0] = KSImageLoader::loadFromAsset("icons/white_key1.png");
    keyImages[1] = KSImageLoader::loadFromAsset("icons/white_key1_ontap.png");
    keyImages[2] = KSImageLoader::loadFromAsset("icons/black_key1.png");
    keyImages[3] = KSImageLoader::loadFromAsset("icons/black_key1_ontap.png");

    //white and black keys, TODO texutre array,cobine textures into one texture,also performance
    glGenTextures(4,keyTextures);
    for(int i = 0; i < 4; ++i)
    {
        if(keyImages[i])
        {
            glBindTexture(GL_TEXTURE_2D,keyTextures[i]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, keyImages[i]->width,keyImages[i]->height);//wiki commonmistakes//use glTexImage for mutable textures.//glpixelstore for way to read(pack)and write(unpack) image using this fun.
            glTexSubImage2D(GL_TEXTURE_2D,0,0,0, keyImages[i]->width,keyImages[i]->height,GL_RGBA,GL_UNSIGNED_BYTE,keyImages[i]->data);
          //  delete keyImages[i];
            //keyImages[i] = nullptr;
        }else
        {
            KSLOGE(TAGLOG,"error loading keyTexture %d",keyTextures[i]);
        }

    }

    //set note names texture as texturearray2d /a to g and 1 to 7 and may be # ,+ and -
    //getPixmap for each name a-g ang 1-7 and #
    int maxHeight = 0,maxWidth = 0;//of the noteNames
    char noteName[2];
    noteName[1]='\0';//to make as string;
    uint64_t totalPixelsForAllNames = 0;
    const int totalnoteNameImages = 17;//TOTATAL IMAGES FOR  NOTE NAMES,0-7, A-G and  # AND


    for(int i=0;i<totalnoteNameImages;i++)
    {
        if(i<7)
        {
            noteName[0] = char(65+i);
        }
        else if(i<16)
        {
            noteName[0] = (i - 7) + '0';
        }
        else if(i==16)
        {
            noteName[0] = '#';
        }

        textEngine->getPixamapFromString(noteName,&this->noteNames[i]);

        if(maxHeight < this->noteNames[i].height)
            maxHeight = this->noteNames[i].height;
        if(maxWidth < this->noteNames[i].width)
            maxWidth = this->noteNames[i].width;
        totalPixelsForAllNames += this->noteNames[i].height*this->noteNames[i].width;
        //KSLOGD(TAGLOG,"the string is %s and obtained the bitmap toalPxel %ld",noteName,totalPixelsForAllNames);

    }

    glGenTextures(1,&noteNamesTex);
    glBindTexture(GL_TEXTURE_2D_ARRAY,noteNamesTex);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexStorage3D(GL_TEXTURE_2D_ARRAY,1,GL_RGBA8,maxWidth,maxHeight,totalnoteNameImages);
    for(int i = 0; i < totalnoteNameImages; i++)
    {
        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0,(maxWidth-noteNames[i].width)/2,(maxHeight-noteNames[i].height)/2,i,noteNames[i].width,noteNames[i].height,1,GL_RGBA,GL_UNSIGNED_BYTE,(void *)noteNames[i].pixels);
        //glTexImage3D(GL_TEXTURE_2D_ARRAY,0,GL_RGBA,noteNames[1].width,noteNames[1].height,i,0,GL_RGBA,GL_UNSIGNED_BYTE,(void *)(noteNames[0].width*noteNames[0].height*4));
    }

    glBindTexture(GL_TEXTURE_2D,0);
    glBindTexture(GL_TEXTURE_2D_ARRAY,0);



    KSLOGE(TAGLOG, "Setup Textures Complete");

}

void PianoView::prepare(AssetManager *assetManager)
{
    //TODO async
    prepareShaders(assetManager);
    setupTextures();
    initKeyRenderIndices();
}

void PianoView::translateKeyXPosition(float factor)
{
    factor *= -1;//Just to align with the direction;
    keyTranslateXFactor += factor;
    float maxFactor = MAX_WHITEKEY_COUNT - numWhiteKeysVisible;
    if(keyTranslateXFactor > maxFactor)
    {
        keyTranslateXFactor = maxFactor;
        KSLOGW(TAGLOG,"Translating key Position already reached max val");
        bMovePositionModifierRightDebug = false;
    }
    else if(keyTranslateXFactor < 0.0)
    {
        keyTranslateXFactor = 0.0;
        bMovePositionModifierRightDebug = true;
        KSLOGW(TAGLOG,"Translating key Position already reached min val");
    }

    doPianoLayout();
}

void PianoView::doPianoLayout()
{

    keysAreaView.setBounds(startX,startY ,width, height);


    whiteKeyWidth = (width-(numWhiteKeysVisible-1)*keyGap)/(float)(numWhiteKeysVisible);
    blackKeyWidth = 0.6*whiteKeyWidth;

    //TODO add methods to setBounds in DP in View

    allKeysView.setBounds(startX + width * 20.0/100, startY, width * 60.0/100, 100);

    //
    keyPositionModifierOffset = keyTranslateXFactor * ((float)allKeysView.getWidth()) / (MAX_WHITEKEY_COUNT);
   // keyPositionModifierView.setBounds(allKeysView.getStartX() + keyPositionModifierOffset, allKeysView.getStartY(), (allKeysView.getWidth() * (float) numWhiteKeysVisible) / MAX_WHITEKEY_COUNT, allKeysView.getHeight());

    //keyCountModifierTranlations[0]= width - keyCountDecView.getWidth();//for translation of keyCountIncView
   // keyCountModifierTranlations[1]=0;


    int keyGapCnt = floor(keyTranslateXFactor);//TODO check required

    globalKeyTranslateX = keyTranslateXFactor * (whiteKeyWidth ) + (((int)keyTranslateXFactor)*keyGap);// + keyGapCnt > 0 ?((keyGapCnt)*keyGap) : 0 ;//TODO was (int)keyTranslateXFactor;

    KSLOGD(TAGLOG,"global %f",globalKeyTranslateX);

    KSLOGD("TAGLOG","whiteKeyBounds  scree width %d height %d globalTranslate %f transFactor %f mulitpl %f",dispMetrics.screenWidth,dispMetrics.screenHeight,globalKeyTranslateX,keyTranslateXFactor,keyTranslateXFactor * whiteKeyWidth);
    whiteKeyView.setBounds(startX - globalKeyTranslateX ,startY ,whiteKeyWidth,height);

    //startX same as whitekey and setBlackkey translation  accordingly
    blackKeyView.setBounds(whiteKeyView.getStartX(),whiteKeyView.getStartY(),blackKeyWidth,whiteKeyView.getHeight() * 0.6);
    setBlackKeyTranslations();

    //TODO DP
    noteNameView.setBounds(whiteKeyView.getStartX() + whiteKeyView.getWidth()/4.0 ,whiteKeyView.getStartY() + 0.8*whiteKeyView.getHeight(),whiteKeyView.getWidth()/4.0,whiteKeyView.getWidth()/4.0);
    octaveNumView.setBounds(noteNameView.getEndX(),noteNameView.getStartY(),noteNameView.getWidth(),noteNameView.getHeight());

}

void PianoView::prepareShaders(AssetManager *assetManager)
{
    //TODO check intermediate shaders are cleared

    IKSStream *vertKeys = assetManager->openAsset("shaders/pianoShaders/keystexturedandnotes.vert");
    IKSStream *fragKeys = assetManager->openAsset("shaders/pianoShaders/keystexturedandnotes.frag");
    assert(vertKeys && fragKeys);

    keysAndNoteNamesShader = Shader::createProgram(vertKeys,fragKeys);
    assert(keysAndNoteNamesShader);

    //cache keys and notes shader layout locations
    whiteKeyVertsLoc  = glGetAttribLocation(keysAndNoteNamesShader, "whiteKeyVerts");
    blackKeyVertsLoc  = glGetAttribLocation(keysAndNoteNamesShader,"blackKeyVerts");
    noteNameVertsLoc  = glGetAttribLocation(keysAndNoteNamesShader,"noteNameVerts");
    octaveNumVertsLoc = glGetAttribLocation(keysAndNoteNamesShader,"octaveNumVerts");
    textureCoordsLoc  = glGetAttribLocation(keysAndNoteNamesShader,"textureCoords");
    keysAreaBoundsLoc = glGetUniformLocation(keysAndNoteNamesShader,"keysAreaBounds");
    keyTexLoc         = glGetUniformLocation(keysAndNoteNamesShader,"keyTextures");
    blackKeyTransLationXLoc = glGetUniformLocation(keysAndNoteNamesShader,"blackKeyTransLationX");
    noteNamesTexLoc = glGetUniformLocation(keysAndNoteNamesShader,"notesTextures");
    paramsLoc = glGetUniformLocation(keysAndNoteNamesShader ,"params");
    isKeyOnLoc = glGetUniformLocation(keysAndNoteNamesShader,"isKeyOn");

    GLContext::getError("get Shader Locations");

    KSLOGD(TAGLOG, "btextuers locations %d %d %d %d %d %d %d %d %d %d %d", whiteKeyVertsLoc,blackKeyVertsLoc,noteNameVertsLoc,octaveNumVertsLoc
    ,textureCoordsLoc,keysAreaBoundsLoc,keyTexLoc,blackKeyTransLationXLoc,paramsLoc,isKeyOnLoc,noteNamesTex);



    //TODO debug

    if(GLContext::getError("RainbowPianoView :: prepareShaders") != GL_NO_ERROR)
    {
        assert(false);
    }

}

EKeyName PianoView::getKeyNoAtLoc(float x, float y)
{

    EKeyType keyType{EKeyType::BASE};
    EKeyNameBase keyBaseName;
    int keyNo;

    /*
     * get keyX relative to origin,as if firstkey is at origin
     * (x,y) are in screenCoords
     */
    float keyX = x + globalKeyTranslateX - startX;

    //gives the whitekey no from 0-51
    keyNo = keyX/(whiteKeyView.getWidth() + keyGap );

    //convert A0 to C1 as octave start
    int realOctaveNum = (keyNo + 5) / 7 ;

    //get key nos 0-6;within octaveNum; a - g
    keyBaseName = static_cast<EKeyNameBase>(keyNo % 7);

    if(y <= blackKeyView.getEndY() && y >= blackKeyView.getStartY())
    {
        //could be a black key

        //get startX of this white key relative to origin;
        float keyNoStartX = keyNo * (whiteKeyView.getWidth() + keyGap);

        //TODO should keyGap be considered?
        if (keyX <= keyNoStartX + blackKeyView.getWidth() / 2)
        {
            //because C and F don't have flats
            if (keyBaseName != EKeyNameBase::C && keyBaseName != EKeyNameBase::F)
            {keyType = EKeyType::FLAT;}
        }

        else if (keyX >= (keyNoStartX + whiteKeyView.getWidth() - blackKeyView.getWidth() / 2))
        {
            //cus B and E dont have sharps
            if (keyBaseName != EKeyNameBase::B && keyBaseName != EKeyNameBase::E)
            {keyType = EKeyType::SHARP;}
        }
    }


    /*
     *     //TODO check why below is happening and remove //becase of screen and window size mismatch we get 88 key
        if(name < 0 )
        name = 0;
        else if(name > 87)
        name = 87;
        //TODO
         */
    EKeyName res =  getKeyName(keyBaseName, realOctaveNum, keyType);

    if(res < 0)
    {
        KSLOGW("RainbowPiano","getKeyAtLocation return invalid keyNo %d",res);
        res = static_cast<EKeyName>(0);
    }
    else if(res > PIANO_MAX_KEY_COUNT -1)
    {
        KSLOGW("RainbowPiano","getKeyAtLocation return invalid keyNo %d",res);
        res = static_cast<EKeyName>(PIANO_MAX_KEY_COUNT - 1);
    }

    return res;
}

/***IPianoController Impl***/


KSFORCEINLINE void PianoView::setKeyState(const EKeyName name, const EKeyState state)
{
    //TODO name shouldn't be invalid or out of bounds handled this in upper layer calling this func
    //TODO thread safety always.
    std::lock_guard<std::mutex> stateLock(mutKeyStates);
    keyOn[keyRenderIndices[name]] = state ;
}


void PianoView::setKeysStates(const std::vector<KeyStateInfo> states)
{
    for(auto &state : states)
    {
        setKeyState(state.keyName,state.keyState);
    }
}

void PianoView::initKeyRenderIndices()
{
    //TODO can compute value although this is fast;
    //for rendering white keys 0-51 ,black keys 52-87 in order
    std::array<int,PIANO_MAX_KEY_COUNT> &i = keyRenderIndices;
    i[EKeyName::A0] = 0;
    i[EKeyName::B0] = 1;

    i[EKeyName::C1] = 2;
    i[EKeyName::D1] = 3;
    i[EKeyName::E1] = 4;
    i[EKeyName::F1] = 5;
    i[EKeyName::G1] = 6;
    i[EKeyName::A1] = 7;
    i[EKeyName::B1] = 8;

    i[EKeyName::C2] = 9;
    i[EKeyName::D2] = 10;
    i[EKeyName::E2] = 11;
    i[EKeyName::F2] = 12;
    i[EKeyName::G2] = 13;
    i[EKeyName::A2] = 14;
    i[EKeyName::B2] = 15;

    i[EKeyName::C3] = 16;
    i[EKeyName::D3] = 17;
    i[EKeyName::E3] = 18;
    i[EKeyName::F3] = 19;
    i[EKeyName::G3] = 20;
    i[EKeyName::A3] = 21;
    i[EKeyName::B3] = 22;

    i[EKeyName::C4] = 23;
    i[EKeyName::D4] = 24;
    i[EKeyName::E4] = 25;
    i[EKeyName::F4] = 26;
    i[EKeyName::G4] = 27;
    i[EKeyName::A4] = 28;
    i[EKeyName::B4] = 29;


    i[EKeyName::C5] = 30;
    i[EKeyName::D5] = 31;
    i[EKeyName::E5] = 32;
    i[EKeyName::F5] = 33;
    i[EKeyName::G5] = 34;
    i[EKeyName::A5] = 35;
    i[EKeyName::B5] = 36;

    i[EKeyName::C6] = 37;
    i[EKeyName::D6] = 38;
    i[EKeyName::E6] = 39;
    i[EKeyName::F6] = 40;
    i[EKeyName::G6] = 41;
    i[EKeyName::A6] = 42;
    i[EKeyName::B6] = 43;

    i[EKeyName::C7] = 44;
    i[EKeyName::D7] = 45;
    i[EKeyName::E7] = 46;
    i[EKeyName::F7] = 47;
    i[EKeyName::G7] = 48;
    i[EKeyName::A7] = 49;
    i[EKeyName::B7] = 50;

    i[EKeyName::C8] = 51;

    //black keys

    i[EKeyName::As0] = 52;

    i[EKeyName::Cs1] = 53;
    i[EKeyName::Ds1] = 54;
    i[EKeyName::Fs1] = 55;
    i[EKeyName::Gs1] = 56;
    i[EKeyName::As1] = 57;

    i[EKeyName::Cs2] = 58;
    i[EKeyName::Ds2] = 59;
    i[EKeyName::Fs2] = 60;
    i[EKeyName::Gs2] = 61;
    i[EKeyName::As2] = 62;

    i[EKeyName::Cs3] = 63;
    i[EKeyName::Ds3] = 64;
    i[EKeyName::Fs3] = 65;
    i[EKeyName::Gs3] = 66;
    i[EKeyName::As3] = 67;

    i[EKeyName::Cs4] = 68;
    i[EKeyName::Ds4] = 69;
    i[EKeyName::Fs4] = 70;
    i[EKeyName::Gs4] = 71;
    i[EKeyName::As4] = 72;

    i[EKeyName::Cs5] = 73;
    i[EKeyName::Ds5] = 74;
    i[EKeyName::Fs5] = 75;
    i[EKeyName::Gs5] = 76;
    i[EKeyName::As5] = 77;

    i[EKeyName::Cs6] = 78;
    i[EKeyName::Ds6] = 79;
    i[EKeyName::Fs6] = 80;
    i[EKeyName::Gs6] = 81;
    i[EKeyName::As6] = 82;

    i[EKeyName::Cs7] = 83;
    i[EKeyName::Ds7] = 84;
    i[EKeyName::Fs7] = 85;
    i[EKeyName::Gs7] = 86;
    i[EKeyName::As7] = 87;

}

int PianoView::getNumWhiteKeysVisible() {
    return numWhiteKeysVisible;
}

void PianoView::incrementNumKeysVisible(int incCount) {

    KSLOGD("PianoSetting","increase keys");
    int count = getNumWhiteKeysVisible() + incCount;
    if(count > MAX_WHITEKEY_COUNT)count=MAX_WHITEKEY_COUNT;
    resizeKeyCount(count);

}

void PianoView::decrementNumKeysVisible(int decCount) {

    KSLOGD("PianoSetting","decrease keys");
    int count = getNumWhiteKeysVisible() - decCount;
    if(count < MIN_WHITEKEY_COUNT)count=MIN_WHITEKEY_COUNT;
    resizeKeyCount(count);
}

void PianoView::moveKeysToRight(float whiteKeyFactor) {

    translateKeyXPosition(whiteKeyFactor);

}

void PianoView::moveKeysToLeft(float whiteKeyFactor) {
    translateKeyXPosition(whiteKeyFactor);
}


//Touch Listener
bool PianoTouchListener::onTouch(const ks::MotionEvent &event, View *view)
{
//TODO not using this now
    int32_t index = event.getPointerIndex();
    int32_t id  = event.getPointerId(index);
    float pointX = event.getX(index);
    float pointY = event.getY(index);
    KSLOGD(TAGLOG,"PianoSynthNode on Touch x %f y %f ",pointX,pointY);
    PianoView *piano = static_cast<PianoView *>(view);


    //TODO maybe need to check touch event history cause when swipes speed across screen some ponts may be missiong
    //therfore some keys may not be played or events may be repeated check;
    assert(piano);
    if(piano->keysAreaView.isPointInside(pointX,pointY))
    {    //Touch action on keys
        KSLOGD(TAGLOG,"touching keysArea");
        EKeyName keyName = piano->getKeyNoAtLoc(pointX,pointY);
        piano->controller->setKeyState(keyName,EKeyState::KEYSTATE_ON);

        return true;
    }
    return false;
}

bool PianoTouchListener::onTouchDown(const float &x, const float &y, const ks::TouchID &id,const bool &isPrimary)
{
    PianoView* piano = reinterpret_cast<PianoView *>(this->view);
    if(piano->keysAreaView.isPointInside(x,y))
    {    //Touch action on keys
        KSLOGD(TAGLOG,"touching keysArea");
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

bool PianoTouchListener::onTouchUp(const float &x, const float &y, const ks::TouchID &id,const bool &isLast)
{
    //TODO if islast all should be up (experiment instead of just setting all to off to see any odd thng happens

    PianoView* piano = reinterpret_cast<PianoView *>(this->view);
    if(piano->keysAreaView.isPointInside(x,y))
    {    //Touch action on keys
        KSLOGD(TAGLOG,"touching keysArea action up");
        EKeyName keyName = piano->getKeyNoAtLoc(x,y);
        piano->getController()->setKeyState(keyName,EKeyState::KEYSTATE_OFF);

        //what if touchdown/up not same key?
        previousPointedKey[id] = EKeyName::INVALID;

        return true;
    }
    else
    {//TODO
        KSLOGE(TAGLOG,"error");
    }

    //if isLast allKeySHould be off
    return false;
}

bool PianoTouchListener::onMove(const float &x, const float &y, const ks::TouchID &id)
{
    PianoView* piano = reinterpret_cast<PianoView *>(this->view);

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
            //finger moving in same key that is already set on.

            return true;
        }
    }
    else
    {
        //TODO,This generally should be triggered from outside to HoverExit;

        //Move out of the keys area but same finger moved that caused keyon before;,HoverExit(equivalent).
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


bool PianoTouchListener::onHoverExit(const ks::TouchID &id, const float &x, const float &y)
{
    //TODO also check aptly inside keys area of sliders
    //Moved out of piano

    PianoView* piano = reinterpret_cast<PianoView *>(this->view);

    if(previousPointedKey[id] != EKeyName::INVALID)
    {
        piano->getController()->setKeyState(previousPointedKey[id],EKeyState::KEYSTATE_OFF);
        previousPointedKey[id] = EKeyName::INVALID;
    }


    KSLOGD(TAGLOG,"OnHoverExit");
    return false;
}

bool PianoTouchListener::onHoverEnter(const float &d, const float &d1, const ks::TouchID &i) {
    KSLOGW(TAGLOG,"OnHoverEnter");
    return TouchListener::onHoverEnter(d, d1, i);
}

