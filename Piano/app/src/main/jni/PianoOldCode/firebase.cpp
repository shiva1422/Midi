//
// Created by pspr1 on 11/9/2020.
//

#include "firebase.h"
#include "commons.h"

extern firebase::admob::BannerView* banner_view;
extern firebase::admob::AdRequest my_ad_request;
extern DisplayParams displayParams;
extern bool load,show;
void setupFireBaseAndLoadBanner(android_app *app)
{
    JNIEnv* env;
    app->activity->vm->AttachCurrentThread(&env,NULL);
    firebase::App* fapp =firebase::App::Create(firebase::AppOptions(),env,app->activity->clazz);
    if(fapp)
    {
        FirebaseLog("successfully created fapp firebase app");
    }
    const char* kAdMobAppID = "ca-app-pub-1832865342460243~6631894368";

    firebase::admob::Initialize(*fapp, kAdMobAppID);

    const char* kBannerAdUnit = "ca-app-pub-1832865342460243/7270845157";//"ca-app-pub-3940256099942544/6300978111";test unit
    banner_view=new firebase::admob::BannerView();

    firebase::admob::AdSize ad_size;
    ad_size.ad_size_type = firebase::admob::kAdSizeStandard;
    ad_size.width = 320;
    ad_size.height = 50;
    banner_view->Initialize(static_cast<firebase::admob::AdParent>(app->activity->clazz), kBannerAdUnit, ad_size);



    my_ad_request.gender = firebase::admob::kGenderUnknown;
    // The user's birthday, if known. Note that months are indexed from one.
    my_ad_request.birthday_day = 10;
    my_ad_request.birthday_month = 11;
    my_ad_request.birthday_year = 1996;
    // Additional keywords to be used in targeting.
    static const char* kKeywords[] = {"music", "keyBoard", "PianoSynth","drums","synthesizer","sequencer"};
    my_ad_request.keyword_count = sizeof(kKeywords) / sizeof(kKeywords[0]);
    my_ad_request.keywords = kKeywords;
// "Extra" key value pairs can be added to the request as well.
    static const firebase::admob::KeyValuePair kRequestExtras[] = {
            {"photo", "editor"}};
    my_ad_request.extras_count = sizeof(kRequestExtras) / sizeof(kRequestExtras[0]);
    my_ad_request.extras = kRequestExtras;
    static const char* kTestDeviceIDs[] ={"DC014E2A6601FD3AAF71861FE222D9B1"};// {"DC014E2A6601FD3AAF71861FE222D9B1","098fe087d987c9a878965454a65654d7"};
    my_ad_request.test_device_id_count =
            sizeof(kTestDeviceIDs) / sizeof(kTestDeviceIDs[0]);
    my_ad_request.test_device_ids = kTestDeviceIDs;

    if (banner_view->InitializeLastResult().status() ==
        firebase::kFutureStatusComplete &&
        banner_view->InitializeLastResult().error() ==
        firebase::admob::kAdMobErrorNone) {
        //  banner_view->LoadAd(my_ad_request);
        FirebaseLog("breakpoint");
        banner_view->Show();

    }
    //  MAINLOG("breakpoint");

    // banner_view->MoveTo(0,0);


    //banner_view->Show();
    // MAINLOG("breakpoint");

    //if (banner_view->LoadAdLastResult().status() ==firebase::kFutureStatusComplete)// &&banner_view->LoadAdLastResult().error() ==firebase::admob::kAdMobErrorNone)
    { // MAINLOG("breakpoint");

        //banner_view->Show();
    }
    app->activity->vm->DetachCurrentThread();
}

void checkBanner()
{
    if(!load)
        if (banner_view->InitializeLastResult().status() ==firebase::kFutureStatusComplete &&banner_view->InitializeLastResult().error() ==firebase::admob::kAdMobErrorNone)
        {
            //  banner_view->LoadAd(my_ad_request);
            FirebaseLog("breakpoint");
            banner_view->LoadAd(my_ad_request);
            load=true;
        }
    if(load)
        if (banner_view->LoadAdLastResult().status() ==firebase::kFutureStatusComplete &&banner_view->LoadAdLastResult().error() ==firebase::admob::kAdMobErrorNone)
        {
            banner_view->LoadAd(my_ad_request);
            FirebaseLog("breakpoint");
            //banner_view->MoveTo(banner_view->kPositionTopRight);
            banner_view->Show();
            firebase::admob::BoundingBox bounds=banner_view->bounding_box();
            int startx=(displayParams.screenWidth-bounds.width)/2;
            FirebaseLog("adwidth is %d",bounds.width);
            banner_view->MoveTo(startx,0);
            show=true;

        }

}

