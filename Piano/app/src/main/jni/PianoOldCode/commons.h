//
// Created by pspr1 on 6/22/2020.
//

#ifndef DIGIBAND_COMMONS_H
#define DIGIBAND_COMMONS_H
#if __ANDROID_API__ >= 24
#include <GLES3/gl32.h>
#elif __ANDROID_API__ >= 21
#include <GLES3/gl31.h>
#include <EGL/egl.h>

#else
#include <GLES3/gl3.h>
#endif
#include "android_native_app_glue.h"
enum status{STATUS_OK,STATUS_KO,STATUS_KO_NONFATAL,STATUS_KO_FATAL,STATUS_EXTRA};

typedef struct GLContextm{
    int drawing;
    int32_t width,height;
    GLuint programID;
    bool glInitStatus=false,windowInitStatus=false,appFirstOpen=true;
    android_app* app;
    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;
}GLContextm;
typedef struct DisplayParams{
    uint32_t screenWidth,screenHeight,densityDpi,deviceStableDensity,screenStride;
    float density,scaledDensity,xdpi,ydpi;
}DisplayParams;
#endif //DIGIBAND_COMMONS_H
