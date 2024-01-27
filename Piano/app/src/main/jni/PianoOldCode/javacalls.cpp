//
// Created by pspr1 on 9/25/2020.
//
#include <malloc.h>
#include "javacalls.h"
#include "commons.h"
#include "android/log.h"
#include "android/bitmap.h"

void getDisplayParams(android_app *app, DisplayParams *displayParams)
{
    JavaVM *vm = app->activity->vm;
    JNIEnv *env;
    vm->AttachCurrentThread(&env, NULL);
    if (env == NULL)
    {
        JNILOG("coulf not attach/obtain current thread/get java environment");
        return;
    }


    jclass cls = (env)->GetObjectClass(app->activity->clazz);
    if(cls==NULL)
    {
        JNILOG("coulf not get java object class");
        return ;
    }
    jmethodID mid = env->GetMethodID(cls, "getDisplayParams", "()[F");
    if (mid == 0)
    {
        JNILOG("error obtaining the method id");
        return ;
    }
    jfloatArray  displayParamsArray=(jfloatArray) env->CallObjectMethod(app->activity->clazz,mid);
    // jsize length=env->GetArrayLength(displayParamsArray);
    jfloat  *params=env->GetFloatArrayElements(displayParamsArray,0);
    /* for(int i=0;i<length;i++)
     {
         IOLOG("the params are %f ",params[i]);
     }*/
    displayParams->screenWidth=params[0];
    displayParams->screenHeight=params[1];
    displayParams->density=params[2];
    displayParams->densityDpi=params[3];
    displayParams->deviceStableDensity=params[4];
    displayParams->scaledDensity=params[5];
    displayParams->xdpi=params[6];
    displayParams->ydpi=params[7];
    env->ReleaseFloatArrayElements(displayParamsArray,params,0);
    vm->DetachCurrentThread();/////dssdfsdfs
}
/////
status getPhoto(android_app* app,PixaMap *pixaMap) {
    JavaVM* vm=app->activity->vm;
    JNIEnv *env ;
    vm->AttachCurrentThread(&env,NULL);
    if(env==NULL) {
        JNILOG("coulf not attach/obtain current thread/get java environment");
        return STATUS_KO;
    }
    jclass cls = (env)->GetObjectClass(app->activity->clazz);
    if(cls==NULL)
    {
        JNILOG("coulf not get java object class");
        return STATUS_KO;
    }
    jmethodID mid = env->GetMethodID(cls, "getBitmap", "()Landroid/graphics/Bitmap;");
    if (mid == 0) {
        JNILOG("error obtaining the method id");
        return STATUS_KO;
    }
    jobject bitmap=env->CallObjectMethod(app->activity->clazz,mid);
    if(bitmap!=NULL)
    {
        JNILOG("successfully obtained the bitmap");
        AndroidBitmapInfo bitmapInfo;
        if(AndroidBitmap_getInfo(env,bitmap,&bitmapInfo)<0)
        {
            JNILOG("coulnd not obtain bitmap info");
            return STATUS_KO;
        }
        if(bitmapInfo.format!=ANDROID_BITMAP_FORMAT_RGBA_8888)
        {
            JNILOG("THE BITMAP FORMATNOT NOT RGBA 8888");
            return STATUS_KO;
        }


        /*pixaMap=(PixaMap *)malloc(sizeof(PixaMap));
         if(pixaMap==NULL)
         {
             JNILOG("could not allocate memory for newImage sorry");
             return STATUS_KO;
         }*/
        pixaMap->width=bitmapInfo.width;
        pixaMap->height=bitmapInfo.height;
        pixaMap->stride=bitmapInfo.stride;
        if(AndroidBitmap_lockPixels(env,bitmap,(void **)&pixaMap->pixels)<0)
        {
            JNILOG("the bitmap could not be locked");
            return STATUS_KO;
        }
    }

    ///////////////////dont forget to unlock bitmap
    AndroidBitmap_unlockPixels(env,bitmap);
    env->DeleteLocalRef(bitmap);
    vm->DetachCurrentThread();
    return STATUS_OK;
}
status hideSystemUI(android_app *app)
{
    JavaVM* vm=app->activity->vm;
    JNIEnv *env ;
    vm->AttachCurrentThread(&env,NULL);
    if(env==NULL) {
        JNILOG("coulf not attach/obtain current thread/get java environment");
        return STATUS_KO;
    }
    jclass cls = (env)->GetObjectClass(app->activity->clazz);
    if(cls==NULL)
    {
        JNILOG("coulf not get java object class");
        return STATUS_KO;
    }
    jmethodID mid = env->GetMethodID(cls, "hideSystemUI", "()V");
    if (mid == 0) {
        JNILOG("error obtaining the method id");
        return STATUS_KO;
    }
    env->CallVoidMethod(app->activity->clazz,mid);
    return STATUS_OK;
}
status changeVolume(android_app *app,bool increase)
{
    JavaVM* vm=app->activity->vm;
    JNIEnv *env ;
    vm->AttachCurrentThread(&env,NULL);
    if(env==NULL) {
        JNILOG("coulf not attach/obtain current thread/get java environment");
        return STATUS_KO;
    }
    jclass cls = (env)->GetObjectClass(app->activity->clazz);
    if(cls==NULL)
    {
        JNILOG("coulf not get java object class");
        return STATUS_KO;
    }

    jmethodID mid = env->GetMethodID(cls, "changeVolume", "(Z)V");
    if (mid == 0) {
        JNILOG("error obtaining the method id");
        return STATUS_KO;
    }
    jboolean inc;
    if(increase)
    {
        inc=true;
    }
    else
        {inc=false;}
    env->CallVoidMethod(app->activity->clazz,mid,inc);
    return STATUS_OK;

}


