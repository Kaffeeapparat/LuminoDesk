#pragma once

#include "../ledchannel/ledchannel.h"

typedef struct
{
    uint8_t state;
    uint8_t active_channel;
    RGBColorSelect active_color;

    channel_t *channel_map;
}device_t;

typedef enum {
    init,
    waitforaction,
    effect,
    constant,
    mode,
    remoteisconnected,
    keepconnection,
    recievedcommando,


} Mainstate;

typedef enum {
    waitforinput,
    selectchannel,
    rgbselselect,
} Sidestate;

void updateActiveColor();
void updateActiveChannel();