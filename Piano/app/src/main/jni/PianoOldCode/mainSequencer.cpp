//
// Created by pspr1 on 6/21/2020.
//
#include<android_native_app_glue.h>
#include <unistd.h>
#include "android/log.h"
#include "glSetup.h"
#include "inputoutput.h"
#include "helperfuncs.h"
#include "shapes.h"
#include "piano.h"
#include "audiomanager.h"
#include "ui.h"
#include "inputoutput.cpp"
#include "main.h"
#include "javacalls.h"
#include "filemanager.h"
#define TAG "MAINSEQUENCER"
#include "text.h"
#include "firebase.h"
AudioEngine audioEngine;
TextEngine textEngine;
//#define LOGE(...)((void)__android_log_print(ANDROID_LOG_INFO,TAG,__VA_ARGS__));
#define SMain(...)((void)__android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__))
firebase::admob::BannerView* banner_view;
firebase::admob::AdRequest my_ad_request;
bool load=false,show=false;
DisplayParams displayParams;
void android_main(struct android_app* app){////////////////main
papp=app;
    contextApp.app = app;
    getDisplayParams(contextApp.app,&displayParams);
    hideSystemUI(app);
    textEngine.init();
    setupFireBaseAndLoadBanner(app);
    app->onAppCmd = onCmd;
    app->onInputEvent = onInput;

    int32_t eventId, events, fdesc;
    android_poll_source *source;
    while (true) {//SMain("loooping");
        while ((eventId = ALooper_pollAll(0, &fdesc, &events, (void **) &source)) >= 0)
        {
            if (source != NULL)
            {
                source->process(app, source);
                if(contextApp.windowInitStatus)
                drawFrame();
            }
        }
       if(!show)
        {
            checkBanner();
        }
        if (app->destroyRequested)
            break;
    }
}////////////////main
void drawFrame()
{
    glClearColor(0.0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);
    piano.draw();
    eglSwapBuffers(contextApp.display,contextApp.surface);


}
void initializeParams()
{
    piano.textEngine=&textEngine;//textengine is initilize in main
    piano.pianoInitialSetup(contextApp.app);//text engine and set for the piano before this
    SMain("pianosetup success");

}

void destroyParams()
{
    //clear audio (audio data pointers free)pianoSounds.soundData;
   // close(audioPipe[0]);
  //  close(audioPipe[1]);
}






