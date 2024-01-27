//
// Created by shivaaz on 11/12/22.
//

#include<jni.h>
#include <memory>
#include<android_native_app_glue.h>
#include "PianoApplication.h"

void android_main(struct android_app* app)
{
    std::shared_ptr<KSApplication> application = std::make_shared<PianoApplication>(app);
    application->run();
}