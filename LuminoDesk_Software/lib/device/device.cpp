#include "device.hpp"

Device::Device(): active_channel(), active_color(RGBColor::RED), active_state(){}

void Device::addChannel(Channel* channel)
{ 
    channels.push_back(channel);
}

uint8_t Device::getActiveChannelId()
{
    return active_channel->getId();
}

uint8_t Device::getNumberofChannels()
{
    return channels.size()-1;
}

void Device::setActiveChannelId(uint8_t active_id) {

    auto it = std::find_if(channels.begin(), channels.end(),
        [active_id](Channel* channel) { return channel->getId() == active_id; });

    if (it != channels.end()) {
        active_channel = *it;
    }
}

Channel* Device::getActiveChannel()
{
    return this->active_channel;
}

void Device::setActiveColor(RGBColor color)
{
    this->active_color=color;
}

void Device::toggleActiveColor()
{
    switch (this->active_color)
    {
        case RGBColor::RED:
            setActiveColor(RGBColor::GREEN);
            break;
        case RGBColor::GREEN:
            setActiveColor(RGBColor::BLUE);
            break;
        case RGBColor::BLUE:
            setActiveColor(RGBColor::RED);
            break;
        default:
            setActiveColor(RGBColor::RED);
            break;
    }
}

void Device::toggleActiveChannel()
{
    uint8_t number_of_channels=channels.size();
    if((getActiveChannelId()+1)>getNumberofChannels())
    {
        setActiveChannelId(0);
    }
    else
    {
        setActiveChannelId(getActiveChannelId()+1);
    }
}       

RGBColor Device::getActiveColor()
{
    return this->active_color;
}
void Device::updateDeviceStateSignals(Shiftregister& shift_register)
{
    switch (this->active_color)
    {
        case RGBColor::RED:
            shift_register.unsetBit(LED_B);
            shift_register.unsetBit(LED_G);
            shift_register.setBit(LED_R);
            break;
        case RGBColor::GREEN:
            shift_register.unsetBit(LED_B);
            shift_register.unsetBit(LED_R);
            shift_register.setBit(LED_G);
            break;
        case RGBColor::BLUE:
            shift_register.unsetBit(LED_R);
            shift_register.unsetBit(LED_G);
            shift_register.setBit(LED_B);
            break;
        default:
            shift_register.unsetBit(LED_R);
            shift_register.unsetBit(LED_G);
            shift_register.unsetBit(LED_B);
            break;
    }

    switch (this->active_channel->getId())
    {
        case 0:
            shift_register.unsetBit(LED_CH1);
            shift_register.unsetBit(LED_CH2);
            shift_register.setBit(LED_CH0);
            break;
        case 1:
            shift_register.unsetBit(LED_CH2);
            shift_register.unsetBit(LED_CH0);
            shift_register.setBit(LED_CH1);
            break;
        case 2:
            shift_register.unsetBit(LED_CH0);
            shift_register.unsetBit(LED_CH1);
            shift_register.setBit(LED_CH2);
            break;
        default:
            shift_register.unsetBit(LED_CH0);
            shift_register.unsetBit(LED_CH1);
            shift_register.unsetBit(LED_CH2);
            break;
    }
    
    switch (this->active_state)
    {
    case DeviceState::INIT:
        shift_register.unsetBit(LED_CONST);
        shift_register.unsetBit(LED_FX);
        shift_register.unsetBit(LED_CHTOGGLE);
        shift_register.unsetBit(LED_REMOTE);
        shift_register.unsetBit(LED_SPEED);
        shift_register.unsetBit(LED_EMPTY);
        break;
    case DeviceState::EFFECT:
        shift_register.unsetBit(LED_CONST);
        shift_register.setBit(LED_FX);
        shift_register.unsetBit(LED_CHTOGGLE);
        shift_register.unsetBit(LED_REMOTE);
        shift_register.unsetBit(LED_SPEED);
        shift_register.unsetBit(LED_EMPTY);
        break;
    case DeviceState::CONSTANT:
        shift_register.setBit(LED_CONST);
        shift_register.unsetBit(LED_FX);
        shift_register.unsetBit(LED_CHTOGGLE);
        shift_register.unsetBit(LED_REMOTE);
        shift_register.unsetBit(LED_SPEED);
        shift_register.unsetBit(LED_EMPTY);
        break;
    case DeviceState::EFFECT_MODE:
        shift_register.unsetBit(LED_CONST);
        shift_register.setBit(LED_FX);
        shift_register.setBit(LED_CHTOGGLE);
        shift_register.unsetBit(LED_REMOTE);
        shift_register.unsetBit(LED_SPEED);
        shift_register.unsetBit(LED_EMPTY);
        break;
    case DeviceState::EFFECT_SPEED:
        shift_register.unsetBit(LED_CONST);
        shift_register.setBit(LED_FX);
        shift_register.unsetBit(LED_CHTOGGLE);
        shift_register.unsetBit(LED_REMOTE);
        shift_register.setBit(LED_SPEED);
        shift_register.unsetBit(LED_EMPTY);
        break;
    case DeviceState::REMOTEISCONNECTED:
        break;
    case DeviceState::KEEPCONNECTION:
        break;
    case DeviceState::RECIEVEDCOMMANDO:
        break;
    default:
        shift_register.unsetBit(LED_CONST);
        shift_register.unsetBit(LED_FX);
        shift_register.unsetBit(LED_CHTOGGLE);
        shift_register.unsetBit(LED_REMOTE);
        shift_register.unsetBit(LED_SPEED);
        shift_register.setBit(LED_EMPTY);
        break;
    }

    shift_register.transmit();

}