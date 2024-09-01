//
// Created by shivaaz on 11/12/22.
//

#ifndef KALASOFT_PIANOAPPLICATION_H
#define KALASOFT_PIANOAPPLICATION_H


#include <KSApp/KSApplication.h>
#include <KSUI/ViewGroup.h>
#include "Codec/WavDecoder.h"

#include "Audio/SLESAudioPlayer.h"
#include "Piano/PianoSynthNode.h"
#include "UI/RainbowPianoView.h"
#include "Text/Text.h"
#include "PianoController.h"
#include "Audio/OboeAudioRenderNode.h"
#include "Midi/MidiPlayer.h"
#include "UI/PianoView.h"


class PianoApplication : public KSApplication{

public:

    PianoApplication() = delete;

    PianoApplication(android_app *app);

    bool onInterceptMotionEvent(const ks::MotionEvent &me) override;

protected:

    void onCreate() override;

    void onStart() override;

    void onResume() override;

    void onPause() override;

    void onStop() override;

    void onDestroy() override;

    void onWindowInit() override;

    void onWindowResized() override;

    void onWindowRedrawNeeded() override;

    void onWindowTermination() override;

    void onContentRectChanged() override;

    void onLowMemory() override;

    void onFocusLost() override;

    void onFocusGained() override;

    void onConfigChanged() override;

    void onSaveState() override;

    void onInputChange() override;

    void runTasks() override;

    void resizeUI();

protected:

    virtual void onDraw() override;

private:

    void createContentView();

private:

    PianoSynthNode piano;
    ViewGroup *contentView = nullptr;
    ViewGroup *midFrame = nullptr;
    GLImageView *topFrame = nullptr;
    PianoView *pianoVew = nullptr;

    MidiPlayer midiPlayer;
    //This controls UI and Music Synth
    PianoController pianoController;
    IPianoController *pianoUIController;

    TextEngine textEngine;

};


#endif //KALASOFT_PIANOAPPLICATION_H
