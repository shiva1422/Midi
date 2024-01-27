//
// Created by pspr1 on 6/22/2020.
//

#ifndef DIGIBAND_SHADERS_H
#define DIGIBAND_SHADERS_H
//each individual type variable occupies a single location for array of size n occupies n locations continuolsy
//iskeyon[97]=>52 white,37 black(1st key g# offset 52 is ignored so the black key actually starts at 53 for iskeyon )and 8 pads;
const char *vertexShader="#version 310 es \n"
                       //  "precision highp float;"
                      //   "layout(location = 1)uniform float RadianAngle;\n"//change this
                         "layout(location = 0) in vec2 whiteVertices;\n"
                         "layout(location = 1) in vec2 blackVertices;\n"
                         "layout(location = 2) in vec2 backVertices;\n"
                        // "layout(location = 3) in vec2 padVertices;\n"
                         "layout(location = 4) in vec2 keySizeAdjusters;\n"
                         "layout(location = 5) in vec2 keyPositionAdjuster;\n"
                         "layout(location =6) in vec2 textCoods;\n`"
                         // "layout(location = 4) uniform vec4 keyOnColors[3];\n"
                         "layout(location = 5) uniform float displayParams[5];\n "//includes screeH,w,whitekeyW,keyGap,padH
                         "layout(location = 10) uniform float keyTranslationsX[5];"//blackKeyTranslations
                        "layout(location = 20) uniform int isKeyOn[97];"//for pads and keys on-1 off-0
                         "layout(location = 120) uniform int drawType;"
                         "layout(location = 122) uniform float keyAdjusterTranslations[2];"//sizeIncTrans,postionsTrans
                         "out vec4 colorOut;\n"
                         "vec4 color;\n"
                         "float finalPositionX,octaveNum;\n"
                         "vec3 finalVerts;\n"
                         "vec3 finalColor;\n"
                         "int index;"
                       //  "mat2 rotation;\n"
                         "void main()\n{"
                         "index=gl_InstanceID;\n"
                         "finalVerts.z=0.0;"
                         "switch(drawType){"
                            "case 0:{"//drawing whitekeys blackkeys back pads in this order///case 0
                              "if(index<52){"//whitekeys
                                "finalVerts.x=whiteVertices.x+(float(index)*(displayParams[2]+displayParams[3])*2.0)/displayParams[0];\n"
                                "finalVerts.y=whiteVertices.y;\n"
                                "if(isKeyOn[index]==1){"
                                " color.x=0.5,color.y=0.0,color.z=0.8,color.w=1.0;}\n"
                              "else{"
                                "color.x=1.0,color.y=1.0,color.z=1.0,color.w=1.0;}"
                                "}"
                              "else if(index<88&&index>=52){"//blackKeys start at 52 instance id
                               "finalVerts.x=blackVertices.x+(( float((index-52)/5)*7.0*(displayParams[2]+displayParams[3])+keyTranslationsX[(index-52)%5])*2.0)/displayParams[0];\n"
                               "finalVerts.y=blackVertices.y;\n"
                               "if(isKeyOn[index+1]==1){"//cuz the starting key g#(52) is ignored //so 53 is start for indices wher 52 was for draing instance
                                 "color.x=1.0,color.y=1.0,color.z=0.0,color.w=1.0;}"
                               "else{"
                                "color.x=0.0,color.y=0.0,color.z=0.0,color.w=1.0;}"
                                "}"
                             "else if(index==88){"//backGoround abovekeys
                              "finalVerts.x=backVertices.x;\n"
                              "finalVerts.y=backVertices.y;\n"
                              "color.x=1.0,color.y=0.2,color.z=0.2,color.w=1.0;"
                              "}"
                         /*  "else if(index<97){"//pads start at 89 instanceID and end at 96
                             "finalVerts.x=padVertices.x-(float(index-89)*(displayParams[4]+displayParams[3]*2.0)*2.0)/displayParams[0];\n"
                             "finalVerts.y=padVertices.y;\n"
                            "color.x=1.0,color.y=0.8,color.z=0.8,color.w=1.0;\n"
                            "}"*/
                             "else{"//keySize and Position Modifieres
                               "if(index==89){"
                                "finalVerts.x=keySizeAdjusters.x;"
                                "finalVerts.y=keySizeAdjusters.y;"
                                "color.x=0.5,color.y=0.5,color.z=0.5,color.w=0.5;}\n"
                               "else if(index==90){"
                                "finalVerts.x=keySizeAdjusters.x+(keyAdjusterTranslations[0]*2.0)/displayParams[0];"
                                "finalVerts.y=keySizeAdjusters.y;"
                                "color.x=0.5,color.y=0.5,color.z=0.5,color.w=0.5;}\n"
                              "else if(index==91){"
                                "finalVerts.x= keyPositionAdjuster.x+(( keyAdjusterTranslations[1]*2.0))/displayParams[0];\n"
                                "finalVerts.y=keyPositionAdjuster.y;\n"
                                "color.x=0.0,color.y=0.0,color.z=1.0,color.w=1.0;\n"
                                "}"
                             "}"
                            "}break;"
                             "}"

                      //   "rotation=mat2(cos(RadianAngle),sin(RadianAngle),-sin(RadianAngle),cos(RadianAngle));\n "
                         "gl_Position=vec4(finalVerts.x,finalVerts.y,1,1);\n"
                         // "gl_PointSize=2.0;\n"
                         "colorOut=color;\n"
                         "}";
const char *fragmentShader="#version 310 es\n"
                           "precision mediump float;\n"
                           "in vec4 colorOut;\n"
                           "out vec4 Fragcolor;\n"
                           "void main(){ Fragcolor=colorOut;}";
const char *computeShader="#version 310 es\n"
                           "layout(local_size_x = 128) in; \n "
                           "layout(std430) buffer;\n"
                           "layout(binding=5) buffer BufferIn { \n"
                           "float x; \n"
                           "}inp;\n"
                          "layout(binding=6)  buffer BufferOut { \n"
                          "float y; \n"
                          "}outp;\n"
                           "void main()\n"
                           "{\n"
                           "outp.y=345345.1;\n"
                           "}\n";



#endif //DIGIBAND_SHADERS_H
