//
// Created by pspr1 on 9/24/2020.
//

#ifndef DIGIBAND_JAVACALLS_H
#define DIGIBAND_JAVACALLS_H

#include "pixamap.h"
#include "android_native_app_glue.h"
#include "commons.h"
#define TAGJC "JAVACALLS"
#define JNILOG(...)((void)__android_log_print(ANDROID_LOG_ERROR,TAGJC,__VA_ARGS__))
void getDisplayParams(android_app *app, DisplayParams *displayParams);
status getPhoto(android_app* app,PixaMap *pixaMap);
status hideSystemUI(android_app *app);
status changeVolume(android_app *app,bool increase);
#endif //DIGIBAND_JAVACALLS_H
