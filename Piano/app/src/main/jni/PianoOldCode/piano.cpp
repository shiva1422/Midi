#include <sys/types.h>
#include <GLES3/gl31.h>
#include <malloc.h>
#include "piano.h"
#include "android/log.h"
#include "ui.h"
#include "commons.h"
#include "inputoutput.h"
#include "unistd.h"
#include "filemanager.h"
#include "text.h"
#include "javacalls.h"

//
// Created by pspr1 on 8/2/2020.
//
extern DisplayParams displayParams;
void PianoSynthNode::pianoInitialSetup(android_app *papp)
{       glUseProgram(programId);
        app=papp;
        setDimensions(noOfKeysVisible);
        noTimesOfKeyTranslateX=20;
       positionChangerMovedDistance=noTimesOfKeyTranslateX*keyPosChangerFull.width/(TOTALWHITEKEYS);///just to set INITials tranlatoilns
       keyGlobalTranslateX=noTimesOfKeyTranslateX*(whiteKey.width)+(int(noTimesOfKeyTranslateX)-1)*keyGap;
       setDimensions(noOfKeysVisible);
        //noteName.setDimensions(whiteKey.startX,whiteKey.startY+whiteKey.height*3/4,whiteKey.width,whiteKey.height*1/4);
        setupTextures();
        for(int i=0;i<50;i++)
        {
            previousPointedKey[i]=52;
        }
        for(int i=0;i<88;i++)
        {
            keyOn[i] = 0;//all keys of initially
           // totLen[i]=0;
            curPos[i]=0;

        }
        /////////Audio
        ////////////////////////setallmidimsgs to 0 if needed
      //getAudioFormatFromFile(app,&audioFormat);
      setUpAudio();

}
void PianoSynthNode::setUpAudio()
{
    loadSounds(app,audio,totLen);
    audioEngine.setUp();
}
void PianoSynthNode::updateMsgsOnAction(int noOfMsgs, int pointerId)
{
    if(noOfMsgs==1)
    {//action up or down
        midiMsgs[writeOffset][0] = pianoKeyNo[keyDown] -1;///////////edit in the pianoKeyNo everthing by-1;////////////check if wrinting and reading msgon and msgoff seperately helps latency
        midiMsgs[writeOffset][1] = keyOn[keyDown];
        writeOffset++;
        if (writeOffset > 49)//49 is the size of midimsgs-1;
            writeOffset = 0;
        if(pthread_mutex_lock(&msgLock)==0)
        {
            msgCount++;
            pthread_mutex_unlock(&msgLock);
           // PianoLog("updated keys on onaction");
        }


    }
    else if (noOfMsgs==2)
    {//for action move swipe from one key to other one is off and other is on
        midiMsgs[writeOffset][0]= pianoKeyNo[previousPointedKey[pointerId]]-1;//////////////edit in the pianoKeyNo everthing by-1;
        midiMsgs[writeOffset][1]=0;
        writeOffset++;
        if(writeOffset>49)
            writeOffset=0;
        midiMsgs[writeOffset][0] = pianoKeyNo[keyDown] -1;///////////edit in the pianoKeyNo everthing by-1;////////////check if wrinting and reading msgon and msgoff seperately helps latency
        midiMsgs[writeOffset][1] = 1;
        writeOffset++;
        if (writeOffset > 49)
            writeOffset = 0;
        if(pthread_mutex_lock(&msgLock)==0)
        {
            msgCount+=2;
            pthread_mutex_unlock(&msgLock);

        }

    }
}
int32_t PianoSynthNode::onAction(int pointerId, float touchX, float touchY, PianoActionType actionType)
{

    touchX+=keyGlobalTranslateX;///////////////////////////////////////////
    keyDown=touchX/(whiteKey.width+keyGap);//gives the whitekey no from 0-51
    octaveNum=keyDown/7;
    octaveKeyNo=keyDown%7;//gives key nos 0-6;(single octave);
    octaveGap=octaveNum*7;
    // IO("the octave num is %d",pointerIndex);
   // PianoLog("the on action test ");
    if(touchY<=blackKey.endY())//if blackKey
    {
        switch(octaveKeyNo) {
            case 0:
            {//ignore first key g#0 a# is 53
                if (touchX <= ((octaveGap)*(whiteKey.width + keyGap)+ whiteKey.width / 4))
                {
                   //PianoLog ("the touch is g#%d",octaveNum);
                    keyDown=TOTALWHITEKEYS+5*octaveNum;
                    if(keyDown==52)
                        keyDown=0;
                }
                else if (touchX >= ((octaveGap+1)*(whiteKey.width + keyGap) -whiteKey.width/ 4))
                {
                   // PianoLog("the touch is a#%d",octaveNum);
                    keyDown=TOTALWHITEKEYS+1+5*octaveNum;

                }//else{// PianoLog("the key touched is a, %f, %d", touchX, keyDown);}
            }
                break;
            case 1:
            {
                if (touchX <= ((octaveGap+1)*(whiteKey.width + keyGap) + whiteKey.width / 4))
                {
                   // PianoLog("the touch is a#%d",octaveNum);
                    keyDown=TOTALWHITEKEYS+1+5*octaveNum;
                }//else{//  PianoLog("the touch is b");}

            }
                break;
            case 2:
            {
                if (touchX >= ((octaveGap+3)*(whiteKey.width + keyGap)- whiteKey.width / 4))
                {
                   // PianoLog("the touch is c#%d",octaveNum);
                    keyDown=TOTALWHITEKEYS+2+5*octaveNum;
                    if(keyDown>88)//cuz after last c not blackkey
                        keyDown=51;
                }//else{// PianoLog("the key touched is a, %f, %d", touchX, keyDown);}

            }
                break;
            case 3:
            {
                if (touchX <= ((octaveGap+3)*(whiteKey.width + keyGap) +whiteKey.width / 4))
                {
                    //PianoLog("the touch is c#%d",octaveNum);
                    keyDown=TOTALWHITEKEYS+2+5*octaveNum;
                    if(keyDown>88)
                        keyDown=51;
                }
                else if (touchX >= ((octaveGap+4)*(whiteKey.width + keyGap)- whiteKey.width / 4))
                {
                   // PianoLog("the touch is d#%d",octaveNum);
                    keyDown=TOTALWHITEKEYS+3+5*octaveNum;
                }//else{//PianoLog("the key touched is a, %f, %d", touchX, keyDown);}
            }
                break;
            case 4:
            {
                if (touchX <= ((octaveGap+4)*(whiteKey.width + keyGap) +whiteKey.width / 4))
                {
                   // PianoLog("the touch is d#%d",octaveNum);
                    keyDown=TOTALWHITEKEYS+3+5*octaveNum;
                }//else{// PianoLog("the key touched is a, %f, %d", touchX, keyDown);}

            }
                break;
            case 5:
            {
                if (touchX >= ((octaveGap+6)*(whiteKey.width + keyGap)- whiteKey.width / 4))
                {
                   // PianoLog("the touch is f#%d",octaveNum);
                    keyDown=TOTALWHITEKEYS+4+5*octaveNum;
                }//else{// PianoLog("the key touched is a, %f, %d", touchX, keyDown);}
            }
                break;
            case 6:
            {
                if (touchX <= ((octaveGap+6)*(whiteKey.width + keyGap) +whiteKey.width/ 4))
                {
                    //PianoLog("the touch is f#%d",octaveNum);
                    keyDown=TOTALWHITEKEYS+4+5*octaveNum;
                }
                else if (touchX >= ((octaveGap+7)*(whiteKey.width + keyGap) - whiteKey.width / 4))
                {
                    //PianoLog("the touch is g#%d",octaveNum);
                    keyDown=TOTALWHITEKEYS+5+5*octaveNum;
                }//else{// PianoLog("the key touched is a, %f, %d", touchX, keyDown);}
            }
                break;

        }
    }
    if(actionType==ACTION_MOVE)
    {
        if (previousPointedKey[pointerId] != keyDown)//////////write in order for faster
        {
            updateMsgsOnAction(2,pointerId);//////////write function inline if can
            keyOn[previousPointedKey[pointerId]] = 0;
            keyOn[keyDown]=1;
            glUniform1i(20 + previousPointedKey[pointerId],0);//20 is  location of the array[shader] .***********************//off
            glUniform1i(20 + keyDown,keyOn[keyDown]);//20 is  location of the array[shader] .***********************//////NEED NOT UPDATE WHIE MOVE IF SAME KEY
            previousPointedKey[pointerId] = keyDown;
        }

    }
    else if (actionType == ACTION_DOWN&&keyOn[keyDown]!=1)
    {
        keyOn[keyDown] = 1;
        updateMsgsOnAction(1,pointerId);
        previousPointedKey[pointerId] = keyDown;
        glUniform1i(20 + keyDown,keyOn[keyDown]);//20 is  location of the array[shader] .***********************//////NEED NOT UPDATE WHIE MOVE IF SAME KEY
    }
    else if (actionType == ACTION_UP)
    {
        keyOn[keyDown] = 0;//first 52 white keys next 37 black next 8 pads.//FIST G#(BLACK KEY) IGNORE
        updateMsgsOnAction(1,pointerId);
        previousPointedKey[pointerId] = 52;//cuz 52 is ignored////////////////////////////
        glUniform1i(20 + keyDown,keyOn[keyDown]);//20 is  location of the array[shader] .***********************//////NEED NOT UPDATE WHIE MOVE IF SAME KEY
    }
    else if(actionType == ACTION_MOVEOUT)
    {
        int tempkeyDown=keyDown;
        keyDown = previousPointedKey[pointerId];///////////edit in the pianoKeyNo everthing by-1;//keyDown here is assigned directly although it wont affect other code needs check if keyDown is used after this
        keyOn[keyDown]=0;
        updateMsgsOnAction(1,pointerId);
        glUniform1i(20 + previousPointedKey[pointerId],0);//20 is  location of the array[shader]
        previousPointedKey[pointerId] = 52;//////////if the same finger when moved back in if dont need sound back just include this !=52 in actionmove
        keyDown=tempkeyDown;//just so that the keyDown assignment just above doesnt effect after this.
    }
    return 1;
}

void PianoSynthNode::setupTextures()
{
    // textEngine->getPixamapFromString("C",&allKeysTexture);
    getPhoto(app,&allKeysTexture);///////////make pixmap as a class and make params private
    //setallkeysTexture
    glGenBuffers(1,&texBufId);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER,texBufId);
    glBufferData(GL_PIXEL_UNPACK_BUFFER,allKeysTexture.height*allKeysTexture.width*4,(void *)allKeysTexture.pixels,GL_STATIC_COPY);
    glGenTextures(1,&texId);
    glUniform1i(125,0);//setting sample unit to 0texture in fragmentshader tex
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,texId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexStorage2D(GL_TEXTURE_2D,1,GL_RGBA8,allKeysTexture.width,allKeysTexture.height);
    glTexSubImage2D(GL_TEXTURE_2D,0,0,0,allKeysTexture.width,allKeysTexture.height,GL_RGBA,GL_UNSIGNED_BYTE,0);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER,0);
    glBindTexture(GL_TEXTURE_2D,0);
    ///set note names texture as texturearray2d /a to g and 1 to 7 and may be # ,+ and -
      ///getPixmap for each name a-g ang 1-7 and #
    int maxHeight=0,maxWidth=0;//of the noteNames
    char noteName[2];
    noteName[1]='\0';//to make as string;
    uint totalPixelsForAllNames=0;
    const int totalnoteNameImages=19;///////////////tOTAL IMAGES FROM NOTE NAMES  # AND + AND -

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
        if(maxHeight<this->noteNames[i].height)
            maxHeight=this->noteNames[i].height;
        if(maxWidth<this->noteNames[i].width)
            maxWidth=this->noteNames[i].width;
        totalPixelsForAllNames+=this->noteNames[i].height*this->noteNames[i].width;
        PianoLog("the string is %s and obtained the bitmap",noteName);

    }
   /* glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,texId);
    glTexSubImage2D(GL_TEXTURE_2D,0,0,0,noteNames[1].width,noteNames[1].height,GL_RGBA,GL_UNSIGNED_BYTE,noteNames[1].pixels);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER,0);
    glBindTexture(GL_TEXTURE_2D,0);*/


    //create buffer for noteNames and upload data
    glGenBuffers(1,&noteNamesTexBufId);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER,noteNamesTexBufId);
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
        PianoLog("error mapping buffer for noteNames");
      /*/////createTextureArrayanduploadData from above pixelUnpack Buffer*/
   //   glBindBuffer(GL_PIXEL_UNPACK_BUFFER,0);

    //create 3D texture for notenames
    glGenTextures(1,&noteNamesTexId);
    glUniform1i(126,1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D_ARRAY,noteNamesTexId);
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
void PianoSynthNode::setDimensions(int noKeysVisible)
{
    float screenWidth=displayParams.screenWidth;
    float screenHeight=displayParams.screenHeight;
    noOfKeysVisible = noKeysVisible;
    float whiteKeyWidth =(screenWidth-(noOfKeysVisible-1)*keyGap)/(noOfKeysVisible);
    float blackKeyWidth=0.6*whiteKeyWidth;
    ///chaning the dimensions of keySizeDec is enough to change dimension of whole piano //total piano dimensions start at this
    ///the width of keyPositionchanger is calculated from total scoll bar width , noOfKeysVIsible and totalWhiteKeys
    keySizeDec.setDimensionsDeviceIndependent(0,screenHeight*20/100,screenHeight*8/100,screenHeight*8/100);
    keyPosChangerFull.setDimensions(keySizeDec.width,keySizeDec.startY,(screenWidth-(2*keySizeDec.width)),keySizeDec.height);
    keyPositionChanger.setDimensions(keyPosChangerFull.startX+positionChangerMovedDistance,keyPosChangerFull.startY,(keyPosChangerFull.width*noOfKeysVisible)/TOTALWHITEKEYS,keyPosChangerFull.height);
    keyAdjusterTranslations[0]=screenWidth-keySizeDec.width;
    keyAdjusterTranslations[1]=0;

   ///
    float whiteKeyHeight=screenHeight-(keySizeDec.startY+keySizeDec.height);
    whiteKey.setDimensions(keySizeDec.startX-keyGlobalTranslateX,keySizeDec.startY+keySizeDec.height,whiteKeyWidth,whiteKeyHeight);
    blackKey.setDimensions(whiteKey.startX,whiteKey.startY,blackKeyWidth,whiteKey.height*0.6);//startX same as whitekey and setBlackkey translation done accorfing to this
    setBlackKeyTranslations();
    setKeyNameDims();
}

void PianoSynthNode::onNonKeyAction(int pointerId, float touchX, float touchY, PianoActionType actionType)
{
    if(actionType==ACTION_DOWN)
    {
        if(touchY>=keySizeDec.startY)//<=keySIzeDec.startY+keySizeDec.height is not checked cuz this function is called if already above keys
        {
            /////////////////////keySize and position modifiers

            if(touchX<=(keySizeDec.startX+keySizeDec.width))//decrease the noOFVisibleKeys
            {
                if(noOfKeysVisible>MINWHITEKEYSVISIBLE)
                {
                    noOfKeysVisible--;
                    float whiteKeyWidth=((float)displayParams.screenWidth-(noOfKeysVisible-1)*keyGap)/(noOfKeysVisible);
                    float blackKeyWidth=0.6*whiteKeyWidth;
                    noTimesOfKeyTranslateX+=0.5;//halfkey left halfkey right
                    positionChangerMovedDistance=noTimesOfKeyTranslateX*keyPosChangerFull.width/(TOTALWHITEKEYS);
                    keyGlobalTranslateX=noTimesOfKeyTranslateX*(whiteKeyWidth)+(int(noTimesOfKeyTranslateX)-1)*keyGap;
                    setDimensions(noOfKeysVisible);///////////reduce this function to update only needed ones
                   // PianoLog("decrese noOfKeys");
                }

            }
            else if(touchX>=(displayParams.screenWidth-keySizeDec.width))//increase the no of keys and setNewDimension
            {
                if(noOfKeysVisible<MAXWHITEKEYSVISIBLE)
                {
                    noOfKeysVisible++;
                    float whiteKeyWidth=((float)displayParams.screenWidth-(noOfKeysVisible-1)*keyGap)/(noOfKeysVisible);
                    float blackKeyWidth=0.6*whiteKeyWidth;
                    static float allKeysBarKeyWidth=keyPosChangerFull.width/TOTALWHITEKEYS;
                    float endDiff=(keyPosChangerFull.endX()-keyPositionChanger.endX());
                    if(endDiff>=(allKeysBarKeyWidth/2))
                    {
                        noTimesOfKeyTranslateX-=0.5;//halfkey left halfkey right
                    }
                    else if(endDiff<(allKeysBarKeyWidth/2))              //if need check condition where abouve or this conditons dont work
                    {
                        noTimesOfKeyTranslateX-=((1.0)-endDiff*(TOTALBLACKKEYS)/keyPosChangerFull.width);

                    }
                    else
                    {
                        noTimesOfKeyTranslateX-=1.0;
                    }
                    if(noTimesOfKeyTranslateX<0.0)
                    {
                        noTimesOfKeyTranslateX+=-1*noTimesOfKeyTranslateX;
                    }

                    positionChangerMovedDistance=noTimesOfKeyTranslateX*keyPosChangerFull.width/(TOTALWHITEKEYS);
                    keyGlobalTranslateX=noTimesOfKeyTranslateX*(whiteKeyWidth)+(int(noTimesOfKeyTranslateX)-1)*keyGap;
                    setDimensions(noOfKeysVisible);///////////reduce this function to update only needed ones
                   // PianoLog("increase noOfKeys");

                }

            }
            else if(touchX>=keyPositionChanger.startX&&touchX<=(keyPositionChanger.startX+keyPositionChanger.width))//positionChange
            {
              //  PianoLog("touched the positionChanger");
                if(!positionSliderActive) //only first finger activates and controls the slider next fingers are ignored
                {
                    positionSliderActive = true;
                    positionChangerPointerId = pointerId;
                    previousPosSliderTouchX=touchX;
                    previousPosSliderTouchY=touchY;
                }
            }


        }
    }
    else if(actionType==ACTION_MOVE)/////monitoring pointer of some slider motion not piano check pointer id and do action accordingly
    {
        if(pointerId==positionChangerPointerId)//positionChanger moved
        {
            //  PianoLog("the position changer moved");
            float moveDisX = touchX - previousPosSliderTouchX;
            if (moveDisX !=0)//only if moved
            {
                float maxMoveAllowed = keyPosChangerFull.width - keyPositionChanger.width;
                positionChangerMovedDistance += moveDisX;
                if (positionChangerMovedDistance > maxMoveAllowed)
                {
                    positionChangerMovedDistance = maxMoveAllowed;
                } else if (positionChangerMovedDistance < 0)
                {
                    positionChangerMovedDistance = 0;
                }
                noTimesOfKeyTranslateX=(positionChangerMovedDistance*(TOTALWHITEKEYS))/keyPosChangerFull.width;
                keyGlobalTranslateX=noTimesOfKeyTranslateX*whiteKey.width+(int (noTimesOfKeyTranslateX)-1)*keyGap;
               setDimensions(noOfKeysVisible);///////////reduce this function to update only needed ones
                //update shader variable if only updating vertices once;
                previousPosSliderTouchX = touchX;
                previousPosSliderTouchY = touchY;
            }
        }

    }
}

void PianoSynthNode::draw()
{
   // whiteKey.printVertices();
   // setBlackKeyTranslations();///////erron in ifelse (keydown/move/all && keydow[on]=1 in onkeyaction for key or else not needed this here
    glUseProgram(programId);
    float displayParameters[]={(float)displayParams.screenWidth,(float)displayParams.screenHeight,whiteKey.width,keyGap,1};//1 padheight remove
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, whiteKey.vertices);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void *) blackKey.vertices);
    glUniform1fv(5,5,displayParameters);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void *) topBackground.vertices);
   // setBlackKeyTranslations();
    glUniform1fv(10,5,keyTranslationX);
    glUniform1i(120,0);//drawtype
    /////keyPOSIOTN AND SIZE MODIFIERS
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 0, (void *) keySizeDec.vertices);
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE, 0, (void *) keyPositionChanger.vertices);//keyPositonMo
    glUniform1fv(122,2,keyAdjusterTranslations);//keyAdjustTranslation

    /////keyPOSIOTN AND SIZE MODIFIERS
    float textCood[]={0,1,1,1,1,0,0,0};/////////redo
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6,2,GL_FLOAT,GL_FALSE,0,(void *)textCood);
    glEnableVertexAttribArray(7);
    glVertexAttribPointer(7,2,GL_FLOAT,GL_FALSE,0,keyPosChangerFull.vertices);
    glEnableVertexAttribArray(8);
    glVertexAttribPointer(8,2,GL_FLOAT,GL_FALSE,0,(void *)noteNameDims.vertices);
    glEnableVertexAttribArray(9);
    glVertexAttribPointer(9,2,GL_FLOAT,GL_FALSE,0,(void *)octaveNumDims.vertices);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,texId);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D_ARRAY,noteNamesTexId);
    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, keyIndices,196);//keys background  and keySize and position changers' and allkeys bar-92  + 52 note names + 52 octaveNum
   // glDrawArraysInstanced(GL_TRIANGLE_FAN,0,4,145);//no indices required
    glUniform1i(120,1);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_SHORT,keyIndices);//drawing positionchanger
    glDisable(GL_BLEND);

 }
void PianoSynthNode::setBlackKeyTranslations()
{
    //define variables(not vertices) in screencoordinates in shader normalize to get the accurate postions and gaps between keys
   // keyGap=2.0;///in pixels also black key width should change with this
   float tempDif=(blackKey.width+keyGap)/2;
    keyTranslationX[0]=(whiteKey.width+keyGap)-tempDif;//KEY TRANSLATIONS ONLY FOR BlACK  KEYS a#
    keyTranslationX[1]=3*(whiteKey.width+keyGap)-tempDif;//c#
    keyTranslationX[2]=4*(whiteKey.width+keyGap)-tempDif;//d#
    keyTranslationX[3]=6*(whiteKey.width+keyGap)-tempDif;
    keyTranslationX[4]=7*(whiteKey.width+keyGap)-tempDif;
}

bool PianoSynthNode::onPlay(void *audioData, int32_t numFrames)
{
    outBuf=(int16_t *)audioData;
    static int id;//id is key or soundNo
    updatePlaylistsFromMidiMsgs();
//////////////////Fill AudioBuffer
   memset(outBuf,0,numFrames*4);////////////////////////check if all zeroes and remove to decrease latency
    if(playListCount!=0)
    for(int i=0;i<playListCount;i++)//i is the list index of playLIst
    {
       // PianoLog("the count in lop is %d ",playListCount);
        id=playList[i];
        for (int j = 0; j < numFrames *2; j += 2)
        {
            outBuf[j] += (audio[id][curPos[id]]/playListCount);
            outBuf[j + 1] += (audio[id][curPos[id]+1]/playListCount);
            curPos[id] += 2;
            if ((curPos[id]+1) >= totLen[id]/2)
            {

                curPos[id] = 0;
               updatePlayList(id,0,REMOVEKEY);
                i--;//cus after remove the current place is assign to other
               break;
            }


        }

    }
 //  PianoLog("buffer filled");
    return true;
}
bool PianoSynthNode::onPlayNew(void *audioData, int32_t numFrames)
{
    outBuf=(int16_t *)audioData;
    static int id;//id is key or soundNo
    updatePlaylistsFromMidiMsgs();
//////////////////Fill AudioBuffer
    memset(outBuf,0,numFrames*4);////////////////////////check if all zeroes and remove to decrease latency
    if(playListCount!=0)
        for(int i=0;i<playListCount;i++)//i is the list index of playLIst
        {
            id=playList[i];
            for (int j = 0; j < numFrames *2; j += 2)
            {
                outBuf[j] += (audio[id][curPos[id]]/playListCount);
                outBuf[j + 1] += (audio[id][curPos[id]+1]/playListCount);
                curPos[id] += 2;
                if ((curPos[id]+1) >= totLen[id]/2)
                {
                   curPos[id] = 0;
                    updatePlayList(id,0,REMOVEKEY);
                    i--;//cus after remove the current place is assign to other
                    break;
                }


            }

        }
        return true;
}
void PianoSynthNode::updatePlaylistsFromMidiMsgs()
{
    if (pthread_mutex_lock(&msgLock) == 0)
    {
        readMsgCount = msgCount;
        pthread_mutex_unlock(&msgLock);
    }
    if (readMsgCount > 0)
    {
    for (int i = 0; i < readMsgCount; i++)
    {
        if (midiMsgs[readOffset][1] == 0)//replace 5 with 0
        {

                updatePlayList(midiMsgs[readOffset][0], 0, STOPKEY);
                //curPos[midiMsgs[readOffset][0]] = 0;

        }
        else if (midiMsgs[readOffset][1] == 1)
        {
            updatePlayList(midiMsgs[readOffset][0], 0, ADDKEY);
            curPos[midiMsgs[readOffset][0]] = 0;

        }
        readOffset++;
        if (readOffset > 49)//49 size of midimsgs-1
        {
            readOffset = 0;
        }
    }
    //////////done reading msgs
        if (pthread_mutex_lock(&msgLock) == 0)
        {
            msgCount -= readMsgCount;
            pthread_mutex_unlock(&msgLock);
        }
        readMsgCount = 0;
        ////////////////remove all from playlist that are in pauselist
    if(playListCount>3&&pauseListCount>0)
    {
         playPauseDiff=playListCount-3;/////3 is the min allow to playOnce on pause  which is same as in the above line if 3

        if(pauseListCount<=playPauseDiff)
        {
            playPauseDiff=pauseListCount;
        }
        if(playPauseDiff>0)
        {
            for (int i = (playListCount - 1); i >= 0; i--)
            {
                for(int k=0;k<playPauseDiff;k++)
                {
                    if(playList[i]==pauseList[k])
                    {

                        playList[i] = playList[playListCount - 1];
                        playListCount--;
                        nextUpdateIndex = playListCount;
                        curPos[pauseList[k]] = 0;
                    }
                }
            }
            pauseListCount-=playPauseDiff;
            for(int p=0;p<pauseListCount;p++)
            {
                pauseList[p]=pauseList[p+playPauseDiff];
            }
        }
    }
   // PianoLog("the playlistCount-%d and pauseListCount-%d",playListCount,pauseListCount);
    }
}
void PianoSynthNode::updatePlayList(int keyNo, int index, UpdateKey updateType)
{
   // PianoLog("the playListCount is %d",playListCount);
        bool updateReq=true;
        if(updateType==ADDKEY)
        {
          for(int i=(playListCount-1);i>=0;i--)
            {
              if(playList[i]==keyNo)//if key already in list dont add
              {
                  updateReq=false;
                  for(int k=(pauseListCount-1);k>=0;k--)
                  {
                      if(pauseList[k]==keyNo)
                      {
                          pauseListCount--;
                       //   PianoLog("the pause list in addkey is %d",pauseListCount);
                          for(int s=k;s<playListCount;s++)
                          {
                              pauseList[s]=pauseList[s+1];
                          }
                          break;
                      }

                  }
                      break;
              }

            }
          if(updateReq)
          {
              playList[nextUpdateIndex]=keyNo;
              playListCount++;
              nextUpdateIndex=playListCount;
          }

        }
        else if(updateType==STOPKEY)////////this
        {
            for (int i = (playListCount - 1); i >= 0; i--)
            {
                if(playList[i]==keyNo)
                {
                    pauseList[pauseListCount] = keyNo;
                    pauseListCount++;
                    break;
                }

            }
           /* if(playListCount>3&&pauseListCount>0)///////////
               {
                   for (int i = (playListCount - 1); i >= 0; i--) {
                       if (playList[i] == pauseList[0])/////remove all previous key if playlistcount>3 or as requrired.
                       {
                           playList[i] = playList[playListCount - 1];
                           playListCount--;
                           nextUpdateIndex = playListCount;
                           curPos[pauseList[0]] = 0;
                           pauseListCount--;
                           for (int k = 0; k <pauseListCount; k++)//////use offset instead of looping like midimsgoffsets
                           {
                               pauseList[k] = pauseList[k + 1];
                           }
                        //   PianoLog("The  value of pauseListCount inside stop  is %d",pauseListCount);

                           break;
                       }
                   }
               }*/


        }
        else
            {
            ///////////////REMOVE KEY SAME AS STOPKEY EXCEPT NEED NOT UPDATE PAUSE LIST BUT DIRECTLY REMOVEKEY
            for (int i = (playListCount - 1); i >= 0; i--)
            {
                if (playList[i] == keyNo)
                {
                    playList[i] = playList[playListCount - 1];
                    playListCount--;
                    nextUpdateIndex = playListCount;
                    break;
                }

            }
            ////////remove the key from pause list also
            for(int i=(pauseListCount-1);i>=0;i--)
            {
                if(pauseList[i]==keyNo)//////use offset instead of looping like midimsgoffsets
                {
                    pauseListCount--;
                    for(int k=i;k<pauseListCount;k++)
                    {
                        pauseList[k]=pauseList[k+1];
                    }
                    break;
                }
            }
            }
}

void PianoSynthNode::setKeyNameDims()
{

    noteNameDims.setDimensions(whiteKey.startX,blackKey.endY(),whiteKey.width/2,whiteKey.width/1.8);
    octaveNumDims.setDimensions(noteNameDims.endX(),noteNameDims.startY,noteNameDims.width,noteNameDims.height);

}
