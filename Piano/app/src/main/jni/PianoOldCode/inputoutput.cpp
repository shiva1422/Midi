
#include <unistd.h>
#include "inputoutput.h"
#include "glSetup.h"
#include "helperfuncs.h"
#include "piano.h"
#include "ui.h"
#include "main.h"
#include "javacalls.h"

//
// Created by pspr1 on 6/22/2020.
//
extern DisplayParams displayParams;
uint screenWidth,screenHeight;
android_app* papp;
GLContextm contextApp;
////keyboard
int keyOn[TOTALKEYSANDPADS];//0-KEYOFF,1-KEYON;
PianoSynthNode piano;
/// /touch
float touchX,touchY,moveX,moveY;
int pointerIndex,pointerCount,pointerId;//use int max for non usable value
bool setFullScreenNeeded=false;
////DIMENSIONS
  int32_t onInput(android_app* papp,AInputEvent* event) {
    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION)
      {
          pointerIndex =(AMotionEvent_getAction(event) & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK)>> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
          pointerId=AMotionEvent_getPointerId(event,pointerIndex);
          //   IO("the pointer index for all is %d",pointerIndex);
          touchX = AMotionEvent_getX(event, pointerIndex);
          touchY = AMotionEvent_getY(event, pointerIndex);
          switch (AMotionEvent_getAction(event) & AMOTION_EVENT_ACTION_MASK) {
              case AMOTION_EVENT_ACTION_DOWN:
                  {

                      if(touchY>=piano.whiteKey.startY)
                      {
                          piano.onAction(pointerId,touchX,touchY,ACTION_DOWN);
                      }
                      else
                      {
                          piano.onNonKeyAction(pointerId,touchX,touchY,ACTION_DOWN);
                      }
                  }
                  break;
              case AMOTION_EVENT_ACTION_POINTER_DOWN: {
                  if(touchY>=piano.whiteKey.startY)
                  {
                      piano.onAction(pointerId,touchX,touchY,ACTION_DOWN);
                  }
                  else
                  {
                      piano.onNonKeyAction(pointerId,touchX,touchY,ACTION_DOWN);
                  }

              }
                  break;
              case AMOTION_EVENT_ACTION_MOVE:
                  {
                  pointerCount=AMotionEvent_getPointerCount(event);
                  for(int i=0;i<pointerCount;i++)//calling action down for all pointers call only if historyValue and current value differ for particular index
                  {
                      touchX = AMotionEvent_getX(event, i);
                      touchY = AMotionEvent_getY(event, i);
                      pointerId = AMotionEvent_getPointerId(event, i);
                      if (pointerId != piano.positionChangerPointerId)///non-slider(piano)////////////
                      {
                          if (touchY >=piano.whiteKey.startY)
                          {
                              piano.onAction(pointerId,touchX,touchY,ACTION_MOVE);

                          }
                          else{
                          if (piano.previousPointedKey[pointerId] !=52)//move out of piano so previous key SHould be off
                          {
                              piano.onAction(pointerId,touchX,touchY,ACTION_MOVEOUT);
                          }
                          }
                      }
                      else ///monitoring pointer of some slider motion not piano
                      {
                         // onNonPianoAction(papp,event,ACTION_MOVE);
                         piano.onNonKeyAction(pointerId,touchX,touchY,ACTION_MOVE);

                      }


                  }
              }
                  break;
              case AMOTION_EVENT_ACTION_POINTER_UP: {
                  //  IO("ACTION UP POINTER");
                  //IO("the pointer index for pointerUp i %d",pointerId);
                  if(pointerId!=piano.positionChangerPointerId)//non slider(piano)
                  {
                     // onPianoAction(papp, event, ACTION_UP);
                     piano.onAction(pointerId,touchX,touchY,ACTION_UP);
                  }
                  else
                  {
                      piano.positionSliderActive=false;
                      piano.positionChangerPointerId=INT_MAX;//just as int max in not possible pointer id(non usable value for convenience)
                  }
                  ////rem0ve this no need


              }
                  break;
              case AMOTION_EVENT_ACTION_UP: {
                  //  IO("ACTION UP");
               //   IO("the pointer index for actionUp i %d",pointerId);
                  if(pointerId!=piano.positionChangerPointerId)//non slider(piano)
                  {
                      piano.onAction(pointerId,touchX,touchY,ACTION_UP);
                      //onPianoAction(papp, event, ACTION_UP);
                  }
                  else
                  {
                      piano.positionSliderActive=false;
                      piano.positionChangerPointerId=INT_MAX;//just as int max in not possible pointer id(non usable value for convenience)
                  }
                  ///any one of the keys still remains on after all fingers (speed) so check the mistake or off all on action up as below in on should be done in piaano key action
                  for(int i=0;i<89;i++)
                  {
                           keyOn[i]=0;
                          glUniform1i(20 + i,0);
                  }



              }
                  break;


          }
          return 1;
      }
    else if((AInputEvent_getType(event) == AINPUT_EVENT_TYPE_KEY))
    {
        int32_t keyCode=AKeyEvent_getKeyCode(event);
        IO("The key code is %d",keyCode);
        switch (AKeyEvent_getAction(event))
        {
            case AKEY_EVENT_ACTION_DOWN:
            {
                //IO("keyDown");
                if(keyCode==AKEYCODE_VOLUME_UP)
                {  IO("volume up");
                    changeVolume(papp, true);
                }
                else if(keyCode==AKEYCODE_VOLUME_DOWN)
                    changeVolume(papp,false);
            }
            break;
            case AKEY_EVENT_ACTION_UP:
            {
             //   IO("keyUP");
            }
            break;


        }
        IO("keyEvent");


    }
      return 1;
  }
////


void onCmd(android_app* app, int32_t cmd)
{

    //  IO("command handling");
    switch(cmd)
    {
        case APP_CMD_SAVE_STATE:
            // the OS asked us to save the state of the app
            break;
        case APP_CMD_INIT_WINDOW:
            // get the window ready for showing
        { hideSystemUI(app);
            if(!contextApp.glInitStatus)
            {
                Shader vertexShader,fragmentShader;
                glQuickSetup(&contextApp);
                vertexShader.loadAndCompile(app,piano.vertexShader,GL_VERTEX_SHADER);
                fragmentShader.loadAndCompile(app,piano.fragmentShader,GL_FRAGMENT_SHADER);
                piano.programId=linkShader(vertexShader.shaderId,fragmentShader.shaderId);
                screenWidth=contextApp.width;
                screenHeight=contextApp.height;
                glUseProgram(piano.programId);
                if(contextApp.appFirstOpen)
                initializeParams();
                piano.setupTextures();////////////sdfsdfsdfsadfasdfadsf
            }

           // computeShaderSetup(&contextApp);

          contextApp.windowInitStatus= true;
            glUseProgram(piano.programId);
            contextApp.appFirstOpen=false;
            contextApp.glInitStatus= true;
        }
            break;
        case APP_CMD_TERM_WINDOW: {
            // clean up the window because it is being hidden/closed
            contextApp.glInitStatus = false;
            contextApp.windowInitStatus= false;

        }
            break;
        case APP_CMD_LOST_FOCUS:
            // if the app lost focus, avoid unnecessary processing
            //(like monitoring the accelerometer)
            break;
        case APP_CMD_GAINED_FOCUS:
            // bring back a certain functionality, like monitoring the accelerometer
            break;
    }
}
