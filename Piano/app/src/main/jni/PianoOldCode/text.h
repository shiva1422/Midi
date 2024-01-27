//
// Created by pspr1 on 10/24/2020.
//

#ifndef DIGIBAND_TEXT_H
#define DIGIBAND_TEXT_H
#include "ft2build.h"
#include "freetype/freetype.h"
#include "freetype/ftglyph.h"
#include "android_native_app_glue.h"
#include "android/log.h"
#include "pixamap.h"
#include "commons.h"
#define textLog(...)((void)__android_log_print(ANDROID_LOG_ERROR,"TEXT RENDERING",__VA_ARGS__))
void textSetup();
void stringToPixmap(const char* string, PixaMap *stringMap);
class TextEngine{
private:
     FT_Library ftLibrary;//handle to the instance of Library//make static if need only one for entire app.
     FT_Face fontFace;
     int fontHeight=20,fontWidth=20;
     const char *defaultFont="/system/fonts/DroidSans.ttf";

public:
    status init();
    status setTextSize(int  width,int height);//WIDTH AND HEIGHT IN POINTS
    status setFont(const char* fontFile);
    void getPixamapFromString(const char *string,PixaMap *pixaMap);


};

#endif //DIGIBAND_TEXT_H
