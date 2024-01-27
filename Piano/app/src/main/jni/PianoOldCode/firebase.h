//
// Created by pspr1 on 11/9/2020.
//

#ifndef DIGIBANDPIANO_FIREBASE_H
#define DIGIBANDPIANO_FIREBASE_H

#include<android_native_app_glue.h>
#include "firebase/admob.h"
#include "firebase/admob/types.h"
#include "firebase/app.h"
#include "firebase/future.h"
#include "firebase/admob/banner_view.h"
#include "android/log.h"
#define FirebaseLog(...)((void)__android_log_print(ANDROID_LOG_ERROR,"FIREBASE:",__VA_ARGS__))
void checkBanner();
void setupFireBaseAndLoadBanner(android_app *app);

#endif //DIGIBANDPIANO_FIREBASE_H
