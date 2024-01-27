//
// Created by shivaaz on 12/18/22.
//

#ifndef KALASOFT_PIANOCONTROLLER_H
#define KALASOFT_PIANOCONTROLLER_H

#include <vector>
#include <KSIO/AssetManager.h>
#include "Piano/IPianoController.h"
#include "GN/IMediaNode.h"
#include "Audio/OboeAudioRenderNode.h"



/**
 * This should be triggered from touch,Keyboard/midi
 * Controls both UI and Music,Make sure all members are initialized right   before calling any function
 */

class PianoController : public IPianoController {

public:

    PianoController();

    int prepare();

    void setAssetManager(AssetManager *manager);

    /***IPianoController Impl***/

    void setKeyState(const EKeyName name, const EKeyState state) override;

    //TODO no need copy can send states as ref;
    virtual void setKeysStates(const std::vector<KeyStateInfo> states) override;


    void setUIController(IPianoController *controller){ uiController = controller;}

private:

    IPianoController *uiController = nullptr;

    IPianoController *synthController = nullptr;

    IMediaNode* pianoSynthNode = nullptr;

    std::shared_ptr<OboeAudioRenderNode> audioRender;

    AssetManager *assetManager = nullptr;


};


#endif //KALASOFT_PIANOCONTROLLER_H
