//
// Created by pspr1 on 8/2/2020.
//

#ifndef DIGIBAND_PIANO_H
#define DIGIBAND_PIANO_H

#include "ui.h"
#include "inputoutput.h"
#include <oboe/AudioStreamBuilder.h>
#include "oboe/Oboe.h"
#include "fcntl.h"
#include "audiomanager.h"
#include "audiomanager.h"
#include "pixamap.h"
#include "text.h"


#define TAGPIANO "PIANO"
//#define LOGE(...)((void)__android_log_print(ANDROID_LOG_INFO,TAG,__VA_ARGS__));
#define PianoLog(...)((void)__android_log_print(ANDROID_LOG_ERROR,TAGPIANO,__VA_ARGS__))

enum  KeyColor{BLACK,WHITE};
enum PianoActionType {ACTION_DOWN,ACTION_UP,ACTION_MOVE,ACTION_MOVEOUT};
enum UpdateKey{ADDKEY,REMOVEKEY,STOPKEY};

//const uint TOTALKEYS=88;
const uint TOTALWHITEKEYS=52;
const uint TOTALBLACKKEYS=37;//fist black key is not drawn g#0 so be careful
const uint TOATALPADS=8;
const uint TOTALKEYSANDPADS=TOATALPADS+TOTALWHITEKEYS+TOTALBLACKKEYS;
//below keyfrequncies and keycodes are in order of piano convert aptly while playing in app(not in order)
//from G#0 to C8
const float keyFrequencies[89]={25.95654,27.50000,29.13524,30.86771,
 32.70320,34.64783,36.70810,38.89087,41.20344,43.65353,46.24930,48.99943,51.91309,55.00000,58.27047,61.73541,
 65.40639,69.29566,73.41619,77.78175,82.40689,87.30706,92.49861,97.99886,103.8262,110.0000,116.5409,123.4708,
 130.8128,138.5913,146.8324,155.5635,164.8138,174.6141,184.9972,195.9977,207.6523,220.0000,233.0819,246.9417,
 261.6256,277.1826,293.6648,311.1270,329.6276,349.2282,369.9944,391.9954,415.3047,440.0000,466.1638,493.8833,
 523.2511,554.3653,587.3295,622.2540,659.2551,698.4565,739.9888,783.9909,830.6094,880.0000,932.3275,987.7666,
 1046.502,1108.731,1174.659,1244.508,1318.510,1396.913,1479.978,1567.982,1661.219,1760.000,1864.655,1975.533,
 2093.005,2217.461,2349.318,2489.016,2637.020,2793.826,2959.955,3135.963,3322.438,3520.000,3729.310,3951.066,
                                4186.009

};
const uint8_t pianoKeyNo[89]={1,3,4,6,8,9,11,13,15,16,18,20,21,23,
                              25,27,28,30,32,33,35,37,39,40,42,44,45,47,
                              49,51,52,54,56,57,59,61,63,64,66,68,69,71,
                              73,75,76,78,80,81,83,85,87,88,//whitekeys done next black
                              0,2,5,7,10,12,14,17,19,22,24,26,29,31,34,36,38,41,43,46,
                              48,50,53,55,58,60,62,65,67,70,72,74,77,79,82,84,86};//to get the offset of real keyFrequencies above from apps unordered keyNos(0-51 whiteeys then52-88 blackKeys);
enum keyCode{A0=0,AS0,B0,
            C1,CS1,D1,DS1,E1,F1,FS1,G1,GS1,A1,AS1,B1,C2,CS2,D2,DS2,E2,F2,FS2,G2,GS2,A2,AS2,B2,
            C3,CS3,D3,DS3,E3,F3,FS3,G3,GS3,A3,AS3,B3,C4,CS4,D4,DS4,E4,F4,FS4,G4,GS4,A4,AS4,B4,
            C5,CS5,D5,DS5,E5,F5,FS5,G5,GS5,A5,AS5,B5,C6,CS6,D6,DS6,E6,F6,FS6,G6,GS6,A6,AS6,B6,
            C7,CS7,D7,DS7,E7,F7,FS7,G7,GS7,A7,AS7,B7,C8};//JUST CHANGE THE VALUE OF A0 FOR MIDI CODES

class PianoSynthNode {//includes keySize changers and positionSlider
public:
    //ApplicationContext//device depe
    android_app* app;
    //////
    int noOfKeysVisible=15,MINWHITEKEYSVISIBLE=7,MAXWHITEKEYSVISIBLE=52;
    int octaveNum = 0, octaveGap = 0, octaveKeyNo = 0, keyDown;//for Action();
    int keyOn[89];//0-KEYOFF,1-KEYON;
    float keyTranslationX[5];//for blackKeysOnly
    float keyAdjusterTranslations[2];//keySizeDec and keyPositionAduster Translation
    float noTimesOfKeyTranslateX=0;//to calculate keyPositionChanger(translate)
    int finalPreviousKeyDown = -1;//keyDown is also keyUp when actionup
    float keyGlobalTranslateX = 0;///////////////////////////if any error in drawing convert to uint or rearrange as required;
    float keyGap = 2.0, keyGlobalTranslationX;//in pixels//keygap keep even
    int previousPointedKey[50];//50 finger movement//the index is pointerId(monitoing); use 52 for not pointing to any key as its ignored
    /////slider
    int positionChangerPointerId = INT_MAX;//use int max for non usable value
    float previousPosSliderTouchX, previousPosSliderTouchY, positionChangerMovedDistance = 0;
    bool positionSliderActive = false;
    Dimensions blackKey, whiteKey, keySizeDec, topBackground, keyPositionChanger, keyPosChangerFull,noteNameDims,octaveNumDims;//keyPositionChanger is the total width of the position Bar


    //////////OPENGL&&text
    GLuint programId;
    GLushort keyIndices[6] = {0, 1, 2, 2, 3, 0};
    GLuint texBufId,texId,noteNamesTexBufId,noteNamesTexId,allkeysSampler,noteNamesSamples;
    PixaMap pixmap,allKeysTexture,noteNames[19];//noteName include #
    Dimensions noteName;
    bool updateGraphics=false;
    const char *vertexShader="shaders/pianoShaders/vertexShader.glsl";
    const char *fragmentShader="shaders/pianoShaders/fragmentShader.glsl";
    TextEngine *textEngine;

    ////SOUND////////////these are updated only on player thread
    int playPauseDiff;
    int16_t playListCount = 0;
    int16_t pauseListCount=0;
    uint playList[88];
    uint pauseList[88];

    uint nextUpdateIndex=0;//to updatePlayList
    ////midimsg
    int msgCount=0,readMsgCount=0;
    int readOffset=0,writeOffset=0;//offsetShouldbe less than msgCount-1;
    uint8_t midiMsgs[50][2];//max 50 msgs with first index keyNo,sendindex,on/off 0/1
    pthread_mutex_t msgLock=PTHREAD_MUTEX_INITIALIZER;
  ///  PianoSound sounds[63];
    int16_t *audio[88],*outBuf;
    uint curPos[88];
    uint totLen[88];
    //AudioFormatKala audioFormat;
    AudioEngine audioEngine;


    //in constructor set default values.-
    void pianoInitialSetup(android_app *papp);
    void setUpAudio();
    void setDimensions(int noKeysVisible);
    void setupTextures();
    void onNonKeyAction(int pointerId,float touchX,float touchY,PianoActionType actionType);//for keySIzeDec ,inc,keyTranslation
    void updatePlayList(int keyNo,int index,UpdateKey updateType);
    void setBlackKeyTranslations();
    void draw();
    void updateMsgsOnAction(int noOfMsgs,int pointerId);//2 for actionmove and one for up and down
    void updatePlaylistsFromMidiMsgs();
    bool onPlay(void *audioData, int32_t numFrames);//true to continue fasle to stop the audio stream
    bool onPlayNew(void *audioData,int32_t numFrames);
    void destroy()
    {
        //destrouy mutex ,clear audioPipes, use destructor;
    }

    int32_t onAction(int pointerId,float touchX, float touchY, PianoActionType actionType);

    void setKeyNameDims();
};




typedef struct PianoMessage
{
    uint8_t keyNo;
    uint8_t keyStatus;
}PianoMsg;
#endif //DIGIBAND_PIANO_H
