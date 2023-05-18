#include "../hardwaredef/hardwaredef.h"

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

extern channel_t channel1;
extern channel_t channel2;
extern channel_t channel0;
extern channel_t channel_map[3];


// Function Prototypes
void initChannels();
void changeVoltageChannel(uint8_t channel, uint8_t voltage);
void changeModeChannel(uint8_t channel, uint8_t mode);
void changeEnableChannel(uint8_t channel, bool enable);
void changeRgbChannel(uint8_t channel);

void updateRgbChannelData(uint8_t channel, uint32_t color_e, uint32_t color_b, uint32_t color_g);
void updateRgbStripeData(uint8_t channel);