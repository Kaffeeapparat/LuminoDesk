#include "hardwaredef.hpp"
#include <iostream>
#include <iterator>
#include <chrono>
void button_isr(uint gpio, uint32_t events);



uint8_t tick;
int32_t tmpencoderval;
uint32_t shiftregisterbitmask=0;
ButtonAction lastinput;

int inputpins[]={SMC1,SMC2,SMC3,SMC4,SMC5,QUAD_1,QUAD_2};

Encoder quad_map[1]={
    {QUAD_1,QUAD_2,0,0,0,0},
};


Button button_map[2][5] = {
    {{SMC1, SMCR1, 0, 0, ButtonAction::onoff_long, ButtonAction::onoff_short},
     {SMC2, SMCR1, 0, 0, ButtonAction::constant_long, ButtonAction::constant_short},
     {SMC5, SMCR1, 0, 0, ButtonAction::channelonoff_long, ButtonAction::channelonoff_short},
     {SMC3, SMCR1, 0, 0, ButtonAction::speed_long, ButtonAction::speed_short},
     {SMC4, SMCR1, 0, 0, ButtonAction::remote_long, ButtonAction::remote_short}},
    {{SMC1, SMCR2, 0, 0, ButtonAction::effect_long, ButtonAction::effect_short},
     {SMC2, SMCR2, 0, 0, ButtonAction::empty_long, ButtonAction::empty_short},
     {SMC3, SMCR2, 0, 0, ButtonAction::rotary_short, ButtonAction::rotary_long},
     {SMC4, SMCR2, 0, 0, ButtonAction::channels_long, ButtonAction::channels_short},
     {SMC5, SMCR2, 0, 0, ButtonAction::rgbs_long, ButtonAction::rgbs_short}}
};

void turnOnSignal(uint8_t led)
{
    shiftregisterbitmask|=(1<<led);
}

void turnOffSignal(uint8_t led)
{
    shiftregisterbitmask&=~(1<<led);
}

void initGPIO(void)
{
    for(int i=0;i<6;i++)
    {
        gpio_init(inputpins[i]);
        gpio_set_dir(inputpins[i],GPIO_IN);
        gpio_pull_up(inputpins[i]);
        gpio_set_irq_enabled_with_callback(inputpins[i],GPIO_IRQ_EDGE_RISE|GPIO_IRQ_EDGE_FALL,true,&button_isr);
    }
}

int8_t pressed_button_lock=0;
uint32_t pressed_button_time;
Button pressed_button;

const uint32_t debounce_threshold=50;
const uint32_t longpress_threshold=2000;
const uint32_t failure_threshold=10000;
const uint32_t debouncelock_threshold=400;

void button_isr(uint gpio, uint32_t events)
{
    if(GPIO_IRQ_EDGE_RISE==events)
    {
        gpio_acknowledge_irq(gpio,GPIO_IRQ_EDGE_RISE);
    }
    else
    {
        gpio_acknowledge_irq(gpio,GPIO_IRQ_EDGE_FALL);
    }



    int i=0;
    int pos=-1;
    int row;

    if(shiftregisterbitmask&(1<<SHIFTMASK_SMR0))
    {
        row=1;

    }
    else if(shiftregisterbitmask&(1<<SHIFTMASK_SMR1))
    {
        row=0;
    }
    else
    {
        std::cout << "Eror, No Shiftmask set" << "\n";
        return;
    }

    for(int j=0;j<5;j++)
    {
        if(((uint)button_map[row][j].gpio)==gpio)
        {
            pos=j;
        }
    }


    if(pos==-1)
    {
        return;
    }

    i=pos;
    uint32_t current_time=to_ms_since_boot(get_absolute_time());

    if(button_map[row][pos].debouncelock>current_time)
    {
        //std::cout << "presed burtton lock unset in isr" << "\n";
        pressed_button_lock=0;
        return;
    }

    if(GPIO_IRQ_EDGE_FALL==events)
    {
                //std::cout << "presed burtton lock set in isr" << "\n";

        pressed_button_lock=1;
        button_map[row][pos].timestamp=current_time;
    }

    if(GPIO_IRQ_EDGE_RISE==events)
    {
        uint32_t deltat=current_time-button_map[row][i].timestamp;

        if(deltat<=debounce_threshold)
        {
            tight_loop_contents();
        }
        else if(deltat>=failure_threshold)
        {
            button_map[row][pos].timestamp=0;
            button_map[row][pos].debouncelock=current_time+debouncelock_threshold;
            button_map[row][pos].debouncelock=current_time+debouncelock_threshold;
            pressed_button_lock=0;
        }
        else if(deltat>longpress_threshold)
        {
            button_map[row][pos].timestamp=0;
            button_map[row][pos].debouncelock=current_time+debouncelock_threshold;
            lastinput=button_map[row][pos].longpress;
            pressed_button_lock=0;
        }
        else if(deltat<longpress_threshold)
        {
            button_map[row][pos].timestamp=0;
            button_map[row][pos].debouncelock=current_time+debouncelock_threshold;
            lastinput=button_map[row][pos].shortpress;
            pressed_button_lock=0;
        }
    }
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

/*
* Encoder init routine
* Inspired by https://www.mikrocontroller.net/articles/Drehgeber
*/
void rotaryencoder1_init(){

    gpio_init(QUAD_1);
    gpio_init(QUAD_2);
    gpio_set_dir(QUAD_1,GPIO_IN);
    gpio_set_dir(QUAD_2,GPIO_IN);
    gpio_pull_up(QUAD_1);
    gpio_pull_up(QUAD_2);

    quad_map[0].laststatep1=gpio_get(quad_map[0].phase1);
    quad_map[0].laststatep2=gpio_get(quad_map[0].phase2);
}


/*
* Encoder prcessing routine
* Inspired by https://www.mikrocontroller.net/articles/Drehgeber
*/
bool rotaryencoder1_isr(struct repeating_timer *t){
    /*
    uint32_t currenttime = to_us_since_boot(get_absolute_time());
    uint32_t deltat; 

    if(gpio==quad_map[0].phase1)
    {
        deltat=currenttime-quad_map[0].timestampp1;
        if(deltat<900)
        {
            return;
        }
        quad_map[0].timestampp1=currenttime;
    }
    if(gpio==quad_map[0].phase2)
    {
        deltat=currenttime-quad_map[0].timestampp2;
        if(deltat<900)
        {
            return;
        }
        quad_map[0].timestampp2=currenttime;
    }
*/
    int8_t neew,diff,last =0;

    if (gpio_get(quad_map[0].phase1)) neew=3;
    if (!gpio_get(quad_map[0].phase2)) neew^=1;
    last=(quad_map[0].laststatep1 << 1)|(quad_map[0].laststatep2);
    diff=last-neew;
    if (diff & 1)
    {
        quad_map[0].laststatep1 = gpio_get(quad_map[0].phase1);
        quad_map[0].laststatep2 = gpio_get(quad_map[0].phase2);
        tmpencoderval += (diff & 2)-1;
    }
    return 1;

}
