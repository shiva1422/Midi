#include <sys/types.h>
#include <GLES3/gl31.h>
#include <stdlib.h>
#include "ui.h"
#include "commons.h"

//
// Created by pspr1 on 8/10/2020.
//
extern DisplayParams displayParams;
void Dimensions::setDimensions(float xStart, float yStart, float width, float height)
{
    startX = xStart;
    startY = yStart;
    this->width=width;
    this->height=height;

    ///X's
 //   if(startX<=screenWidth/2)
        vertices[0] =-1.0+ startX * 2.0 /(float)displayParams.screenWidth;//*2 so that-1 to 1 else we get 0 to 1 after conversion
        vertices[2] =-1.0+(startX+width)*2.0/(float)displayParams.screenWidth;
        vertices[4] =vertices[2];
        vertices[6] =vertices[0];
///Y's
    vertices[1]=1.0-(startY+height)*2.0/(float)displayParams.screenHeight;
    vertices[3]=vertices[1];
    vertices[5]=1.0-(startY)*2.0/(float)displayParams.screenHeight;
    vertices[7]=vertices[5];


}
void Dimensions::setDimensionsDeviceIndependent(float xStart, float yStart, float width, float height)
{
    //UILOG("xpid %f and ydpi %f",displayParams.xdpi,displayParams.ydpi);
    float newWidth=width*displayParams.xdpi/displayParams.densityDpi;
    float newHeight=height*displayParams.ydpi/displayParams.densityDpi;
    setDimensions(xStart,yStart,newWidth,newHeight);

}
void Dimensions::printVertices()
{
      for(int i=0;i<8;i++)
       UILOG("the vertex %d is %f",i,vertices[i]);
}
