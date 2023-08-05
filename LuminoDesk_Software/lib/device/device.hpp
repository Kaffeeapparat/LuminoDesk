#pragma once

#include "../ledchannel/ledchannel.hpp"
#include "../shiftregister/shiftregister.hpp"
#include "../effect/effect.hpp"
#include <vector>
#include <iterator>
#include <map>
#include <functional>


enum class DeviceState {
    INIT,
    EFFECT,
    CONSTANT,
    EFFECT_MODE,
    EFFECT_SPEED,
    REMOTEISCONNECTED,
    KEEPCONNECTION,
    RECIEVEDCOMMANDO,
};

enum class SideState {
    change_led_color_and_channel, //Change channel and rgb selection
    change_number_of_leds, //Change number of digital leds

};


class Device {
public:
    Device();

    void addChannel(Channel* channel,uint8_t frontpanel_led);
    void addChannel(Channel* channel);
    void delChannel(Channel* channel);

    uint8_t getNumberofChannels();
    uint8_t getActiveChannelId();
    void setActiveChannelId(uint8_t active_id);
    Channel* getActiveChannel();
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


private:
    Channel* active_channel;
    RGBColorSelect active_color;
    DeviceState active_state;
    SideState active_side_state;
    std::vector<Channel*> channels; 
};