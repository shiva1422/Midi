//
// Created by pspr1 on 8/27/2020.
//

#ifndef DIGIBAND_PIXAMAP_H
#define DIGIBAND_PIXAMAP_H


typedef struct PixaMap{
    uint width,height,stride,startX,startY;
    uint8_t * pixels=NULL;
}PixaMap;

#endif //DIGIBAND_PIXAMAP_H
