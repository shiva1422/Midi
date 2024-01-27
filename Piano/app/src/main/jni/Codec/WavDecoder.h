//
// Created by shivaaz on 12/26/22.
//

#ifndef KALASOFT_WAVDECODER_H
#define KALASOFT_WAVDECODER_H

#include "../Audio/AudioSamples.h"
#include "string"
#define TAGWAV "Wave Header"


//http://soundfile.sapp.org/doc/WaveFormat/

//The 44 byte header for normal pcm format
//https://sharkysoft.com/archive/lava/docs/javadocs/lava/riff/wave/doc-files/riffwave-content.html

struct WavHeader{

    char chunkId[4];//"RIFF"  = 0x46464952 //littleEnd

    uint32_t chunkSize;//size of entire file leaving(this and above) 8 bytes

    char format[4]; //"WAVE" = 0x45564157

    //The "WAVE" format consists of two subchunks: "fmt " and "data":


    /** (The "fmt " subchunk1)describes the sound data's format:*/

    char subChunk1ID[4];//"fmt " = 0x20746D66;null terminated

    uint32_t subChunk1Size;//remaining fmt subChunck Size following this

    uint16_t audioFormat;//1 = Linear quantization(pcm) else other compressions

    uint16_t channelCnt;

    uint32_t sampleRate;

    uint32_t byteRate;

    uint16_t blockAlign;//channelCnt * bitspersample/8;//numbytes per frame

    uint16_t bitsPerSample;

   // uint16_t  ExtraParamSize;//   if PCM, then doesn't exist
   //  X   ExtraParams      space for extra parameters



    /** (The "data " subchunk2)contains the sound data: //below as WavData
    char dataID[4];//"data"
    uint32_t dataSize;//total audio buffer size*/

    void printAudioFormat()
    {
        KSLOGI(TAGWAV,"channelCnt %u",channelCnt);
        KSLOGI(TAGWAV,"sampleRate %u",sampleRate);
        KSLOGI(TAGWAV,"AudioFormat(Compression) : %u",audioFormat);
        KSLOGI(TAGWAV,"bitsPerSample %u",bitsPerSample);
        KSLOGI(TAGWAV,"blockAlign %u",blockAlign);
    }

};

struct WavData{

    char id[4] ;//"data"
    uint32_t dataSize;//Audio data size;

};


class WavDecoder{

public:


    static AudioSamples* readInput(IKSStream* input)
    {
        if(!input)return nullptr;

        //read the 44 header
        WavHeader header{0};
        //TEST
        assert(readHeader(header,input));

        KSLOGD(TAGWAV,"File Type: %s\n", std::string(header.chunkId,4).c_str());
        KSLOGD(TAGWAV,"File Size: %u\n", header.chunkSize);
        KSLOGD(TAGWAV,"WAV Marker: %s\n", std::string(header.format,4).c_str());
        KSLOGD(TAGWAV,"Format Name: %s\n", std::string(header.subChunk1ID,4).c_str());
        KSLOGD(TAGWAV,"Format Length: %u\n", header.subChunk1Size );
        KSLOGD(TAGWAV,"Format Type: %hd\n", header.audioFormat);
        KSLOGD(TAGWAV,"Number of Channels: %u\n", header.channelCnt);
        KSLOGD(TAGWAV,"Sample Rate: %u\n", header.sampleRate);
        KSLOGD(TAGWAV,"Bits per Sample: %u\n", header.bitsPerSample);
       // input->read(&header,sizeof(header));

        //to seek to data chunck;
        //can contain other chunck in fmt chunk,like fact chunk check above link in header.
        input->seek(header.subChunk1Size - 16,SEEK_CUR);
        WavData data;
        input->read(&data,sizeof(data));
        std::string id(data.id,4);
        KSLOGD("WavDecoder","wave chunck ID - %s and data size - %u",id.c_str(),data.dataSize);

        return nullptr;

       /*
        if(header.bitsPerSample == 16)
            samples->sampleFmt = ESampleFormat::INT16;
        else if(header.bitsPerSample == 32)
            samples->sampleFmt = ESampleFormat::FLOAT;//TODO accurate? 0.0 - 1.0


        return samples;*/
        //TODO close input destroy;
    }

private:


   static  bool readHeader(WavHeader &head,IKSStream *stream)
    {
        stream->read(&head.chunkId,4);
        const char *c = head.chunkId;

        //TODO RIFF motorola RIFX intel RIFF byte order //daubnet.com/en/file-format-riff
        if((c[0] != 'R' || c[1] != 'I' || c[2] != 'F' ) || !(c[3] == 'F' || c[3] == 'X') )
            return false;

        KSLOGD("WavHeader","chunkId %s",std::string(c,4).c_str());

        stream->read(&head.chunkSize,4);
        KSLOGD(TAGWAV,"chunkSize %u",head.chunkSize);

        stream->read(&head.format,4);
        const char* f = head.format;
        if(f[0] != 'W' || f[1] != 'A' || f[2] != 'V' || f[3] != 'E')
            return false;

        stream->read(&head.subChunk1ID,4);
        const char* s = head.subChunk1ID;
        //if not "fmt " could be "JUNK"  skip junk to get format;
        if(s[0] != 'f')
        {
            //TODO check other chars
            if(s[0] == 'J')
            {
                KSLOGD(TAGWAV,"junk exist skipping junk");
                //Get junksize
                stream->read(&head.subChunk1Size,4);
                KSLOGD(TAGWAV,"junk size %u",head.subChunk1Size);
                stream->seek(head.subChunk1Size,SEEK_CUR);
            }

            stream->read(head.subChunk1ID,4);
            stream->read(&head.subChunk1Size,4);

        }

        //no need check first char enough?
        if(s[0] != 'f' || s[1] != 'm' || s[2] != 't')
        {
           KSLOGE(TAGWAV,"read header error fmt chunk not found");
            return false;
        }
        KSLOGD(TAGWAV,"subChunck %c %c %c %c",s[0],s[1],s[2],s[3]);

        /*

  uint16_t audioFormat;//1 = Linear quantization else other compressions

  uint16_t channelCnt;

  uint32_t sampleRate;

  uint32_t byteRate;

  uint16_t blockAlign;//channelCnt * bitspersample/8;

  uint16_t bitsPerSample;
       */
        stream->read(&head.format,2);
        stream->read(&head.channelCnt,2);
        stream->read(&head.sampleRate,4);
        stream->read(&head.byteRate,4);
        stream->read(&head.blockAlign,2);
        stream->read(&head.bitsPerSample,2);


        //TODO there may be extra params following this check headerstruct;
        //then a fact chunck for compressed pcm


        return true;
    }


public:

    //Temporory simple wav 44 byte header uncompressed pcm later TODO to cover all using above ,dont use this
    static AudioSamples* decodeSimpleAudio(IKSStream *stream)
    {
        if(stream == nullptr)
        {
           KSLOGE(TAGWAV,"Invalid Input Stream");
            return nullptr;
        }
        WavHeader header;

        char decode[44];
        int32_t noOfBytesRead = stream->read(decode,44);
        //TODO if not first 4 RIFF/X return null;
        //This should be 1 for pcm and other for compressed;
        header.audioFormat = *((short *)(decode+20));
        header.channelCnt = *((short *)(decode+22));
        header.sampleRate = *(uint32_t *)(decode+24);
        header.byteRate = *(uint32_t *)(decode+28);
        header.blockAlign = *((short *)(decode+32));
        header.bitsPerSample=*((short *)(decode+34));

        ks::AudioFormat audioFormat;
        audioFormat.sampleRate = header.sampleRate;
        audioFormat.channelFmt = static_cast<ks::EChannelFormat>(header.channelCnt);
        if(header.bitsPerSample == 16)
        {
            audioFormat.sampleFmt = ks::ESampleFormat::INT16;
        }
        else
            assert(false);//TODO

        int32_t dataSize = *(int32_t *) (decode + 40);

        AudioSamples *samples = new AudioSamples(audioFormat,dataSize/(header.blockAlign));
        assert(stream->read(samples->data(),dataSize) == dataSize);
        KSLOGD(TAGWAV,"audio sampleCnt %d totol buf size %d",samples->getSampleCnt(),dataSize);
        //TODO remove debug only
        header.printAudioFormat();

        stream->close();
        delete stream;

        return samples;
    }
};


#endif //KALASOFT_WAVDECODER_H
