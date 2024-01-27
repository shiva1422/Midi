//
// Created by pspr1 on 6/22/2020.
//

#ifndef DIGIBAND_INPUTOUTPUT_H
#define DIGIBAND_INPUTOUTPUT_H
#include <stdint.h>
#include<android/log.h>
#include<android_native_app_glue.h>
#define TAGIO "INPUTOUTPUT"
//#define LOGE(...)((void)__android_log_print(ANDROID_LOG_INFO,TAG,__VA_ARGS__));
#define IO(...)((void)__android_log_print(ANDROID_LOG_ERROR,TAGIO,__VA_ARGS__))
void onCmd(android_app* app, int32_t cmd);
int32_t onInput(android_app* papp,AInputEvent* event);





#endif //DIGIBAND_INPUTOUTPUT_H
