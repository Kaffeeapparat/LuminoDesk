#include "ledchannel.hpp"
#include <iostream>

Channel::Channel(uint8_t id, uint8_t mode, uint8_t voltage, uint8_t modeSignal, uint8_t voltageSignal, uint8_t enableSignal, uint8_t colorRSignal, uint8_t colorGSignal, uint8_t colorBSignal)
    : id(id), mode(mode), voltage(voltage), enable(enable), mode_signal(mode_signal), voltage_signal(voltage_signal), enable_signal(enable_signal), color_r(color_r), color_g(color_g), color_b(color_b), color_r_signal(color_r_signal), color_g_signal(color_g_signal), color_b_signal(colorBSignal) {}


void Channel::setEnable(bool enable)
{
    this->enable=enable;
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
    if(this->enable){gpio_put(this->enable,1);}
    if(!this->enable){gpio_put(this->enable,0);}
}

void Channel::setRGBChannelData(int32_t colorR, int32_t colorG, int32_t colorB)
{
    if(this->color_r!=-1){this->color_r=(uint16_t)color_r;}
    if(this->color_r>PWM_WRAP){this->color_r=PWM_WRAP;}

    if(this->color_g!=-1){this->color_g=(uint16_t)color_g;}
    if(this->color_g>PWM_WRAP){this->color_g=PWM_WRAP;}

    if(this->color_b!=-1){this->color_b=(uint16_t)color_b;}
    if(this->color_b>PWM_WRAP){this->color_b=PWM_WRAP;}
}


void Channel::setRGBChannelData(RGBColor color, int32_t value)
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

void Channel::initChannel()
{

        gpio_init(this->color_r_signal);
        gpio_init(this->color_g_signal);
        gpio_init(this->color_b_signal);
        gpio_init(this->mode_signal);

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
