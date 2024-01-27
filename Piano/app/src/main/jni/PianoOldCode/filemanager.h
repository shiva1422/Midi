//
// Created by pspr1 on 9/6/2020.
//

#ifndef DIGIBAND_FILEMANAGER_H
#define DIGIBAND_FILEMANAGER_H
#include <android_native_app_glue.h>
#include "android/log.h"
#include "audiomanager.h"
#include "piano.h"
#define TAGFILES "FILEMANAGER"
//#define LOGE(...)((void)__android_log_print(ANDROID_LOG_INFO,TAG,__VA_ARGS__));
#define FilesLog(...)((void)__android_log_print(ANDROID_LOG_ERROR,TAGFILES,__VA_ARGS__))

void loadSounds(android_app *app,int16_t ** audio,uint32_t *totLen);
void getAudioFormatFromFile(android_app *app,AudioFormatKala *format);
#endif //DIGIBAND_FILEMANAGER_H
