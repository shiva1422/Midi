//
// Created by pspr1 on 6/22/2020.
//
#include <assert.h>
#include "glSetup.h"
#include "commons.h"
GlSetup::GlSetup(GLContextm* pcontext){
    context=pcontext;
}
status GlSetup::init_display(GLContextm* pcontext){
    GE("initializeing opengl");
    const EGLint attribs[]={ EGL_RENDERABLE_TYPE,EGL_OPENGL_ES3_BIT,EGL_BLUE_SIZE, 8,EGL_GREEN_SIZE, 8,EGL_RED_SIZE, 8, EGL_SURFACE_TYPE, EGL_WINDOW_BIT,EGL_NONE};
    const EGLint context_attribs[]={EGL_CONTEXT_CLIENT_VERSION,3,EGL_NONE};
    EGLint w, h, format;
    EGLint numConfigs;
    EGLConfig config = nullptr;
    EGLSurface surface;
    EGLContext context;
    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if(display==EGL_NO_DISPLAY) {
        GE("no display");
        return STATUS_KO;
    }
    if( !eglInitialize(display, nullptr, nullptr))//can pass variable to get the result opengl versions
    {        GE("egl initialize failed");

        return STATUS_KO;
    }
    if(!eglChooseConfig(display, attribs, &config,1, &numConfigs)||numConfigs<=0){
        GE("eglChooseConfig failed ");
        return STATUS_KO;

    }
    //   std::unique_ptr<EGLConfig[]> supportedConfigs(new EGLConfig[numConfigs]);
    //  assert(supportedConfigs);
    // eglChooseConfig(display, attribs, supportedConfigs.get(), numConfigs, &numConfigs);
    //  assert(numConfigs);
    // auto i = 0;
    GE("numConfigs %d",numConfigs);
    /*  for (; i < numConfigs; i++) {
           auto& cfg = supportedConfigs[i];
           EGLint r, g, b, d,a;
           if (eglGetConfigAttrib(display, cfg, EGL_RED_SIZE, &r)   &&
               eglGetConfigAttrib(display, cfg, EGL_GREEN_SIZE, &g) &&
               eglGetConfigAttrib(display, cfg, EGL_BLUE_SIZE, &b)  &&
               eglGetConfigAttrib(display, cfg, EGL_DEPTH_SIZE, &d) &&
               r == 8 && g == 8 && b == 8) {//&&d=0 removed

               config = supportedConfigs[i];
               break;
           }
       }*/
    //  if (i == numConfigs) {
    //   GE("no configuration match");
    //   config = supportedConfigs[0];
    // }


    if (config == nullptr) {
        GE("Unable to initialize EGLConfig");
        return STATUS_KO;
    }
    if(! eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format)){
        GE("eglgetConfig attrib failed");
        return STATUS_KO;
    }
    ANativeWindow_setBuffersGeometry(pcontext->app->window,0,0,format);
    surface = eglCreateWindowSurface(display, config, pcontext->app->window, nullptr);
    if(surface==EGL_NO_SURFACE)
    {
        GE("window surface creation failed");
        return STATUS_KO;
    }
    context = eglCreateContext(display, config, nullptr, context_attribs);//context attribs mandatory for creating required context
    if(context==EGL_NO_CONTEXT){
        GE("context creation failed");
        return STATUS_KO;
    }
    if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
        GE("Unable to eglMakeCurrent");
        return STATUS_KO;
    }
    eglQuerySurface(display, surface, EGL_WIDTH, &w);
    eglQuerySurface(display, surface, EGL_HEIGHT, &h);
    pcontext->display = display;
    pcontext->context = context;
    pcontext->surface = surface;
    pcontext->width = w;
    pcontext->height = h;
    auto opengl_info = {GL_VENDOR, GL_RENDERER, GL_VERSION, GL_EXTENSIONS};
    for (auto name : opengl_info) {
        auto info = glGetString(name);
        GE("OpenGL Info: %s", info);
    }
    //  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
    glEnable(GL_CULL_FACE);
    //  glShadeModel(GL_SMOOTH);
    glDisable(GL_DEPTH_TEST);

    return STATUS_OK;

}
bool Shader::getSource(android_app *app,const char *fileName)
{
    int32_t  shaderSize=0;
    AAssetManager *assetManager=app->activity->assetManager;
    AAsset *asset=AAssetManager_open(assetManager,fileName,AASSET_MODE_UNKNOWN);
    if(asset)
    {
        shaderSize=AAsset_getLength(asset);
        this->source=(char *)malloc(shaderSize+1);
        int noOfBytesRead=AAsset_read(asset,this->source,shaderSize);
        AAsset_close(asset);
        if(shaderSize==noOfBytesRead)
        {
            this->source[shaderSize]='\0';
            GE("the shader %s size in is %d bytes is read Succesfully",fileName,shaderSize);
         //   GE("the shader Source is \n %s",this->source);
            return true;
        }
        else
            return false;
    }
    else
        return false;
}
GLuint Shader::compile(GLenum shaderType)
{
    GLuint shader=glCreateShader(shaderType);

    if(shader)
    {    GE("createing shader program");

        glShaderSource(shader,1,&source,NULL);
        glCompileShader(shader);
        //status of compilation
        GLint compiled=0;

        glGetShaderiv(shader,GL_COMPILE_STATUS,&compiled);
        if(!compiled)
        {

            GE("shader not compiled getting failure info .....");
            GLint  infolen=0;
            glGetShaderiv(shader,GL_INFO_LOG_LENGTH,&infolen);
            if(infolen)
            {
                char* buf=(char *)malloc(infolen);
                if(buf)
                {
                    glGetShaderInfoLog(shader,infolen,NULL,buf);
                    GE("the reason for failue of shader compilation %s",buf);
                    free(buf);
                }
                glDeleteShader(shader);
            }
        }
        else GE("shader Compile Succesfully");
    }
    return shader;

}
GLuint linkShader(GLuint vertexShaderId, GLuint fragmentShaderId) {
    if(!vertexShaderId|!fragmentShaderId)
    {
        return 0;//failure
    }
    GLuint program=glCreateProgram();
    if(program)
    {
        glAttachShader(program,vertexShaderId);
        glAttachShader(program,fragmentShaderId);
        glLinkProgram(program);
        GLint linkStatus=GL_FALSE;
        glGetProgramiv(program,GL_LINK_STATUS,&linkStatus);
        if(linkStatus!=GL_TRUE)
        {
            GLint buflen=0;
            glGetProgramiv(program,GL_INFO_LOG_LENGTH,&buflen);
            if(buflen)
            {
                char* buf=(char *)malloc(buflen);
                if(buf)
                {
                    glGetProgramInfoLog(program,buflen,NULL,buf);
                    GE("could not link the  program reason : %s",buf);
                    free(buf);
                }
            }
            glDeleteProgram(program);
            program=0;
        }
        else
            GE("link succesful shader progra");
    }

    return program;
}
