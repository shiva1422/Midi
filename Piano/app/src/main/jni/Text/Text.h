//
// Created by pspr1 on 10/24/2020.
//

#ifndef DIGIBAND_TEXT_H
#define DIGIBAND_TEXT_H

#include <Display.h>
#include <string>
#include "ft2build.h"
#include "freetype/freetype.h"
#include "freetype/ftglyph.h"
#include "android/log.h"
#include "Commons.h"
//TODO REfactor whole thing also include Kerning
//TODO use common bitmap structs
typedef int status;
struct PixaMap{
    uint8_t *pixels = nullptr;
    int width,height;
};
void textSetup();
void stringToPixmap(const char* string, PixaMap *stringMap);
class TextEngine{

public:

    status init();

    //WIDTH AND HEIGHT IN POINTS(1/72 th of inch)
    status setTextSize(int  width,int height);

    status setFont(const char* fontFile);

    void getPixamapFromString(const std::string pString,PixaMap *pixaMap);

    void setDisplayMetrics(const DisplayMetrics &displayMetrics){displayParams = displayMetrics;}

private:

     FT_Library ftLibrary;//handle to the instance of Library//make static if need only one for entire app.

     FT_Face fontFace;

     int fontHeight=20,fontWidth=20;

     const char *defaultFont="/system/fonts/DroidSans.ttf";

     DisplayMetrics displayParams;


};

#endif //DIGIBAND_TEXT_H
