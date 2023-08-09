#pragma once

#include "../ledchannel/ledchannel.hpp"
#include "../shiftregister/shiftregister.hpp"
#include "../effect/effect.hpp"
#include <vector>
#include <iterator>
#include <map>
#include <functional>


enum class DeviceState {
    //The device is initialising itself
    INIT,
    //The device is open for user input and operates the channel
    OPERATION,
    //The device is operating in effect mode
    OPERATION_FX,
    //The device is operating in const mode
    OPERATION_CONST,
    //The device ignores user inputs except break connection and mainly operates on remote connections  
    REMOTE,
    };

enum class SideState {
    change_led_color_and_channel, //Change channel and rgb selection
    change_number_of_leds, //Change number of digital leds
    change_speed_of_effect,
    change_type_of_effect,
    change_para0_of_effect,
    change_para1_of_effect,
    change_color1_of_effect,
};


class Device {
public:
    Device();

    void addChannel(Channel* channel,Effect* effect,uint8_t frontpanel_led);
    void addChannel(Channel* channel,Effect* effect);
    void delChannel(Channel* channel);

    uint8_t getNumberofChannels();
    uint8_t getActiveChannelId();
    void setActiveChannelId(uint8_t active_id);
    Channel* getActiveChannelByID(uint8_t active_id);
    Channel* getActiveChannel();
    Effect* getActiveEffect();
    void toggleActiveChannel();
    void setShiftregister(Shiftregister& shift_register);

    RGBColorSelect getActiveColor();
    void setActiveColor(RGBColorSelect color);
    void toggleActiveColor();

    DeviceState getActiveState();
    void setActiveState(DeviceState state);

    SideState getSideState();
    void setSideState(SideState state);

    void updateDeviceStateSignals(Shiftregister& shift_register);

    void updateAllEffects();

    //Methods to handle the on/off function

    void turnOn();
    void turnOff();


private:
    Channel* active_channel;
    RGBColorSelect active_color;
    DeviceState active_state;
    SideState active_side_state;
    std::vector<Channel*> channels; 
    std::map<Channel*,Effect*>effectmap;
    
    //Saves the state of the channels so during an on/off cycle the device remebers which channels to turn on
    std::vector<bool> onoff_memory;
    bool is_on;
};