//
// Created by pspr1 on 7/29/2020.
//
#include "glSetup.h"
#include "shapes.h"
#include "piano.h"
extern uint screenWidth,screenHeight;
void drawLine(GLuint programId,float angel)
{
    GLfloat centreLineVertices[]={-1,0,1,0};
    glUniform1f(1,0);
    glUniform4f(2,0,0,0,1);
    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,0,(void *)centreLineVertices);
    glEnableVertexAttribArray(0);
    glLineWidth(10);
    glDrawArrays(GL_LINES, 0, 2); // Starting from vertex 0; 3 vertices total -> 1 triangle
    glDisableVertexAttribArray(0);




}
void Triangle::onDraw(GLuint programId, float angle)
{
    float radian;
    GLuint vertexHandle,colorHandle,uniformResult;
    vertexHandle=glGetAttribLocation(programId,"VertexPosition");
    uniformResult=glGetUniformLocation(programId,"RadianAngle");//check result grater than equal to one0 ot(!-1);faster uniforms using cachin
    radian=60;//convert to radian
    glUniform1f(uniformResult,radian);
    uniformResult=glGetUniformLocation(programId,"color");
    glUniform4f(2,r,g,b,a);
    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,0,(void*)vertices);
    glEnableVertexAttribArray(0);
    glUseProgram(programId);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 3,20000); // Starting from vertex 0; 3 vertices total -> 1 triangle
    glDisableVertexAttribArray(0);

}
void drawRect(GLuint programId)
{
    float vertices[8];
    vertices[0]=-1;
    vertices[1]=0.2;
    vertices[2]=1;
    vertices[3]=0.2;
    vertices[4]=1;
    vertices[5]=1;
    vertices[6]=-1;
    vertices[7]=1;
    GLushort keyIndices[]={0,1,2,2,3,0};
    float radian=0;//convert to radian
    glUniform4f(2,0.8,0.1,0.1,1);
    glUniform4f(2,(float)screenWidth,(float)screenHeight,0,0);
    glUniform4f(3, 1,0,0,1);
    glUniform1i(5,2);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,0,(void*)vertices);
    glUseProgram(programId);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, keyIndices);
    glDisableVertexAttribArray(0);
}
