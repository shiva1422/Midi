//
// Created by shivaaz on 12/2/22.
//

#include <cstdint>
//TODO independent of disp.h
#include <Display.h>
#include <cassert>
#include <Shader.h>
#include <vector>
#include "RainbowPianoView.h"
#include "Media/KSImage/KSImageLoader.h"
#define TAGLOG "RainbowPianoView"


/*
 * To modify any postion of keys or others things on piano check directly in setBounds;
 * //check setupTextures done correctly
 */
RainbowPianoView :: RainbowPianoView()
{

}
RainbowPianoView::RainbowPianoView(float width, float height) : RainbowPianoView(0, 0, width, height)
{

}

RainbowPianoView::RainbowPianoView(float startX, float startY, float width, float height) : View(startX, startY, width, height)
{
    setBounds(startX,startY,width,height);
}

RainbowPianoView::RainbowPianoView(float startX, float startY, float width, float height, TextEngine *textEngine) : RainbowPianoView(startX, startY, width, height)
{

    this->textEngine = textEngine;
}

RainbowPianoView::~RainbowPianoView()
{
    //clear Textures,clear Shader,delete allkeysImage
}

void RainbowPianoView::clearBackground()
{
    //DOnt use this method
    glEnable(GL_SCISSOR_TEST);
    glScissor(startX,dispMetrics.screenHeight-startY-height,width,height);//if glScissor enable after this then scissor bounds should bes same as viewport bounds;
    glClearColor(r,g,b,a);
    glClear(GL_COLOR_BUFFER_BIT);
   // glDisable(GL_SCISSOR_TEST);//end of drawing TODO
}




void RainbowPianoView::testDraw()
{
   keyPositionModifierViewFull.setBackgroundColor(1.0,0.2,1.0,1.0);
   keyPositionModifierView.setBackgroundColor(0.0,0.4,1.0,1.0);
   keyCountDecView.setBackgroundColor(0.4, 1.0, 0.5, 1.0);
   noteNameView.setBackgroundColor(1.0,0.0,0.0,1.0);
   octaveNumView.setBackgroundColor(0.0,1.0,1.0,1.0);
   whiteKeyView.setBackgroundColor(1.0,1.0,1.0,1.0);
   blackKeyView.setBackgroundColor(0.0,0.0,0.0,1.0);
   keyPositionModifierViewFull.clearBackground();
   keyPositionModifierView.clearBackground();
   keyCountDecView.clearBackground();
   whiteKeyView.clearBackground();
   blackKeyView.clearBackground();
   noteNameView.clearBackground();
   octaveNumView.clearBackground();
}


void RainbowPianoView::drawNonKeyViews()
{
    assert(false);
}

static  GLushort keyIndices[] = {0, 1, 2, 2, 3, 0};


void RainbowPianoView::drawKeysAndNoteNames()
{
    /*
     * draw all in one call using instance rendering
     * 88 keys(52 white + 36 black) ,52 notenames,52 octave numbers
     */

//    if(bMovePositionModifierRightDebug)
//     translateKeyXPosition(0.05);
//    else
//      translateKeyXPosition(-0.05);
//TODO use Vertex and Uniform buffers and VertexArrays

    glEnable(GL_SCISSOR_TEST);
    glScissor(keysAreaView.getStartX(),dispMetrics.screenHeight - keysAreaView.getStartY() - keysAreaView.getHeight(),keysAreaView.getWidth(),keysAreaView.getHeight());

    //GLContext::getError("drawKeysAndNoteNames0");
    glUseProgram(keysAndNoteNamesShader);
   // GLContext::getError("drawKeysAndNoteNames 1");

    glEnableVertexAttribArray(whiteKeyVertsLoc);
    glVertexAttribPointer(whiteKeyVertsLoc, 2, GL_FLOAT, GL_FALSE, 0, (void *) whiteKeyView.getVertices());

    glEnableVertexAttribArray(blackKeyVertsLoc);
    glVertexAttribPointer(blackKeyVertsLoc, 2, GL_FLOAT, GL_FALSE, 0, (void *) blackKeyView.getVertices());

   // GLContext::getError("drawKeysAndNoteNames 3");


    if(bNoteNamesVisible || true)
    {

        glEnableVertexAttribArray(noteNameVertsLoc);
        glVertexAttribPointer(noteNameVertsLoc,2,GL_FLOAT,GL_FALSE,0,(void *)noteNameView.getVertices());
     //   GLContext::getError("drawKeysAndNoteNames pd");

        glEnableVertexAttribArray(octaveNumVertsLoc);
        glVertexAttribPointer(octaveNumVertsLoc,2,GL_FLOAT,GL_FALSE,0,(void *)octaveNumView.getVertices());

        //GLContext::getError("drawKeysAndNoteNames unigdfd");

    }



    glUniform1fv(blackKeyTransLationXLoc,5,blackKeyTranslationsGL);
   // GLContext::getError("drawKeysAndNoteNames unigd3");

    glUniform4f(keysAreaBoundsLoc,keysAreaView.getStartXGL(),keysAreaView.getStartYGL(),keysAreaView.getWidthGL(),keysAreaView.getHeightGL());//TODO enable
   //KSLOGD(TAGLOG,"keyAreaStartX %f,keyArea StartY %f ,keyAreaWidht %f,keyAreaHeight %f",keysAreaView.getStartXGL(),keysAreaView.getStartYGL(),keysAreaView.getWidthGL(),keysAreaView.getHeightGL());

    float params[] = {whiteKeyView.getWidthGL(),(float)(keyGap * 2.0)/dispMetrics.screenWidth,static_cast<float>(((whiteKeyView.getWidth() + keyGap) * 2.0)/(float)dispMetrics.screenWidth)};//TODO keyGap in GL helper coords

    glUniform1fv(paramsLoc,3,params);
    //GLContext::getError("drawKeysAndNoteNames unigd");

    std::unique_lock<std::mutex> stateLock(mutKeyStates);
    glUniform1iv(isKeyOnLoc,89,keyOn);//TODO 88
    stateLock.unlock();//TODO keep copy of keyon instead of lock around glfunction.






    int instanceCnt = 88 ;//88 keys
    if(bNoteNamesVisible)//TODO this is not yet done
        instanceCnt += 52 + 52;//notename + octaveNum
    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, keyIndices,instanceCnt);

   // GLContext::getError("drawKeysAndNoteNames 2");



    glDisable(GL_SCISSOR_TEST);//TODO do this after around correct bound in draw

   // KSLOGD(TAGLOG,"drawingKeys and NoteNames");
   // assert(false);
}


void RainbowPianoView::draw() {


    drawKeysAndNoteNames();
   /// drawNonKeyViews();

    //return;
//    //All the piano is drawn in single draw call using instanced Rendering .
//    if(bMovePositionModifierRightDebug)
//    translateKeyXPosition(0.05);
//    else
//        translateKeyXPosition(-0.05);


    //TODO no nedx
    assert(shader != 0);
    setBackgroundColor(1.0,0.0,0.0,1.0);
   // clearBackground();
    int drawCost = 100;//ms
   // KSLOGD(TAGLOG,"DrawingPiano");

     //Drawing
     drawKeysAndNoteNames();
     return;
    //  testDraw();
    // whiteKey.printVertices();
    // setBlackKeyTranslations();///////erron in ifelse (keydown/move/all && keydow[on]=1 in onkeyaction for key or else not needed this here

    glUseProgram(shader);
    //TODO
    float displayParameters[]={(float) dispMetrics.screenWidth,(float)dispMetrics.screenHeight,static_cast<float>(whiteKeyView.getWidth()),static_cast<float>(keyGap),1};//1 padheight remove

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0,(void *) whiteKeyView.getVertices());

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void *) blackKeyView.getVertices());

    glUniform1fv(5,5,displayParameters);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void *) keyPositionModifierViewFull.getVertices());//TODO

    // setBlackKeyTranslations();
    glUniform1fv(10,5,blackKeyTranslations);
    glUniform1i(120,0);//drawtype

    //Key Position and size modifiers
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 0, (void *) keyCountDecView.getVertices());
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE, 0, (void *) keyPositionModifierView.getVertices());
    glUniform1fv(122,2,keyCountModifierTranlations);

    /////keyPOSIOTN AND SIZE MODIFIERS
    float textCood[]={0,1,1,1,1,0,0,0};/////////redo
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6,2,GL_FLOAT,GL_FALSE,0,(void *)textCood);
    glEnableVertexAttribArray(7);
    glVertexAttribPointer(7,2,GL_FLOAT,GL_FALSE,0,keyPositionModifierViewFull.getVertices());
    glEnableVertexAttribArray(8);
    glVertexAttribPointer(8,2,GL_FLOAT,GL_FALSE,0,(void *)noteNameView.getVertices());
    glEnableVertexAttribArray(9);
    glVertexAttribPointer(9,2,GL_FLOAT,GL_FALSE,0,(void *)octaveNumView.getVertices());
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,allKeyTex);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D_ARRAY,noteNamesTex);

    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, keyIndices,196);//keys background  and keySize and position changers' and allkeys bar-92  + 52 note names + 52 octaveNum
    // glDrawArraysInstanced(GL_TRIANGLE_FAN,0,4,145);//no indices required

    glUniform1i(120,1);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_SHORT,keyIndices);//drawing positionchanger
    glDisable(GL_BLEND);

    //Drawing End
    glDisable(GL_SCISSOR_TEST);

    KSLOGD(TAGLOG,"piano Draw Cost %d ms",drawCost);
}

void RainbowPianoView::resizeKeyCount(int whiteKeysVisibleCnt)
{

    this->numWhiteKeysVisible = whiteKeysVisibleCnt;
    float maxWidth = width;
    float maxHeight = height;

    whiteKeyWidth = (maxWidth-(numWhiteKeysVisible - 1)*keyGap)/(float)(numWhiteKeysVisible);
    blackKeyWidth = 0.6*whiteKeyWidth;

}

void RainbowPianoView::setBounds(float startX, float startY, float width, float height)
{
    View::setBounds(startX, startY, width, height);
    doPianoLayout();
}

void RainbowPianoView::setBlackKeyTranslations()
{
    //5 blackkeys are rendered as translated from blackKeyView
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

void RainbowPianoView::setupTextures()
{

    assert(textEngine);
    glUseProgram(shader);

    allKeysImage = KSImageLoader::loadFromAsset("icons/allkeys.png");
     // textEngine->getPixamapFromString("C",&allKeysTexture);
    //getPhoto(app,&allKeysTexture);///////////make pixmap as a class and make params private
    /////setallkeysTexture
    //glGenBuffers(1,&texBufId);
    //glBindBuffer(GL_PIXEL_UNPACK_BUFFER,texBufId);
    //glBufferData(GL_PIXEL_UNPACK_BUFFER,allKeysTexture.height*allKeysTexture.width*4,(void *)allKeysTexture.pixels,GL_STATIC_COPY);

    //Texture for drawing KeyPositionModifierViewFull
    glGenTextures(1,&allKeyTex);
    glUniform1i(125,0);//setting sample unit to 0texture in fragmentshader tex
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,allKeyTex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexStorage2D(GL_TEXTURE_2D,1,GL_RGBA8,allKeysImage->width,allKeysImage->height);
    glTexSubImage2D(GL_TEXTURE_2D,0,0,0,allKeysImage->width,allKeysImage->height,GL_RGBA,GL_UNSIGNED_BYTE,allKeysImage->data);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER,0);
    glBindTexture(GL_TEXTURE_2D,0);

    //set note names texture as texturearray2d /a to g and 1 to 7 and may be # ,+ and -
    //getPixmap for each name a-g ang 1-7 and #
    int maxHeight = 0,maxWidth = 0;//of the noteNames
    char noteName[2];
    noteName[1]='\0';//to make as string;
    uint64_t totalPixelsForAllNames = 0;
    const int totalnoteNameImages = 19;//TOTATAL IMAGES FOR  NOTE NAMES A-G and  # AND + AND -


    for(int i=0;i<totalnoteNameImages;i++)
    {
        if(i<7)
        {
            noteName[0]=char(65+i);
        }
        else if(i<16)
        {
            noteName[0] = (i - 7) + '0';
        }
        else if(i==16)
        {
            noteName[0] = '#';
        }
        else if(i==17)
        {
            noteName[0] = '_';
        }
        else if(i==18)
        {
            noteName[0] = '+';
        }
        textEngine->getPixamapFromString(noteName,&this->noteNames[i]);

        if(maxHeight < this->noteNames[i].height)
            maxHeight = this->noteNames[i].height;
        if(maxWidth < this->noteNames[i].width)
            maxWidth = this->noteNames[i].width;
        totalPixelsForAllNames += this->noteNames[i].height*this->noteNames[i].width;
        KSLOGD(TAGLOG,"the string is %s and obtained the bitmap",noteName);

    }

    /* glActiveTexture(GL_TEXTURE0);
     glBindTexture(GL_TEXTURE_2D,texId);
     glTexSubImage2D(GL_TEXTURE_2D,0,0,0,noteNames[1].width,noteNames[1].height,GL_RGBA,GL_UNSIGNED_BYTE,noteNames[1].pixels);
     glBindBuffer(GL_PIXEL_UNPACK_BUFFER,0);
     glBindTexture(GL_TEXTURE_2D,0);*/



    //create buffer for noteNames and upload data
    glGenBuffers(1,&noteNamesBuf);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER,noteNamesBuf);
    glBufferData(GL_PIXEL_UNPACK_BUFFER,totalPixelsForAllNames*sizeof(int32_t),(void *)0,GL_STATIC_DRAW);
    int32_t *data=(int32_t *)glMapBufferRange(GL_PIXEL_UNPACK_BUFFER,0,totalPixelsForAllNames*sizeof(int32_t),GL_MAP_WRITE_BIT);
    int32_t offset=0;
    int32_t totalPixels=0;
    if(data)
    {
        for(int i=0;i<totalnoteNameImages;i++)
        {
            totalPixels=noteNames[i].height*noteNames[i].width;
            memcpy(data+offset,noteNames[i].pixels,totalPixels*sizeof(int32_t));
            offset+=totalPixels;
        }
        glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);

    }
    else
        KSLOGE(TAGLOG,"error mapping buffer for noteNames");
    /*/////createTextureArrayanduploadData from above pixelUnpack Buffer*/
    //   glBindBuffer(GL_PIXEL_UNPACK_BUFFER,0);

    glGenTextures(1,&noteNamesTex);
    glUniform1i(126,1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D_ARRAY,noteNamesTex);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexStorage3D(GL_TEXTURE_2D_ARRAY,1,GL_RGBA8,maxWidth,maxHeight,totalnoteNameImages);
    offset=0;
    for(int i=0;i<totalnoteNameImages;i++)
    {
        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0,(maxWidth-noteNames[i].width)/2,(maxHeight-noteNames[i].height)/2,i,noteNames[i].width,noteNames[i].height,1,GL_RGBA,GL_UNSIGNED_BYTE,(void *)offset);
        offset+=noteNames[i].width*noteNames[i].height*4;
        //glTexImage3D(GL_TEXTURE_2D_ARRAY,0,GL_RGBA,noteNames[1].width,noteNames[1].height,i,0,GL_RGBA,GL_UNSIGNED_BYTE,(void *)(noteNames[0].width*noteNames[0].height*4));
    }

    glBindBuffer(GL_PIXEL_UNPACK_BUFFER,0);
    glBindTexture(GL_TEXTURE_2D_ARRAY,0);


}

void RainbowPianoView::prepare(AssetManager *assetManager)
{
    prepareShaders(assetManager);
    assert(shader);
    setupTextures();
    initKeyRenderIndices();
}

void RainbowPianoView::translateKeyXPosition(float factor)
{
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

void RainbowPianoView::doPianoLayout()
{
    //All measurements inside piano here;
    //TODO check DP?


    int topNonKeyHeight = topNonKeyHeightRatio * height;
    //To make the keyCountDecView square convert to dp and the px;
    int sizeModifierHieghtDP = dispMetrics.pixelToDP(topNonKeyHeight,EScreenDirection::Y);
    int sizeModifierWidthInPX = dispMetrics.DPToPixel(sizeModifierHieghtDP,EScreenDirection::X);

    keysAreaView.setBounds(startX,startY + topNonKeyHeight,width,(1.0 - topNonKeyHeightRatio) * height);


    whiteKeyWidth = (width-(numWhiteKeysVisible-1)*keyGap)/(float)(numWhiteKeysVisible);
    blackKeyWidth = 0.6*whiteKeyWidth;

    //TODO add methods to setBounds in DP in View

    keyCountDecView.setBounds(startX, startY, sizeModifierWidthInPX, topNonKeyHeight);
    keyPositionModifierViewFull.setBounds(keyCountDecView.getEndX(), startY, width - 2.0 * keyCountDecView.getWidth(), keyCountDecView.getHeight());
    //
    keyPositionModifierOffset = keyTranslateXFactor * ((float)keyPositionModifierViewFull.getWidth())/(MAX_WHITEKEY_COUNT);
    keyPositionModifierView.setBounds(keyPositionModifierViewFull.getStartX() + keyPositionModifierOffset, keyPositionModifierViewFull.getStartY(), (keyPositionModifierViewFull.getWidth() * (float) numWhiteKeysVisible) / MAX_WHITEKEY_COUNT, keyPositionModifierViewFull.getHeight());

    keyCountModifierTranlations[0]= width - keyCountDecView.getWidth();//for translation of keyCountIncView
    keyCountModifierTranlations[1]=0;


    int keyGapCnt = floor(keyTranslateXFactor);//TODO check required

    globalKeyTranslateX = keyTranslateXFactor * (whiteKeyWidth ) + (((int)keyTranslateXFactor)*keyGap);// + keyGapCnt > 0 ?((keyGapCnt)*keyGap) : 0 ;//TODO was (int)keyTranslateXFactor;

    KSLOGD(TAGLOG,"global %f",globalKeyTranslateX);

    KSLOGD("TAGLOG","whiteKeyBounds  scree width %d height %d globalTranslate %f transFactor %f mulitpl %f",dispMetrics.screenWidth,dispMetrics.screenHeight,globalKeyTranslateX,keyTranslateXFactor,keyTranslateXFactor * whiteKeyWidth);
    whiteKeyView.setBounds(startX - globalKeyTranslateX ,startY + topNonKeyHeight ,whiteKeyWidth,height - topNonKeyHeight);

    //startX same as whitekey and setBlackkey translation  accordingly
    blackKeyView.setBounds(whiteKeyView.getStartX(),whiteKeyView.getStartY(),blackKeyWidth,whiteKeyView.getHeight() * 0.6);
    setBlackKeyTranslations();

    //TODO DP
    noteNameView.setBounds(whiteKeyView.getStartX(),whiteKeyView.getStartY() + 0.7*whiteKeyView.getHeight(),whiteKeyView.getWidth()/2.0,whiteKeyView.getWidth()/1.8);
    octaveNumView.setBounds(noteNameView.getEndX(),noteNameView.getStartY(),noteNameView.getWidth(),noteNameView.getHeight());
}

void RainbowPianoView::prepareShaders(AssetManager *assetManager)
{
    //TODO check intermediate shaders are cleared
    IKSStream *vertA = assetManager->openAsset("shaders/pianoShaders/vertexShader.glsl");
    IKSStream *fragA = assetManager->openAsset("shaders/pianoShaders/fragmentShader.glsl");
    assert(vertA && fragA);
    shader = Shader::createProgram(vertA,fragA);

    IKSStream *vertKeys = assetManager->openAsset("shaders/pianoShaders/keysandnotes.vert");
    IKSStream *fragKeys = assetManager->openAsset("shaders/pianoShaders/keysandnotes.frag");
    assert(vertKeys && fragKeys);

    keysAndNoteNamesShader = Shader::createProgram(vertKeys,fragKeys);
    assert(keysAndNoteNamesShader);

    //cache keysandnotes shader layout locations
    whiteKeyVertsLoc  = glGetAttribLocation(keysAndNoteNamesShader, "whiteKeyVerts");
    blackKeyVertsLoc  = glGetAttribLocation(keysAndNoteNamesShader,"blackKeyVerts");
    noteNameVertsLoc  = glGetAttribLocation(keysAndNoteNamesShader,"noteNameVerts");
    octaveNumVertsLoc = glGetAttribLocation(keysAndNoteNamesShader,"octaveNumVerts");
    keysAreaBoundsLoc = glGetUniformLocation(keysAndNoteNamesShader,"keysAreaBounds");
    blackKeyTransLationXLoc = glGetUniformLocation(keysAndNoteNamesShader,"blackKeyTransLationX");
    paramsLoc = glGetUniformLocation(keysAndNoteNamesShader ,"params");
    isKeyOnLoc = glGetUniformLocation(keysAndNoteNamesShader,"isKeyOn");


    //TODO debug

    if(GLContext::getError("RainbowPianoView :: prepareShaders") != GL_NO_ERROR)
    {
        assert(false);
    }

}

EKeyName RainbowPianoView::getKeyNoAtLoc(float x, float y)
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


KSFORCEINLINE void RainbowPianoView::setKeyState(const EKeyName name, const EKeyState state)
{
    //TODO name shouldn't be invalid or out of bounds handled this in upper layer calling this func
    //TODO thread safety always.
    std::lock_guard<std::mutex> stateLock(mutKeyStates);
    keyOn[keyRenderIndices[name]] = state ;
}


void RainbowPianoView::setKeysStates(const std::vector<KeyStateInfo> states)
{
    for(auto &state : states)
    {
        setKeyState(state.keyName,state.keyState);
    }
}

void RainbowPianoView::initKeyRenderIndices()
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



