#include <stdio.h>
#include "lib/hardwaredef/hardwaredef.h"
#include "lib/testroutines/testroutines.h"
#include "lib/ledchannel/ledchannel.h"
#include "lib/menu/menu.h"

//extern char* button_map[2][5];
extern uint32_t shiftregisterbitmask;
extern ButtonAction lastinput;
extern int8_t pressed_button_lock;
extern uint8_t tick;
extern int32_t tmpencoderval;

extern channel_t channel_map[];


bool generateTick(struct repeating_timer *t);
void printLastInput();
void updateShiftregister();
void checkButtonDebounceLock();
void frontpanel_updateLedActiveChannel(device_t* instance);
void frontpanel_updateLedActiveColor(device_t* instance);
void channel_toggleChannelonoff(device_t* instance);
void channel_updateLedValuecolor(device_t* instance); 

int main() {
    stdio_init_all();
    printf("Check1\n");
    if (cyw43_arch_init()) {
        printf("WiFi init failed");
        gpio_init(1);
        gpio_set_dir(1,1);
        gpio_put(1,1);
        return -1;
    }
        initChannels();
    printf("Check2\n");
    initGPIO();
printf("Check3\n");

    rotaryencoder1_init();
    shiftregisterbitmask ^=(1<<SHIFTMASK_SMR0);
    printf("Check4\n");
    //set alarm to update the shiftregister routinely
    static struct repeating_timer timer;
    add_repeating_timer_ms(1, generateTick, NULL, &timer);
    //set alarm for sampling the rotary encoder

    // channels being refrenced in device_t
    channel_map[0].color_b_signal=CH0_LED_B;
    channel_map[0].color_g_signal=CH0_LED_G;
    channel_map[0].color_r_signal=CH0_LED_R;
    channel_map[0].enable_signal=5;
    gpio_init(5);
    gpio_set_dir(5,1);
    channel_map[0].enable=0;



    //Struct which saves the device state
    device_t instance;
    instance.active_channel=0;
    instance.active_color=red;
    instance.channel_map=channel_map;

    static struct repeating_timer encoder1;
    add_repeating_timer_us(1300, rotaryencoder1_isr, NULL, &encoder1);

    frontpanel_updateLedActiveColor(&instance);
    frontpanel_updateLedActiveChannel(&instance);
    updateShiftregister();
    while(1==1){
        tight_loop_contents();
        checkButtonDebounceLock();
        
        if((tick==10)||(tick==60))
        {
            if(!pressed_button_lock){
                shiftregisterbitmask^=((1<<SHIFTMASK_SMR0)|(1<<SHIFTMASK_SMR1));
            }
            updateShiftregister();
            while((tick==10)||(tick==60))
            {
                tight_loop_contents();
            }
        }
        if(tick==0)
        {
            //printLastInput();
            //printf("\nLast Encoder Value:%d\n",tmpencoderval);
            
            if(lastinput==rgbs_short)
            {
                updateActiveColor(&instance);
                frontpanel_updateLedActiveColor(&instance);
                lastinput=dummy;
            }
            if(lastinput==channels_short)
            {
                updateActiveChannel(&instance);
                frontpanel_updateLedActiveChannel(&instance);
                lastinput=dummy;
            }

            if(tmpencoderval>0 || tmpencoderval<0)
            {
                channel_updateLedValuecolor(&instance);
            }

            if(lastinput==channelonoff_short)
            {
                channel_toggleChannelonoff(&instance);
                lastinput=dummy;
            }

            while(tick==0)
            {
                tight_loop_contents();
            }

            //printLastInput();
        }
        if((tick%2)==0)
        {
            //signalNTimes(10,1,0);
        }
        
    }
    
    /*
    while (1)
    {
        tight_loop_contents();
        printf("hello word\n"); 
        sleep_ms(100);
        blinkOnce(2999);
        }
    */

    return 0;    
}

bool generateTick(struct repeating_timer *t)
{
    if(tick<100){
        tick++;
    }
    else{
        tick=0;
    }
    return true;
} 

void updateShiftregister() {


            //2.Vorschleife Clocksignale geben u. Ausgang
            
    for(int k=0;k<16;k++)
        {
        gpio_put(SHIFT_CLK,0);
        
        
        if(shiftregisterbitmask & (1<< k)){
                gpio_put(SHIFT_DATA,1);
        }
            
        gpio_put(SHIFT_CLK,1);
        sleep_us(10);
        gpio_put(SHIFT_CLK,0);
        sleep_us(10);
        gpio_put(SHIFT_DATA,0);
        sleep_us(10);
                
    }
                     
            gpio_put(SHIFT_LATCH,1);
            sleep_us(10);
            gpio_put(SHIFT_LATCH,0);  

            return;
}

void printLastInput(){

    printf("\n\nLast input was:");
    switch (lastinput) {
        case onoff_long:
            printf("onoff_long");
            break;
        case onoff_short:
            printf("onoff_short");
            break;
        case constant_long:
            printf("constant_long");
            break;
        case constant_short:
            printf("constant_short");
            break;
        case remote_long:
            printf("remote_long");
            break;
        case remote_short:
            printf("remote_short");
            break;
        case channelonoff_short:
            printf("channelonoff_short");
            break;
        case channelonoff_long:
            printf("channelonoff_long");
            break;
        case effect_long:
            printf("effect_long");
            break;
        case effect_short:
            printf("effect_short");
            break;
        case speed_long:
            printf("speed_long");
            break;
        case speed_short:
            printf("speed_short");
            break;
        case rgbs_short:
            printf("rgbs_short");
            break;
        case rgbs_long:
            printf("rgbs_long");
            break;
        case channels_short:
            printf("channels_short");
            break;
        case channels_long:
            printf("channels_long");
            break;
        case empty_long:
            printf("empty_long");
            break;
        case empty_short:
            printf("empty_short");
            break;
        case dummy:
            printf("dummy");
            break;
        default:
            printf("Unknown ButtonAction");
            break;
    }
    printf("\n");

    return;
}

void checkButtonDebounceLock(){
    
uint32_t current_time=to_ms_since_boot (get_absolute_time());

    for(int row=0;row<2;row++){
        for(int pos=0;pos<5;pos++){
                if(button_map[row][pos].debouncelock>current_time){
                    pressed_button_lock=0;
                }
            }

        }

    return;
}

void frontpanel_updateLedActiveChannel(device_t* instance){

    turnOffSignal(LED_CH0);
    turnOffSignal(LED_CH1);
    turnOffSignal(LED_CH2);

    if(instance->active_channel==0)
    {
        turnOnSignal(LED_CH0);
    }
    else if(instance->active_channel==1)
    {
        turnOnSignal(LED_CH1);
    }
    else if(instance->active_channel==2)
    {
        turnOnSignal(LED_CH2);
    }
    else
    {
        printf("/nERROR: Unavailable Channel/n");
    }
    return;
}

void frontpanel_updateLedActiveColor(device_t* instance){

    turnOffSignal(LED_R);
    turnOffSignal(LED_B);
    turnOffSignal(LED_G);   

    switch (instance->active_color) {
        case red:
            turnOnSignal(LED_R);
            break;
        case green:
            turnOnSignal(LED_G);
            break;
        case blue:
            turnOnSignal(LED_B);
            break;
    }

    return;

}

/*
* Updates the value of RGB color values dependend on the rotary encoder value
*/
void channel_updateLedValuecolor(device_t* instance){
    uint8_t channel=instance->active_channel;
    RGBcolor color=instance->active_color;
    uint16_t tempcolorvalue=0;
    

     //printf("tmpencoder  :%i\ntmpencoder*3:%i\ncurrnet_value:%i\n",tmpencoderval,tmpencoderval*3,getRgbChannelData(&instance->channel_map,channel,color));

    if((tmpencoderval*9*5+(int32_t)getRgbChannelData(&instance->channel_map,channel,color))<0)
    {
        tempcolorvalue=0;
    }
    else
    {
        tempcolorvalue=(uint16_t)(tmpencoderval*9*5+(int32_t)getRgbChannelData(&instance->channel_map,channel,color));
    }
    tmpencoderval=0;
                        
    //printf("tmpencoder:%i\ncurrnet_value:%i\n",tmpencoderval,instance->channel_map[channel].color_b);
            
    updateRgbChannelData(&instance->channel_map,channel,tempcolorvalue,-1,-1);
    changeRgbChannel(channel);

    return;

}

/*
* Turns on and off the enable signal of an channel
*/
void channel_toggleChannelonoff(device_t* instance){
    uint8_t channel=instance->active_channel;

    if(instance->channel_map[channel].enable){
        instance->channel_map[channel].enable=0;
    }
    else{
        instance->channel_map[channel].enable=1;
    }

    setEnableChannel(instance->channel_map);

    return;
}