#include "ledchannel.h"

channel_t channel0;
channel_t channel1;
channel_t channel2;

channel_t channel_map[3];
uint8_t enablechannelsignals[]={CH0_ENB_CHN,CH1_ENB_CHN,CH2_ENB_CHN};
uint8_t voltagechannelsignals[]={CH0_CHG_VLT,CH1_CHG_VLT,CH2_CHG_VLT};
uint8_t modeChannelsignals[]={CH0_CHG_MOD,CH1_CHG_MOD,CH2_CHG_MOD};
uint8_t colorRChannelsignals[]={CH0_LED_R,CH1_LED_R,CH2_LED_R};
uint8_t colorGChannelsignals[]={CH0_LED_G,CH1_LED_G,CH2_LED_G};
uint8_t colorBChannelsignals[]={CH0_LED_B,CH1_LED_B,CH2_LED_B};

uint8_t (*gpio_map[6])[] = {
    &enablechannelsignals,
    &voltagechannelsignals,
    &modeChannelsignals,
    &colorRChannelsignals,
    &colorGChannelsignals,
    &colorBChannelsignals
};

void initChannels(){

    // channel 0
    channel0.id=CHANNEL_0;
    channel0.mode=MODE_ANALOG;
    channel0.voltage=VOLTAGE_LOW;

    channel0.mode_signal=CH0_CHG_MOD;
    channel0.voltage_signal=CH0_CHG_VLT;
    //channel0.enable_signal=CH0_ENB_CHN;

    channel0.color_r=0;
    channel0.color_b=0;
    channel0.color_g=0;

    channel0.color_r_signal=CH0_LED_R;
    channel0.color_g_signal=CH0_LED_G;
    channel0.color_b_signal=CH0_LED_B;

    channel0.effect=0;
    channel0.effectfunc=NULL;

    //channel1
    channel1.id=CHANNEL_1;
    channel1.mode=MODE_ANALOG;
    channel1.voltage=VOLTAGE_LOW;

    channel1.mode_signal=CH1_CHG_MOD;
    channel1.voltage_signal=CH1_CHG_VLT;
    channel1.enable_signal=CH1_ENB_CHN;

    channel1.color_r=0;
    channel1.color_b=0;
    channel1.color_g=0;

    channel1.color_r_signal=CH1_LED_R;
    channel1.color_g_signal=CH1_LED_G;
    channel1.color_b_signal=CH1_LED_B;
    
    channel1.effect=0;
    channel1.effectfunc=NULL;

    //channel 2
    channel2.id = CHANNEL_2;
    channel2.mode = MODE_ANALOG;
    channel2.voltage = VOLTAGE_LOW;

    channel2.mode_signal = CH2_CHG_MOD;
    channel2.voltage_signal = CH2_CHG_VLT;
    channel2.enable_signal = CH2_ENB_CHN;

    channel2.color_r = 0;
    channel2.color_b = 0;
    channel2.color_g = 0;

    channel2.color_r_signal = CH2_LED_R;
    channel2.color_g_signal = CH2_LED_G;
    channel2.color_b_signal = CH2_LED_B;

    channel2.effect = 0;
    channel2.effectfunc = NULL;

    channel_map[0]=channel0;
    channel_map[1]=channel1;
    channel_map[2]=channel2;

    //Sets all channels to analog 5V mode at start
    
    for (int i =0;i<3;i++){
        gpio_init(channel_map[i].color_r_signal);
        gpio_init(channel_map[i].color_g_signal);
        gpio_init(channel_map[i].color_b_signal);
        gpio_init(channel_map[i].mode_signal);
        //gpio_init(channel_map[i].enable_signal);
        gpio_init(channel_map[i].voltage_signal);

        gpio_set_dir(channel_map[i].color_r_signal,1);
        gpio_set_dir(channel_map[i].color_g_signal,1);
        gpio_set_dir(channel_map[i].color_b_signal,1);
        gpio_set_dir(channel_map[i].mode_signal,1);
        gpio_set_dir(channel_map[i].enable_signal,1);
        gpio_set_dir(channel_map[i].voltage_signal,1);
  

        gpio_set_function(channel_map[i].color_r_signal,GPIO_FUNC_PWM);
        uint slice_num = pwm_gpio_to_slice_num(channel_map[i].color_r_signal);
        pwm_set_wrap(slice_num, PWM_WRAP);

        gpio_set_function(channel_map[i].color_g_signal,GPIO_FUNC_PWM);
         slice_num = pwm_gpio_to_slice_num(channel_map[i].color_g_signal);
        pwm_set_wrap(slice_num, PWM_WRAP);

        gpio_set_function(channel_map[i].color_b_signal,GPIO_FUNC_PWM);
         slice_num = pwm_gpio_to_slice_num(channel_map[i].color_b_signal);
        pwm_set_wrap(slice_num, PWM_WRAP);
            
            printf("Checkn\n");
    }
    printf("Check1\n");
return;

}

//Changes the Voltage in Hardware
void changeVoltageChannel(uint8_t channel, uint8_t voltage)
{ 

}

//Chagnes the Mode of operation in hardware
void changeModeChannel(uint8_t channel, uint8_t mode)
{

}


//Enables or Diables the channel in Hardware
void setEnableChannel(channel_t *channelmap)
{ 
    uint8_t slice_n=0;
    
    if((channelmap->enable)){
        gpio_put(channelmap->enable_signal,1);
        }
    if(!(channelmap->enable)){
        gpio_put(channelmap->enable_signal,0);
        }
        return;
}

//Changes the RGB Values in the structure
//Pass -1 to not change the color, maximum value is PWM_WRAP
void updateRgbChannelData(channel_t **channelmap,uint8_t channel,int32_t color_r,int32_t color_b,int32_t color_g)
{
    for(int i=0;i<3;i++){
        if(channel_map[i].id==channel){
            if(color_r!=-1){channel_map[i].color_r=(uint16_t)color_r;}
            if(channel_map[i].color_r>PWM_WRAP){channel_map[i].color_r=PWM_WRAP;}
            if(color_g!=-1){channel_map[i].color_g=(uint16_t)color_g;}
            if(channel_map[i].color_g>PWM_WRAP){channel_map[i].color_g=PWM_WRAP;}
            if(color_b!=-1){channel_map[i].color_b=(uint16_t)color_b;}
            if(channel_map[i].color_b>PWM_WRAP){channel_map[i].color_b=PWM_WRAP;}
        }
    }
    return;
}

uint16_t getRgbChannelData(channel_t **channelmap,uint8_t channel,RGBcolor color){

    switch(color){
    case red:
        return channelmap[channel]->color_r;
    case green:
        return channelmap[channel]->color_g;
    case blue:
        return channelmap[channel]->color_r;
    }
    return 0;
}


//Change the RGB output dependend on strip data
void updateRgbStripeData(uint8_t channel)
{

}

//Change the RGB Output dependend on the in the structure saved rgb values
//For Constant Color only
void changeRgbChannel(uint8_t channel)
{
    for(int i=0;i<3;i++){
        if(channel_map[i].id==channel){
            uint slice_num = pwm_gpio_to_slice_num(channel_map[i].color_r_signal);
            uint channel_num = pwm_gpio_to_channel(channel_map[i].color_r_signal);
            pwm_set_chan_level(slice_num, channel_num, channel_map[i].color_r);
            pwm_set_enabled(slice_num, true);

            slice_num = pwm_gpio_to_slice_num(channel_map[i].color_g_signal);
            channel_num = pwm_gpio_to_channel(channel_map[i].color_g_signal);
            pwm_set_chan_level(slice_num, channel_num, channel_map[i].color_g);
            pwm_set_enabled(slice_num, true);

            slice_num = pwm_gpio_to_slice_num(channel_map[i].color_b_signal);
            channel_num = pwm_gpio_to_channel(channel_map[i].color_b_signal);
            pwm_set_chan_level(slice_num, channel_num, channel_map[i].color_b);
            pwm_set_enabled(slice_num, true);
        }
    }
    return;
}
