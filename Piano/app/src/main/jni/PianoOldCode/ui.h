//
// Created by pspr1 on 8/10/2020.
//

#ifndef DIGIBAND_UI_H
#define DIGIBAND_UI_H
#include"android/log.h"
#define TAGUI "UI"
//#define LOGE(...)((void)__android_log_print(ANDROID_LOG_INFO,TAG,__VA_ARGS__));
#define UILOG(...)((void)__android_log_print(ANDROID_LOG_ERROR,TAGUI,__VA_ARGS__))
class Dimensions{
public:
    float startX,startY,width,height;
    float vertices[8];
    void printVertices();
    float endX(){return (startX+width);}
    float endY(){return (startY+height);}
    void setDimensions(float xStart,float yStart,float width,float height);
    void setDimensionsDeviceIndependent(float xStart,float yStart,float width,float height);
};
void drawUI();
#endif //DIGIBAND_UI_H
