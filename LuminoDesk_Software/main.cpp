#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <cstdint>

#include "lib/hardwaredef/hardwaredef.hpp"
#include "lib/testroutines/testroutines.hpp"
#include "lib/ledchannel/ledchannel.hpp"
#include "lib/device/device.hpp"
#include "lib/shiftregister/shiftregister.hpp"

//extern char* button_map[2][5];
extern uint32_t shiftregisterbitmask;
extern ButtonAction lastinput;
extern int8_t pressed_button_lock;
extern uint8_t tick;
extern int32_t tmpencoderval;

extern void signalNTimes(uint32_t time_us, int8_t pulses, uint8_t pin);

bool generateTick(struct repeating_timer *t);
bool rotaryencoder1_isr(struct repeating_timer *t);
void printLastInput();
void updateShiftregister();
void checkButtonDebounceLock();



int main() {
    stdio_init_all();
    std::cout << "Teststring, please!!!" << "\n";
    if (cyw43_arch_init()) {
        std::cout << "WiFi init failed" << std::endl;
        gpio_init(1);
        gpio_set_dir(1,1);
        gpio_put(1,1);
        return -1;
    }
    initGPIO();
    //Create a datastructure to control the device
    Device instance;

   
    
    //Create a channel
    Channel channel0(0,MODE_ANALOG,5,CH0_CHG_MOD,CH0_CHG_VLT,5,CH0_LED_R,CH0_LED_G,CH0_LED_B);
    Channel channel1(1,MODE_DIGITAL,5,CH0_CHG_MOD,CH0_CHG_VLT,6,13,11,12);
    //Channel channel2(2,MODE_ANALOG,5,CH2_CHG_MOD,CH2_CHG_VLT,5,CH2_LED_R,CH2_LED_G,CH2_LED_B);
    


    instance.addChannel(&channel0);
    instance.addChannel(&channel1);

    
    //Create a Shiftregister
    Shiftregister shiftregister0(SHIFT_LATCH,SHIFT_CLK,SHIFT_DATA,10);
    instance.setActiveColor(RGBColorSelect::RED);
    instance.setActiveChannelId(0); 

    //Initial SMC Toggle to set up the Switchmatrix
    shiftregister0.setShiftmask(shiftregisterbitmask);
    shiftregister0.setBit(SHIFTMASK_SMR0);

    //init the rotary encoder
    rotaryencoder1_init();
    
    
    //set alarm to generate ticks regular
    static struct repeating_timer timer;
    add_repeating_timer_ms(1, generateTick, NULL, &timer);
    //set alarm for sampling the rotary encoder
    static struct repeating_timer encoder1;
    add_repeating_timer_us(1500, rotaryencoder1_isr, NULL, &encoder1);


    instance.setActiveState(DeviceState::OPERATION);
    while(1==1){
    checkButtonDebounceLock();


    //Make sure that the device is always in the right state. Only done if device is not in remote
    if(tick==0&&(instance.getActiveState()!=DeviceState::REMOTE))
    {
        if(instance.getActiveChannel()->getEffectEnable())
        {
            instance.setActiveState(DeviceState::OPERATION_FX);
        }
        else
        {
            instance.setActiveState(DeviceState::OPERATION_CONST);
        }
    }


    if((tick==10))

        {
         if(!pressed_button_lock){
                shiftregister0.toggleBit(SHIFTMASK_SMR0);
                shiftregister0.toggleBit(SHIFTMASK_SMR1);
        }
         
        instance.updateDeviceStateSignals(shiftregister0);
        while((tick==10)||(tick==60))
        {
            tight_loop_contents();
        }
        printLastInput();
        switch (lastinput) 
        {
        case ButtonAction::onoff_long:
            // Handle onoff_long action
            lastinput = ButtonAction::dummy;
            break;
        case ButtonAction::onoff_short:
            // Handle onoff_short action
            lastinput = ButtonAction::dummy;
            break;
        case ButtonAction::constant_long:
            // Handle constant_long action
            lastinput = ButtonAction::dummy;
            break;
        case ButtonAction::constant_short:
            // Handle constant_short action
            lastinput = ButtonAction::dummy;
            break;
        case ButtonAction::channelonoff_long:
            // Handle channelonoff_long action
            lastinput = ButtonAction::dummy;
            break;
        case ButtonAction::channelonoff_short:
            instance.getActiveChannel()->toggleEnable();
            lastinput = ButtonAction::dummy;
            break;
        case ButtonAction::speed_long:
            // Handle speed_long action
            lastinput = ButtonAction::dummy;
            break;
        case ButtonAction::speed_short:
            // Handle speed_short action
            lastinput = ButtonAction::dummy;
            break;
        case ButtonAction::remote_long:
            // Handle remote_long action
            lastinput = ButtonAction::dummy;
            break;
        case ButtonAction::remote_short:
            // Handle remote_short action
            lastinput = ButtonAction::dummy;
            break;
        case ButtonAction::effect_long:
            // Handle effect_long action
            lastinput = ButtonAction::dummy;
            break;
        case ButtonAction::effect_short:
            // Handle effect_short action
            lastinput = ButtonAction::dummy;
            break;
        case ButtonAction::empty_long:
            // Handle empty_long action
            lastinput = ButtonAction::dummy;
            break;
        case ButtonAction::empty_short:
            // Handle empty_short action
            lastinput = ButtonAction::dummy;
            break;
        case ButtonAction::rotary_short:
            // Handle rotary_short action

            if(instance.getActiveChannel()->getMode()==MODE_DIGITAL)
            {
                instance.setSideState(SideState::change_number_of_leds);
            }
            lastinput = ButtonAction::dummy;
            break;
        case ButtonAction::rotary_long:
            // Handle rotary_long action
            if(instance.getActiveChannel()->getMode()==MODE_DIGITAL)
            {
                instance.setSideState(SideState::change_led_color_and_channel);
            }
            lastinput = ButtonAction::dummy;
            break;
        case ButtonAction::channels_long:
            lastinput = ButtonAction::dummy;
            break;
        case ButtonAction::channels_short:
            instance.toggleActiveChannel();
            // Handle channels_short action
            lastinput = ButtonAction::dummy;
            break;
        case ButtonAction::rgbs_long:
            lastinput = ButtonAction::dummy;
            break;
        case ButtonAction::rgbs_short:
            instance.toggleActiveColor();
            lastinput = ButtonAction::dummy;
            break;
    }
    }
    if(tick==20)
    {

    if(0!=tmpencoderval)
    {
        if(instance.getSideState()==SideState::change_number_of_leds)
        {
            instance.getActiveChannel()->setNumberOfLeds(instance.getActiveChannel()->getNumberOfLeds()+tmpencoderval);
            instance.getActiveChannel()->putRGBChannelData();
        }
        else if(instance.getSideState()==SideState::change_led_color_and_channel)
        {
            instance.getActiveChannel()->putincRGBChannelData(instance.getActiveColor(),tmpencoderval);
            printf("/n Rotary Encoder:%d",tmpencoderval);
        }
    }
    tmpencoderval=0;
    }
    
}

    
    return 0;
}


void printLastInput(){

    printf("\n\nLast input was:");
switch (lastinput) {
    case ButtonAction::onoff_long:
        printf("onoff_long");
        break;
    case ButtonAction::onoff_short:
        printf("onoff_short");
        break;
    case ButtonAction::constant_long:
        printf("constant_long");
        break;
    case ButtonAction::constant_short:
        printf("constant_short");
        break;
    case ButtonAction::remote_long:
        printf("remote_long");
        break;
    case ButtonAction::remote_short:
        printf("remote_short");
        break;
    case ButtonAction::channelonoff_short:
        printf("channelonoff_short");
        break;
    case ButtonAction::channelonoff_long:
        printf("channelonoff_long");
        break;
    case ButtonAction::effect_long:
        printf("effect_long");
        break;
    case ButtonAction::effect_short:
        printf("effect_short");
        break;
    case ButtonAction::speed_long:
        printf("speed_long");
        break;
    case ButtonAction::speed_short:
        printf("speed_short");
        break;
    case ButtonAction::rgbs_short:
        printf("rgbs_short");
        break;
    case ButtonAction::rgbs_long:
        printf("rgbs_long");
        break;
    case ButtonAction::channels_short:
        printf("channels_short");
        break;
    case ButtonAction::channels_long:
        printf("channels_long");
        break;
    case ButtonAction::empty_long:
        printf("empty_long");
        break;
    case ButtonAction::empty_short:
        printf("empty_short");
        break;
    case ButtonAction::dummy:
        printf("dummy");
        break;
    default:
        printf("Unknown ButtonAction");
        break;
}

    printf("\n");

    return;
}
