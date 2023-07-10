#ifndef LEDCHANNELDEF_H
#define LEDCHANNELDEF_H

#include "../hardwaredef/hardwaredef.h"
#include <stdlib.h>

//Number of channels
#define NUMBER_OF_CHANNELS 3

typedef struct
{
    uint8_t id;
    uint8_t mode;
    uint8_t voltage;    
    bool enable;

    uint8_t mode_signal;
    uint8_t voltage_signal;
    uint8_t enable_signal;

    uint32_t color_r;
    uint32_t color_b;
    uint32_t color_g;

    uint8_t color_r_signal;
    uint8_t color_b_signal;
    uint8_t color_g_signal;

    uint32_t number_of_led;
    uint32_t *led_strip_data; 

    bool effect;
    void *effectfunc;
}channel_t;

typedef enum{
    red,
    green,
    blue,
}RGBcolor;

// Function Prototypes
void initChannels();
void changeVoltageChannel(uint8_t channel, uint8_t voltage);
void changeModeChannel(uint8_t channel, uint8_t mode);
void setEnableChannel(channel_t *channelmap,uint8_t channel);
void changeRgbChannel(channel_t *channelmap,uint8_t channel);

void updateRgbChannelData(channel_t *channelmap,uint8_t channel,int32_t color_r,int32_t color_b,int32_t color_g);
void updateSingleRGBChannelData(channel_t *channelmap,uint8_t channel,RGBcolor color,int32_t rgbvalue);
uint16_t getRgbChannelData(channel_t *channelmap,uint8_t channel,RGBcolor color);


void updateRgbStripeData(uint8_t channel);

#endif 
