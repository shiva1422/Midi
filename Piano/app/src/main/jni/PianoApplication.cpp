//
// Created by shivaaz on 11/12/22.
//

#include <Shader.h>
#include "PianoApplication.h"
#include "UI/RainbowPianoTouchListener.h"
#include "GN/IMediaNode.h"


using namespace ks;
PianoApplication::PianoApplication(android_app *app): KSApplication(app,"PianoApplication")
{

}
void PianoApplication::runTasks()
{

}

void PianoApplication::resizeUI()
{
    View *v = static_cast<View *>(renderer.getContent());
    //Should resize VIews automatically TODO but for now only PianoView so resize it
    v->setBounds(displayMetrics.screenWidth*0/4, displayMetrics.screenHeight/4, displayMetrics.screenWidth, displayMetrics.screenHeight/2);


}


void PianoApplication::onCreate()
{
    KSApplication::onCreate();

  //  renderer.setBackGroundColor(1.0,1.0,0.0,1.0);
    textEngine.setDisplayMetrics(displayMetrics);
    textEngine.init();
    //pianoVew = new RainbowPianoView(400, 200, 400, 400, &textEngine);//TODO textEngine Independent of PianoView
    //pianoVew = new RainbowPianoView(0, 0, displayMetrics.screenWidth, displayMetrics.screenHeight, &textEngine);//TODO textEngine Independent of PianoView
    pianoVew = new RainbowPianoView(0, displayMetrics.screenHeight/2, displayMetrics.screenWidth, displayMetrics.screenHeight/2, &textEngine);//TODO textEngine Independent of PianoView
   // pianoVew->setShaderProgram(Shader::createProgram(vertA,fragA));
    pianoVew->prepare(this);

    pianoVew->setTouchListener(new RainbowPianoTouchListener());

   // pianoVew->setBounds(0,0,1000,500);
    pianoVew->setBackgroundColor(0.0,0.0,0.,1.0);

    pianoUIController = pianoVew->getController();

    pianoController.setUIController(pianoUIController);

    pianoVew->connectController(pianoController.getController());

    pianoController.setAssetManager(this);

    pianoController.prepare();

    std::string midipath = "music/midi/havana.mid";

    midiPlayer.setMediaSource(openAsset(midipath.c_str()));
    midiPlayer.setListenerPiano(pianoController.getController());

    setContentView(pianoVew);

    midiPlayer.play();//TODO;

}

void PianoApplication::onDraw()
{
    KSLOGD(appName.c_str(),"onDraw");
   // midiPlayer.refresh();
    KSApplication::onDraw();
}

void PianoApplication::onStart()
{
    KSApplication::onStart();
}

void PianoApplication::onResume()
{
    KSApplication::onResume();
}

void PianoApplication::onPause()
{
    KSApplication::onPause();
}

void PianoApplication::onStop()
{
    KSApplication::onStop();
}

void PianoApplication::onDestroy()
{
    KSApplication::onDestroy();
}

void PianoApplication::onWindowInit()
{
    KSApplication::onWindowInit();

    int rW = renderer.getWidth();
    int rH = renderer.getHeight();
    if(rW != displayMetrics.screenWidth || rH != displayMetrics.screenHeight)
    {
       KSLOGW(this->appName.c_str(),"DisplayMetrics and RenderWIndow (ANativeWindow ) size mismatch DM(%d,%d) window(%d,%d)",displayMetrics.screenWidth,displayMetrics.screenHeight,rW,rH);

        displayMetrics.screenWidth = rW;
        displayMetrics.screenHeight = rH;

        forceUpdateDisplayMetrics(displayMetrics);

        resizeUI();

       KSLOGW("KS Event","resize UI");
    }

    KSLOGD("onWindowInit","RenderWindow set");
}


bool PianoApplication::onInterceptMotionEvent(const ks::MotionEvent &me)
{
    View *content = getContentView();
    if(!content)return false;//Right? if contentView Changes possible? no

  //if(!content->isPointInside(touchX,touchY)) return false;//TODO now only handling oneView check handling in ViewGroups

    int32_t pointerIndex = me.getPointerIndex();
    int32_t pointerId = me.getPointerId(pointerIndex);
    float touchX = me.getX(pointerIndex);
    float touchY = me.getY(pointerIndex);


    switch(me.getAction())
    {
        case EMotionEventAction::DOWN:
        {
            if(content->isPointInside(touchX,touchY))
            {
                //TODO later avoid or do as required
               return View::dispatchTouchDown(content,touchX,touchY,pointerId,true);
            }
        }break;

        case EMotionEventAction::POINTER_DOWN:
        {
            if(content->isPointInside(touchX,touchY))
            {
                //TODO later avoid or do as required
                return View::dispatchTouchDown(content,touchX,touchY,pointerId,false);
            }
        }break;

        case EMotionEventAction::MOVE:
        {
           int32_t pointerCount = me.getPointerCount();
            //processing for all pointers call only if historyValue and current value differ for particular index
            //TODO maybe need to check touch event history cause when swipes speed across screen some ponts may be missiong
            //therfore some enents may not be processed or events may be repeated check;
            KSLOGD("Action move:","count %d",pointerCount);
            for(int i = 0;i < pointerCount; ++i)
            {
                touchX = me.getX(i);
                touchY = me.getY(i);
                pointerId = me.getPointerId(i);

                /*
               //if(tThe view/touch listener handling this pointerId as touchdown/pointer down or hoveenter maybe.
               //      if touch inside that view forward motion event to that view
               //      else forward hoverexit to the view and hover enter to the view which has touch inside
                *else getView as this location forward hover/enter/move?
                */

                if(View::isHandlingTouch(content, pointerId))
                {
                    if(content->isPointInside(touchX,touchY))
                    {
                        View::dispatchMove(content,touchX,touchY,pointerId);//todo result?
                    }
                    else
                    {
                        View::dispatchHoverExit(content,pointerId);//also hover enter to view at location
                        //TODO hover exit of content and hover enter to whatever View is at this location now
                    }

                }
                else
                {
                    //TODO
                   KSLOGE("Action Move","unhandled");
                }

            }

            //TODO return ?
        }break;
        case EMotionEventAction::POINTER_UP:
        {
            //Consider hover enter exit TODO
            if(content->isPointInside(touchX,touchY))
            {
                //TODO later avoid or do as required
                return View::dispatchTouchUp(content,touchX,touchY,pointerId,false);
            }
        }break;
        case EMotionEventAction::UP:
        {
            //Consider hover enter exit TODO
            if(content->isPointInside(touchX,touchY))
            {
                //TODO later avoid or do as required
                return View::dispatchTouchUp(content,touchX,touchY,pointerId,true);
            }
        }break;

        default:
            assert(false);//TODO

    }
    return false;
}
void PianoApplication::onWindowResized()
{
    KSApplication::onWindowResized();

}

void PianoApplication::onWindowRedrawNeeded()
{
    KSApplication::onWindowRedrawNeeded();
}

void PianoApplication::onWindowTermination()
{
    KSApplication::onWindowTermination();
}

void PianoApplication::onContentRectChanged()
{
    KSApplication::onContentRectChanged();
}

void PianoApplication::onLowMemory() {
    KSApplication::onLowMemory();
}

void PianoApplication::onFocusLost()
{
    KSApplication::onFocusLost();
}

void PianoApplication::onFocusGained()
{
    KSApplication::onFocusGained();
}

void PianoApplication::onConfigChanged()
{
    KSApplication::onConfigChanged();
}

void PianoApplication::onSaveState()
{
    KSApplication::onSaveState();
}

void PianoApplication::onInputChange()
{
    KSApplication::onInputChange();
}




