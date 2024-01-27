//
// Created by pspr1 on 9/6/2020.
#include "filemanager.h"
#include "piano.h"

void getAudioFormatFromFile(android_app *app,AudioFormatKala *format)
{
    AAssetManager *assetManager=app->activity->assetManager;
    AAsset *asset=AAssetManager_open(assetManager,"sounds1/58.wav",AASSET_MODE_UNKNOWN);
    if(asset!=NULL)
    {
        ///////WAVE DECODER
        char decode[44];
        int32_t noOfBytesRead=AAsset_read(asset,decode,44);
        format->audioFormat=*((short *)(decode+20));
        FilesLog("the audio format is %d",format->audioFormat);
        format->numChannels=*((short *)(decode+22));
        FilesLog("numChannels = %d",format->numChannels);
        format->sampleRate=*(uint32_t *)(decode+24);
        FilesLog("sampleRate = %d",format->sampleRate);
        format->byteRate=*(uint32_t *)(decode+28);
        FilesLog("byteRate = %d ",format->byteRate);
        format->blockAlign=*((short *)(decode+32));
        FilesLog("blockAliggn = %d ",format->blockAlign);
        format->bitsPerSample=*((short *)(decode+34));
        FilesLog("BitsPerSample = %d ",format->bitsPerSample);
        //close after use
        AAsset_close(asset);

    }

}
void loadSounds(android_app *app,int16_t ** audio,uint32_t *totLen)
{
    AAssetManager *assetManager = app->activity->assetManager;
    for (int i = 0; i < 88; i++)//88sounds
    {
        int fileNameLength;
        if( i<10)
            fileNameLength=14;
        else
            fileNameLength=15;
        char fileName[fileNameLength];
        strcpy(fileName,"sounds1/");
        int index;
        index=8;
        if(i<10)
        {

            fileName[index]=i+'0';
            index++;

        }
        else
        {
            fileName[index]=(i/10)+'0';
            index++;
            fileName[index]=(i%10) + '0';
            index++;
        }
        fileName[index]='.';
        index++;
        fileName[index]='w';
        index++;
        fileName[index]='a';
        index++;
        fileName[index]='v';
        index++;
        fileName[index]='\0';
        FilesLog("The filename is %s",fileName);
       AAsset *asset = AAssetManager_open(assetManager,fileName, AASSET_MODE_UNKNOWN);
        if (asset != NULL)
        {
        ///////WAVE DECODER
        char decode[44];
        int32_t noOfBytesRead = AAsset_read(asset, decode, 44);
        int32_t dataSize = *(int32_t *) (decode + 40);
        totLen[i] = dataSize;
        audio[i] =(int16_t *) malloc(dataSize);///////////////if memory error show error
        noOfBytesRead = AAsset_read(asset,audio[i], dataSize);
        if (noOfBytesRead == dataSize) {
            FilesLog("successfully imported whole audiofile %d",i);
        }
        //close after use
        AAsset_close(asset);
        }
        else
            FilesLog("the asset could not be opend");


}

}