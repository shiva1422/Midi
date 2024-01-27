//
// Created by pspr1 on 6/22/2020.
//

#ifndef DIGIBAND_HELPERFUNCS_H
#define DIGIBAND_HELPERFUNCS_H

#include "glSetup.h"
#define TAGHF "HELPERFUNTIONS"
//#define LOGE(...)((void)__android_log_print(ANDROID_LOG_INFO,TAG,__VA_ARGS__));
#define HF(...)((void)__android_log_print(ANDROID_LOG_ERROR,TAGHF,__VA_ARGS__))

GLuint glQuickSetup(GLContextm* pcontext);
#endif //DIGIBAND_HELPERFUNCS_H
