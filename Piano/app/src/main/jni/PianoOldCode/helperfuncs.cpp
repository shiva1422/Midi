//
// Created by pspr1 on 6/22/2020.
//
#include <unistd.h>
#include "helperfuncs.h"
#include "commons.h"
#include "shaders.h"
GLuint glQuickSetup(GLContextm* pglContext)
{
    HF("GLQUick SETUP STARTED");
    GlSetup glSetup(pglContext);
    status result=glSetup.init_display(pglContext);
    if(result==STATUS_OK)
    {
        HF("initalized gles width and height are %d and %d",pglContext->width,pglContext->height);
    }

    glClearColor(1,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);
    eglSwapBuffers(pglContext->display,pglContext->surface);
    return 1;


}

