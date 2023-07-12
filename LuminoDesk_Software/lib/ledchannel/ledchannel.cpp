#include "ledchannel.hpp"
#include <iostream>

Channel::Channel(uint8_t id, uint8_t mode, uint8_t voltage, uint8_t mode_signal, uint8_t voltage_signal, uint8_t enable_signal, uint8_t color_r_signal, uint8_t color_g_signal, uint8_t color_b_signal)
    : id(id), mode(mode), voltage(voltage), enable(enable), mode_signal(mode_signal), voltage_signal(voltage_signal), enable_signal(enable_signal), color_r(0), color_g(0), color_b(0), color_r_signal(color_r_signal), color_g_signal(color_g_signal), color_b_signal(color_b_signal), number_of_led(0) {}
Channel::Channel(){}

void Channel::setEnable(bool enable)
{
    this->enable=enable;
    putEnable();
}

bool Channel::getEnable()
{
    return this->enable;
}

void Channel::toggleEnable()
{
    if(this->enable)
    {
        this->enable=false;
    }
    else if(!this->enable)
    {
        this->enable=true;
    }
    putEnable();

}

void Channel::setMode(uint8_t mode)
{
     this->mode=mode;
}

void Channel::setVoltage(uint8_t voltage)
{
    this->voltage=voltage;
}

void Channel::setModeSignal(uint8_t modeSignal)
{
    this->mode_signal=modeSignal;
}

void Channel::setVoltageSignal(uint8_t voltageSignal)
{
    this->voltage_signal=voltageSignal;
}


void Channel::putEnable()
{
    if(this->enable)
    {
        gpio_put(this->enable_signal,1);
    }
    if(!this->enable)
    {
        gpio_put(this->enable_signal,0);
    }
}

uint8_t Channel::getId()
{
    return this->id;
}

void Channel::setRGBChannelData(int32_t colorR, int32_t colorG, int32_t colorB)
{

   this->color_r=colorR;
   this->color_g=colorG;
   this->color_b=colorB;
}

void Channel::setRGBChannelData(RGBColor color, int32_t value)
{
    switch(color) 
    {
        case RGBColor::RED:
            // Handle red color
            this->color_r=value;
            break;
        case RGBColor::GREEN:
            // Handle green color
            this->color_g=value;
            break;
        case RGBColor::BLUE:
            // Handle blue color
            this->color_b=value;
            break;
        default:
            std::cout<<"Unknown color"<< std::endl;
            break;
    }
}


void Channel::putRGBChannelDataPWM()
{


            uint slice_num = pwm_gpio_to_slice_num(this->color_r_signal);
            uint channel_num = pwm_gpio_to_channel(this->color_r_signal);
            pwm_set_chan_level(slice_num, channel_num, this->color_r);
            pwm_set_enabled(slice_num, true);

            slice_num = pwm_gpio_to_slice_num(this->color_g_signal);
            channel_num = pwm_gpio_to_channel(this->color_g_signal);
            pwm_set_chan_level(slice_num, channel_num,this->color_g);
            pwm_set_enabled(slice_num, true);

            slice_num = pwm_gpio_to_slice_num(this->color_b_signal);
            channel_num = pwm_gpio_to_channel(this->color_b_signal);
            pwm_set_chan_level(slice_num, channel_num, this->color_b);
            pwm_set_enabled(slice_num, true);

}
void Channel::putRGBChannelDataPWM(RGBColor color)
{

    switch(color) 
    {
        case RGBColor::RED:
            // Handle red color
        if(this->color_r!=-1){this->color_r=(uint16_t)color_r;}
        if(this->color_r>PWM_WRAP){this->color_r=PWM_WRAP;}
        case RGBColor::GREEN:
            // Handle green color
        if(this->color_g!=-1){this->color_g=(uint16_t)color_g;}
        if(this->color_g>PWM_WRAP){this->color_g=PWM_WRAP;}
        case RGBColor::BLUE:
            // Handle blue color
        if(this->color_b!=-1){this->color_b=(uint16_t)color_b;}
        if(this->color_b>PWM_WRAP){this->color_b=PWM_WRAP;}
            break;
        default:
            std::cout<<"Unknown color"<< std::endl;
            break;
    }
    
}


void Channel::incRGBChannelData(RGBColor color, int32_t value)
{
    if(value>=0)
    {
        if((getRGBChannelData(color)+value)>(PWM_WRAP))
        {
            setRGBChannelData(color,PWM_WRAP);
        }
        setRGBChannelData(color,getRGBChannelData(color)+value);
    }
    else if((((int32_t)getRGBChannelData(color))+value)>0)
    {
        setRGBChannelData(color,getRGBChannelData(color)+value);
    }
    else
    {
        setRGBChannelData(color,0);
    }

    putRGBChannelDataPWM();

}


uint32_t Channel::getRGBChannelData(RGBColor color)
{
    switch (color)
    {
        case RGBColor::RED:
            return this->color_r;
            break;
        case RGBColor::GREEN:
            return this->color_g;
            break;
        case RGBColor::BLUE:
            return this->color_b;
            break;
        default:
            return 0;
            break;
    }
    return 0;
}


void Channel::initChannel()
{

        gpio_init(this->color_r_signal);
        gpio_init(this->color_g_signal);
        gpio_init(this->color_b_signal);
        gpio_init(this->enable_signal);
        gpio_init(this->mode_signal);
        gpio_init(this->voltage_signal);

        gpio_set_dir(this->color_r_signal,1);
        gpio_set_dir(this->color_g_signal,1);
        gpio_set_dir(this->color_b_signal,1);
        gpio_set_dir(this->mode_signal,1);
        gpio_set_dir(this->enable_signal,1);
        gpio_set_dir(this->voltage_signal,1);
  

        gpio_set_function(this->color_r_signal,GPIO_FUNC_PWM);
        uint slice_num = pwm_gpio_to_slice_num(this->color_r_signal);
        pwm_set_wrap(slice_num, PWM_WRAP);

        gpio_set_function(this->color_g_signal,GPIO_FUNC_PWM);
         slice_num = pwm_gpio_to_slice_num(this->color_g_signal);
        pwm_set_wrap(slice_num, PWM_WRAP);

        gpio_set_function(this->color_b_signal,GPIO_FUNC_PWM);
         slice_num = pwm_gpio_to_slice_num(this->color_b_signal);
        pwm_set_wrap(slice_num, PWM_WRAP);
}
