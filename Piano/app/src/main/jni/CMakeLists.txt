cmake_minimum_required(VERSION 3.18.1)

project(PianoApp)

##KALASOFT
set(KS_DIR /Users/shivaaz/projects/kalasoft)
set(KSAPP_DIR /Users/shivaaz/projects/kalasoft/KSApplication)

set(FREETYPE /Users/shivaaz/Projects/kalasoft/external/ft/mac/ft)



include_directories(${FREETYPE}/include)

#todo move graphics ass well to kalasoft
include_directories(${KS_DIR} ${KSAPP_DIR} ${KS_DIR}/Core ${KSAPP_DIR}/Graphics ${KSAPP_DIR}/KSLogger)



add_subdirectory(${KSAPP_DIR}/KSApp ./ksapp)


#ksmain Application
set(ANDROID_SRCS
        AMain.cpp
        ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c
        PianoApplication.cpp
        PianoController.cpp)

add_library(ksmain SHARED ${ANDROID_SRCS})

add_library(pianoui STATIC UI/RainbowPianoView.cpp UI/RainbowPianoTouchListener.cpp)

#This project deps
add_subdirectory(${CMAKE_SOURCE_DIR}/Audio ./pianoaudiolib)

add_subdirectory(Piano ./kspiano)

add_subdirectory(${KS_DIR}/Media/KSImage ./ksimage)

#freetype for text/font rendering
#freetype font
add_subdirectory(./Text ./kstextlib)





target_link_libraries(ksmain ksapp  music pianoaudiolib  pianoui kspiano android  kstextlib ksimage)

link_directories(ksmain PUBLIC ${FT_DIR}/CMAKE_ANDROID_ARCH_ABI)









##Old  make
#[[
cmake_minimum_required(VERSION 3.10.0)
add_library(glue STATIC ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)
target_include_directories(glue PUBLIC ${ANDROID_NDK}/sources/android/native_app_glue)
include_directories(${ANDROID_NDK}/sources/android/native_app_glue)
find_library(log-lib log)
set(CMAKE_SHARED_LINKER_FLAGS  "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")
set(OBOE E:/androidlibs/oboe)
add_subdirectory(${OBOE} ./oboe)
#firebase
set(FIREBASE_CPP_SDK_DIR E:/androidlibs/firebasecppsdk)
add_subdirectory(${FIREBASE_CPP_SDK_DIR} bin/ EXCLUDE_FROM_ALL)
set(firebase_libs firebase_analytics firebase_admob firebase_app)
include_directories(${FIREBASE_CPP_SDK_DIR}/include ${FIREBASE_CPP_SDK_DIR}/libs/android/${CMAKE_ANDROID_ARCH_ABI}/C++)
add_library(firebase STATIC firebase.cpp firebase.h)
#

include_directories(${OBOE}/include)
set(FREETYPE E:/androidlibs/freetype)
include_directories(${FREETYPE}/include)
add_library(mainseq SHARED mainSequencer.cpp main.h)
add_library(ui STATIC ui.cpp ui.h)
#add_library(inout STATIC inputoutput.cpp inputoutput.h )
add_library(helpfun STATIC helperfuncs.cpp helperfuncs.h)
add_library(glsetup STATIC glSetup.cpp glSetup.h)
add_library(shapes STATIC shapes.cpp shapes.h)
add_library(keyboard STATIC piano.cpp piano.h)
add_library(audiomanager STATIC audiomanager.cpp audiomanager.h)
add_library(files STATIC filemanager.cpp filemanager.h)
add_library(javacalls STATIC javacalls.cpp javacalls.h)
add_library(text STATIC text.cpp text.h)
add_library(ft2 STATIC IMPORTED)
set_target_properties(ft2 PROPERTIES IMPORTED_LOCATION ${FREETYPE}/${CMAKE_ANDROID_ARCH_ABI}/libft.a)
target_link_libraries(mainseq android keyboard audiomanager oboe glue text ft2 ui javacalls jnigraphics files helpfun glsetup shapes firebase "${firebase_libs}"  GLESv3 EGL ${log-lib})
]]
