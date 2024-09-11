//
// Created by shivaaz on 11/12/22.
//

#include <Shader.h>
#include "PianoApplication.h"
#include "UI/RainbowPianoTouchListener.h"
#include "GN/IMediaNode.h"
#include "UI/PianoView.h"


using namespace ks;
PianoApplication::PianoApplication(android_app *app): KSApplication(app,"PianoApplication")
{

}

PianoApplication::~PianoApplication() {

}


void PianoApplication::runTasks()
{

}

void PianoApplication::resizeUI()
{
    View *v = static_cast<View *>(renderer->getContent());
    //Should resize VIews automatically TODO but for now only PianoView so resize it
    v->setBounds(displayMetrics.screenWidth*0/4, 0, displayMetrics.screenWidth, displayMetrics.screenHeight);
    topFrame->setBounds(v->getStartX(),v->getStartY(),v->getWidth(),v->getHeight() * 20/100);
    settingsView->setBounds(topFrame->getStartX(),topFrame->getEndY(),topFrame->getWidth(),v->getHeight() * 10/100);
    pianoVew->setBounds(settingsView->getStartX(), settingsView->getEndY() , v->getWidth(),v->getHeight()*70.0/100 );//TODO this actually be triggered inside the viewgroup;

    float lightHeight = settingsView->getHeight() * 0.2;
    light1->setBounds(settingsView->getStartX(),settingsView->getEndY() - lightHeight/2.0 ,settingsView->getWidth(),lightHeight);
    //light2->setBounds(settingsView->getStartX(),settingsView->getStartY()-lightHeight/2.0,settingsView->getWidth(),lightHeight);


}


void PianoApplication::onCreate()
{
    KSApplication::onCreate();

    KSLOGV(appName.c_str(),"prepare Shaders %d", Shader::prepareShaders(this));//TODO move to application/Graphics aptly

  //  renderer.setBackGroundColor(1.0,1.0,0.0,1.0);
    textEngine.setDisplayMetrics(displayMetrics);
    textEngine.init();
    //pianoVew = new RainbowPianoView(400, 200, 400, 400, &textEngine);//TODO textEngine Independent of PianoView
    //pianoVew = new RainbowPianoView(0, 0, displayMetrics.screenWidth, displayMetrics.screenHeight, &textEngine);//TODO textEngine Independent of PianoView

    setContentView(createContentView());

   // midiPlayer.play();//TODO;

}

View * PianoApplication::createContentView() {

    contentView = new ViewGroup();
    pianoVew = new PianoView();
    settingsView = new PianoSettingsView();
    topFrame = new GLImageView();
    light1 = new GLImageView();
    //light2 = new GLImageView();

    settingsView->setPianoControl(pianoVew);
    pianoVew->setTextEngine(&textEngine);
    pianoVew->prepare(this);
    settingsView->prepare();
    topFrame->setImage("icons/topframe.png");
    light1->setImage("icons/light.png");
    //light2->setImage("icons/light.png");


    contentView->addView(topFrame);
    //contentView->addView(light2);
    contentView->addView(settingsView);
    contentView->addView(pianoVew);
    contentView->addView(light1);



    pianoVew->setTouchListener(new PianoTouchListener());

    setContentView(contentView);

    resizeUI();//TODO better//On ViewWillAppear

    pianoUIController = pianoVew->getController();

    pianoController.setUIController(pianoUIController);

    pianoVew->connectController(pianoController.getController());

    pianoController.setAssetManager(this);

    pianoController.prepare();


    return contentView;

}


void PianoApplication::onDraw()
{
   // KSLOGD(appName.c_str(),"onDraw");
    KSApplication::onDraw();
}

void PianoApplication::onStart()
{
    KSApplication::onStart();
}

void PianoApplication::onResume()
{
    KSApplication::onResume();
    pianoController.resume();
}

void PianoApplication::onPause()
{
    KSApplication::onPause();
    pianoController.pause();
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

    int rW = renderer->getWidth();
    int rH = renderer->getHeight();
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
    //Move logic here to a touch Manager;

    View *content = getContentView();
    if(!content)return false;//Right? if contentView Changes possible? no

  //if(!content->isPointInside(touchX,touchY)) return false;//TODO now only handling oneView check handling in ViewGroups

    int32_t pointerIndex = me.getPointerIndex();
    int32_t pointerId = me.getPointerId(pointerIndex);
    float touchX = me.getX(pointerIndex);
    float touchY = me.getY(pointerIndex);

     bool res = false;


     static std::unordered_set<TouchID> touchesActive;//

    switch(me.getAction())
    {
        case EMotionEventAction::DOWN:
        {
            KSLOGD("KSEVENT", " Action down Id - %d, index %d",pointerId,pointerIndex);

            if(content->isPointInside(touchX,touchY))
            {
                //TODO later avoid or do as required
               return View::dispatchTouchDown(content,touchX,touchY,pointerId,true);
            }
        }break;

        case EMotionEventAction::POINTER_DOWN:
        {
            KSLOGD("KSEVENT", " PointerDown Id - %d, index %d",pointerId,pointerIndex);

            if(content->isPointInside(touchX,touchY))
            {
                //TODO later avoid or do as required
                return View::dispatchTouchDown(content,touchX,touchY,pointerId,false);
            }
        }break;

        case EMotionEventAction::MOVE:
        {
            KSLOGD("KSEVENT", " Action move Id - %d, index %d",pointerId,pointerIndex);//The id index would be first one when multiple touches active

            int32_t pointerCount = me.getPointerCount();
            //processing for all pointers call only if historyValue and current value differ for particular index
            //TODO maybe need to check touch event history cause when swipes speed across screen some ponts may be missiong
            //therfore some enents may not be processed or events may be repeated check;
            KSLOGD("Action move:","count %d",pointerCount);
            for(int i = 0;i < pointerCount; ++i)
            {
                touchX = me.getX(i);
                touchY = me.getY(i);
                pointerId = me.getPointerId(i);//TODO check historical value changed and dispatch,move else moving one finger will trigger motionevent on all view with fingers that haven't moved;

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
                        View::dispatchHoverExit(content,touchX,touchY,pointerId);//also hover enter to view at location
                        //TODO hover exit of content and hover enter to whatever View is at this location since content view is the only one, no view for hover enter.
                    }

                }
                else
                {
                    //TODO
                   KSLOGE("KSEVENT"," action moveunhandled Id - %d, index %d",pointerId,pointerIndex);
                }

            }

            //TODO return ?
        }break;
        case EMotionEventAction::POINTER_UP:
        {
            KSLOGD("KSEVENT", " Action pointer up Id - %d, index %d",pointerId,pointerIndex);
            //Consider hover enter exit TODO
            if(content->isPointInside(touchX,touchY))
            {
                //TODO later avoid or do as required
                return View::dispatchTouchUp(content,touchX,touchY,pointerId,false);
            }
        }break;
        case EMotionEventAction::UP:
        {
            KSLOGD("KSEVENT", " Action up Id - %d, index %d",pointerId,pointerIndex);

            //Consider hover enter exit TODO
            if(content->isPointInside(touchX,touchY) )//|| View::isHandlingTouch(content, pointerId) )
            {
                //TODO later avoid or do as required
                return View::dispatchTouchUp(content,touchX,touchY,pointerId,true);
            }else
            {
                KSLOGW("KSEVENT", "Action up warning");//Touch up outside content view/should already be handle in move,as hoverExit

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




