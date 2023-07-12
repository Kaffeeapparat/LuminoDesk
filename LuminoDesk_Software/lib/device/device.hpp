#pragma once

#include "../ledchannel/ledchannel.hpp"
#include "../shiftregister/shiftregister.hpp"
#include <vector>
#include <iterator>
#include <map>

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
    waitforinput,
    selectchannel,
    rgbselselect,
};


class Device {
public:
    Device();

    void addChannel(Channel& channel,uint8_t frontpanel_led);
    void addChannel(Channel& channel);
    void delChannel(Channel& channel);

    uint8_t getNumberofChannels();
    uint8_t getActiveChannelId();
    void setActiveChannelId(uint8_t active_id);
    Channel& getActiveChannel();
    void toggleActiveChannel();
    void setShiftregister(Shiftregister& shift_register);

    RGBColor getActiveColor();
    void setActiveColor(RGBColor color);
    void toggleActiveColor();

    DeviceState getActiveState();
    void setActiveState(DeviceState state);

    void updateDeviceStateSignals(Shiftregister& shift_register);


private:
    
    Channel active_channel;
    RGBColor active_color;
    DeviceState active_state;
    std::vector<Channel> channels; 
    std::map<Channel,uint8_t> ch_fp_led_map;



};
