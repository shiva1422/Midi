//
// Created by pspr1 on 7/29/2020.
//

#ifndef DIGIBAND_SHAPES_H
#define DIGIBAND_SHAPES_H
void drawTriangle(GLuint programId,float angle);
void drawRect(GLuint programId);
void drawLine(GLuint programId,float angel);
class Triangle{
    float vertices[6];
    float a,r,g,b;
public:
    Triangle(){vertices[0]=0,vertices[1]=0.5,vertices[2]=-0.5,vertices[3]=-0.5,vertices[4]=0.5,vertices[5]=-0.5,a=1,r=0,b=0,g=1;}
    void onDraw(GLuint programId,float angle);
};
class Keys
{
private:uint keyNo;

};
#endif //DIGIBAND_SHAPES_H
