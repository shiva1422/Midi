//
// Created by pspr1 on 6/22/2020.
//

#ifndef DIGIBAND_GLSETUP_H
#define DIGIBAND_GLSETUP_H
#include "memory"
#include <android/log.h>
#include "android_native_app_glue.h"
#include "EGL/egl.h"
#include "commons.h"
#if __ANDROID_API__ >= 24
#include <GLES3/gl32.h>
#elif __ANDROID_API__ >= 21
#include <GLES3/gl31.h>
#else
#include <GLES3/gl3.h>
#endif
#define TAGGL "GLES SETUP"
#define GE(...)((void)__android_log_print(ANDROID_LOG_ERROR,TAGGL,__VA_ARGS__))
#define GI(...)((void)__android_log_print(ANDROID_LOG_INFO,TAGGL,__VA_ARGS__))
class Shader{
public:
    char *source=NULL;
    GLuint shaderId;
    void deleteSource(){free(source);source=NULL;}
    GLuint loadAndCompile(android_app *app,const char *fileName,GLenum shaderType)
    {
        getSource(app,fileName);
        shaderId= compile(shaderType);
        return shaderId;
    }

    GLuint compile(GLenum shaderType);
    bool getSource(android_app *app, const char *fileName);
};
class GlSetup{
    GLContextm* context;
public:
    GlSetup(GLContextm* pcontextm);
    static status init_display(GLContextm* pcontext);
};
GLuint linkShader(GLuint vertexShaderId,GLuint fragmentShaderId);


#endif //DIGIBAND_GLSETUP_H
