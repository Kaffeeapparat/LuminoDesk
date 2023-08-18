#include "device.hpp"

Device::Device(): active_channel(), active_color(RGBColorSelect::RED), active_state()
    {
        setSideState(SideState::change_led_color_and_channel);
        this->active_state=DeviceState::INIT;
        this->is_on=true;
    }

void Device::addChannel(Channel* channel,Effect* effect)
{ 
    channels.push_back(channel);
    effect->setChannel(channel);
    effectmap.insert({channel,effect});
    onoff_memory.clear();
    onoff_memory.resize(getNumberofChannels());
    
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

    if(this->active_state!=DeviceState::REMOTE)
    {
        if(this->active_channel->getEffectEnable())
            this->active_state=DeviceState::OPERATION_FX;
    }
    else
    {
        this->active_state=DeviceState::OPERATION_CONST;
    }
}

Channel * Device::getActiveChannelByID(uint8_t active_id) {

    auto it = std::find_if(channels.begin(), channels.end(),
        [active_id](Channel* channel) { return channel->getId() == active_id; });

    if (it != channels.end()) {
        return active_channel = *it;
    }
    return nullptr;
}

Channel* Device::getActiveChannel()
{
    return this->active_channel;
}

Effect* Device::getActiveEffect()
{
    return this->effectmap[this->active_channel];
}

void Device::setActiveColor(RGBColorSelect color)
{
    this->active_color=color;
}

void Device::toggleActiveColor()
{
    switch (this->active_color)
    {
        case RGBColorSelect::RED:
            setActiveColor(RGBColorSelect::GREEN);
            break;
        case RGBColorSelect::GREEN:
            setActiveColor(RGBColorSelect::BLUE);
            break;
        case RGBColorSelect::BLUE:
            setActiveColor(RGBColorSelect::VIOLET);
            break;
        case RGBColorSelect::VIOLET:
            setActiveColor(RGBColorSelect::TURQUOISE);
            break;
        case RGBColorSelect::TURQUOISE:
            setActiveColor(RGBColorSelect::WHITE);
            break;
        case RGBColorSelect::WHITE:
            setActiveColor(RGBColorSelect::RED);
            break;
        default:
            setActiveColor(RGBColorSelect::RED);
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

    if(this->active_state!=DeviceState::REMOTE)
    {
        if(this->active_channel->getEffectEnable())
            this->active_state=DeviceState::OPERATION_FX;
    }
    else
    {
        this->active_state=DeviceState::OPERATION_CONST;
    }
}       

RGBColorSelect Device::getActiveColor()
{
    return this->active_color;
}
void Device::updateDeviceStateSignals(Shiftregister& shift_register)
{
    switch (this->active_color)
    {
        case RGBColorSelect::RED:
            shift_register.unsetBit(LED_B);
            shift_register.unsetBit(LED_G);
            shift_register.setBit(LED_R);
            break;
        case RGBColorSelect::GREEN:
            shift_register.unsetBit(LED_B);
            shift_register.unsetBit(LED_R);
            shift_register.setBit(LED_G);
            break;
        case RGBColorSelect::BLUE:
            shift_register.unsetBit(LED_R);
            shift_register.unsetBit(LED_G);
            shift_register.setBit(LED_B);
            break;
        case RGBColorSelect::VIOLET:
            shift_register.setBit(LED_R);
            shift_register.unsetBit(LED_G);
            shift_register.setBit(LED_B);
            break;
        case RGBColorSelect::TURQUOISE:
            shift_register.unsetBit(LED_R);
            shift_register.setBit(LED_G);
            shift_register.setBit(LED_B);
            break;
        case RGBColorSelect::WHITE:
            shift_register.setBit(LED_R);
            shift_register.setBit(LED_G);
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
    
    
    if(this->active_side_state==SideState::change_number_of_leds)
    {
            shift_register.setBit(LED_CH1);
            shift_register.setBit(LED_CH2);
            shift_register.setBit(LED_CH0);
    }
    if(this->active_side_state==SideState::change_speed_of_effect)
    {
            shift_register.unsetBit(LED_CH0);
            shift_register.unsetBit(LED_CH1);
            shift_register.unsetBit(LED_CH2);
    }
    if(this->active_side_state==SideState::change_para0_of_effect)
    {
            shift_register.unsetBit(LED_CH0);
            shift_register.setBit(LED_CH1);
            shift_register.setBit(LED_CH2);
    }
    if(this->active_side_state==SideState::change_para1_of_effect)
    {
            shift_register.setBit(LED_CH0);
            shift_register.setBit(LED_CH1);
            shift_register.unsetBit(LED_CH2);
    }       
    if(this->active_side_state==SideState::change_color1_of_effect)
    {
            shift_register.setBit(LED_CH0);
            shift_register.setBit(LED_CH1);
            shift_register.setBit(LED_CH2);
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
    case DeviceState::OPERATION_FX:
        shift_register.unsetBit(LED_CONST);
        shift_register.setBit(LED_FX);
        shift_register.unsetBit(LED_CHTOGGLE);
        shift_register.unsetBit(LED_REMOTE);
        if(this->active_side_state==SideState::change_speed_of_effect
                ||this->active_side_state==SideState::change_para0_of_effect
                ||this->active_side_state==SideState::change_para1_of_effect
                ||this->active_side_state==SideState::change_color1_of_effect)
        {
            shift_register.setBit(LED_SPEED);
        }
        else
        {
            shift_register.unsetBit(LED_SPEED);
        }
        break;
    case DeviceState::OPERATION_CONST:
        shift_register.setBit(LED_CONST);
        shift_register.unsetBit(LED_FX);
        shift_register.unsetBit(LED_CHTOGGLE);
        shift_register.unsetBit(LED_REMOTE);
        shift_register.unsetBit(LED_SPEED);
        shift_register.unsetBit(LED_EMPTY);
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


    if(this->active_state==DeviceState::OFF)
    {
        shift_register.unsetBit(LED_CONST);
        shift_register.unsetBit(LED_FX);
        shift_register.unsetBit(LED_CHTOGGLE);
        shift_register.unsetBit(LED_REMOTE);
        shift_register.unsetBit(LED_SPEED);
        shift_register.unsetBit(LED_EMPTY);       

        shift_register.unsetBit(LED_CH1);
        shift_register.unsetBit(LED_CH2);
        shift_register.unsetBit(LED_CH0);
        shift_register.unsetBit(LED_R);
        shift_register.unsetBit(LED_G);
        shift_register.unsetBit(LED_B);
    }
    shift_register.transmit();

}


    DeviceState Device::getActiveState()
    {
        return this->active_state;
    }

    void Device::setActiveState(DeviceState state)
    {
        this->active_state=state;
    }

    void Device::setSideState(SideState state)
    {
        this->active_side_state=state;
    }

    SideState Device::getSideState()
    {
        return this->active_side_state;
    }


    void Device::updateAllEffects()
    {
        for(Channel * n : this->channels)
        {
        this->effectmap[n]->incCurentTimeByTick();

        if(n->getEffectEnable())
        {
            n->setRGBChannelData(this->effectmap[n]->getLEDs());
            if(n->getMode()==MODE_DIGITAL)
            {
            n->putDigitalLED();
            }
            else
            {
            n->putRGBChannelData();
            }
        }
    
        }
    }

    void Device::turnOn()
    {
        if(this->active_state==(DeviceState::OFF))
        {
            for(int n=0;n<this->getNumberofChannels();n++)
            {
                channels[n]->setEnable(this->onoff_memory[n]);
            }

                this->active_state=DeviceState::OPERATION;
            
        }
    }

    void Device::turnOff()
    {
        if(!(this->active_state==(DeviceState::OFF)))
        {
        for(int n=0;n<this->getNumberofChannels();n++)
        {
            if(getActiveChannelByID(n)==nullptr)
            {
                break;
            }
            this->onoff_memory[n]=getActiveChannelByID(n)->getEnable();
            getActiveChannelByID(n)->setEnable(false);
        }
        this->active_state=DeviceState::OFF;
        }

    }