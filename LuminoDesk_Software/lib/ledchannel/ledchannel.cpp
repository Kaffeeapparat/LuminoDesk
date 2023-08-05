#include "ledchannel.hpp"
#include <iostream>

Channel::Channel(uint8_t id, uint8_t mode, uint8_t voltage, uint8_t mode_signal, uint8_t voltage_signal, uint8_t enable_signal, uint8_t color_r_signal, uint8_t color_g_signal, uint8_t color_b_signal)
    : id(id), mode(mode), voltage(voltage), enable(enable), mode_signal(mode_signal), voltage_signal(voltage_signal), enable_signal(enable_signal), color_r(0), color_g(0), color_b(0), color_r_signal(color_r_signal), color_g_signal(color_g_signal), color_b_signal(color_b_signal), number_of_led(0)
    {
        this->is_loaded=false;
        this->max_number_of_led=0;
        initChannel();}
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
uint8_t Channel::getMode()
{
    return this->mode;
}
uint8_t Channel::getVoltage()
{
    return this->voltage;
}
uint8_t Channel::getId()
{
    return this->id;
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
    if(mode == MODE_ANALOG)
    {

    }
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

void Channel::setRGBChannelData(int32_t colorR, int32_t colorG, int32_t colorB)
{

   this->color_r=colorR;
   this->color_g=colorG;
   this->color_b=colorB;
}

void Channel::setRGBChannelData(RGBColorSelect color, int32_t value)
{
    switch(color) 
    {
        case RGBColorSelect::RED:
            // Handle red color
            this->color_r=value;
            break;
        case RGBColorSelect::GREEN:
            // Handle green color
            this->color_g=value;
            break;
        case RGBColorSelect::BLUE:
            // Handle blue color
            this->color_b=value;
            break;
        default:
            std::cout<<"Unknown color"<< std::endl;
            break;
    }
}


void Channel::putRGBChannelData()
{

        if(getMode()==MODE_ANALOG)
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
        if(getMode()==MODE_DIGITAL)
        {
            putDigitalLED(convertRGBtoWS2812B(this->color_r,this->color_g,this->color_b),this->number_of_led);
        }
}
void Channel::putRGBChannelData(RGBColorSelect color)
{

    if(getMode()==MODE_ANALOG)
    {
    switch(color) 
    {
        case RGBColorSelect::RED:
            // Handle red color
            if(this->color_r!=-1){this->color_r=(uint16_t)color_r;}
            if(this->color_r>PWM_WRAP){this->color_r=PWM_WRAP;}
            break;
        case RGBColorSelect::GREEN:
            // Handle green color
            if(this->color_g!=-1){this->color_g=(uint16_t)color_g;}
            if(this->color_g>PWM_WRAP){this->color_g=PWM_WRAP;}
            break;
        case RGBColorSelect::BLUE:
            // Handle blue color
            if(this->color_b!=-1){this->color_b=(uint16_t)color_b;}
            if(this->color_b>PWM_WRAP){this->color_b=PWM_WRAP;}
            break;
        default:
            std::cout<<"Unknown color"<< std::endl;
            break;
    }
    }
}


void Channel::putincRGBChannelData(RGBColorSelect chosen_color, int32_t value)
{
    auto it=rgb_color_weights.find(chosen_color);

    for(RGBColorSelect color : rgb_color_weights[it->first])
    {
    if(getMode()==MODE_ANALOG)
    {
        value*=45;
        if(value>=0)
        {
            if((getRGBChannelData(color)+value)>(PWM_WRAP))
            {
                setRGBChannelData(color,PWM_WRAP);
            }
            else{
            setRGBChannelData(color,getRGBChannelData(color)+value);
            }
        }
        else if((((int32_t)getRGBChannelData(color))+value)>0)
        {
            setRGBChannelData(color,getRGBChannelData(color)+value);
        }
        else
        {
            setRGBChannelData(color,0);
        }
    }
        else if(getMode()==MODE_DIGITAL)
        {
            value*=4;
            if(value>=0)
            {
                if((getRGBChannelData(color)+value)>(DIG_WRAP-2))
                {
                    setRGBChannelData(color,DIG_WRAP-2);
                }
                else
                {
                setRGBChannelData(color,getRGBChannelData(color)+value);
                }
            }
            else if((((int32_t)getRGBChannelData(color))+value)>0)
            {
                setRGBChannelData(color,getRGBChannelData(color)+value);
            }
            else
            {
                setRGBChannelData(color,0);
            }    
    
        }
    
    }
    putRGBChannelData();
}



uint32_t Channel::getRGBChannelData(RGBColorSelect color)
{
    switch (color)
    {
        case RGBColorSelect::RED:
            return this->color_r;
            break;
        case RGBColorSelect::GREEN:
            return this->color_g;
            break;
        case RGBColorSelect::BLUE:
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

        if(mode==MODE_ANALOG)
        {
            initPWM();
        }
        if(mode==MODE_DIGITAL)
        {
            initDigital();
        }
        else
        {
            printf("Error no Mode For Channel %d!\n",getId());
        }
}

void Channel::initPWM()
{

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

void Channel::initDigital()
{
    if(!isDigitalCoreloaded())
    {
    loadDigitalCore(0);
    }
}

/*
* false to use pio0
* true to use pio1
*/
void Channel::loadDigitalCore(uint8_t pio_select)
{
    if(isDigitalCoreloaded())
    {
        return;
    }
    PIO pio;
    if(pio_select==0){
        pio=pio0;
    }
    else
    {
        pio=pio1;
    }
    uint sm=0;
    uint offset= pio_add_program(pio, &ws2812_program);
    this->is_rgbw=false;
    ws2812_program_init(pio, sm, offset, color_r_signal, 800000, is_rgbw);
    this->is_loaded=true;
    this->loaded_pio=pio;
    this->loaded_pio_offset=offset;
    this->loaded_pio_sm=sm;
}
void Channel::unLoadDigitalCore()
{
    if(is_loaded){
    pio_remove_program(this->loaded_pio,&ws2812_program,this->loaded_pio_offset);
    this->is_loaded=false;
    this->loaded_pio=NULL;
    this->loaded_pio_offset=0;
    this->loaded_pio_sm=0;
    }
}


uint32_t Channel::convertRGBtoWS2812B(uint8_t r, uint8_t g, uint8_t b)
{
    uint32_t out= 0;
    out=((uint32_t) (g)<<16)|((uint32_t) (r)<<8)|((uint32_t)(b));
    return out;
}

void Channel::putDigitalLED(uint32_t data, uint16_t n)
{
    if(n>this->max_number_of_led)
    {
        this->max_number_of_led=n;
    }
    irq_set_enabled(IO_IRQ_BANK0,0);
    irq_set_enabled(TIMER_IRQ_0,0);
    irq_set_enabled(TIMER_IRQ_1,0);
    for (uint16_t i=0;i<n;i++)
    {
        pio_sm_put_blocking(this->loaded_pio,this->loaded_pio_sm,(data)<<8u);
    }
    if(n<this->max_number_of_led)
    {
        for(uint16_t i=n;i<this->max_number_of_led;i++)
        {
            pio_sm_put_blocking(this->loaded_pio,this->loaded_pio_sm,(uint16_t)0);
        }
    }
    irq_set_enabled(IO_IRQ_BANK0,1);
    irq_set_enabled(TIMER_IRQ_0,1);
    irq_set_enabled(TIMER_IRQ_1,1);
}

void Channel::putDigitalLED(uint32_t data)
{
    irq_set_enabled(IO_IRQ_BANK0,0);
    irq_set_enabled(TIMER_IRQ_0,0);
    irq_set_enabled(TIMER_IRQ_1,0);

    pio_sm_put_blocking(this->loaded_pio,this->loaded_pio_sm,(data)<<8u);

    irq_set_enabled(IO_IRQ_BANK0,1);
    irq_set_enabled(TIMER_IRQ_0,1);
    irq_set_enabled(TIMER_IRQ_1,1);
    return;
}

bool Channel::isDigitalCoreloaded(){
    return this->is_loaded;
}

void Channel::setNumberOfLeds(uint32_t led_number)
{
    if(((int32_t)this->number_of_led+(int32_t)led_number)<0)
    {
        this->number_of_led=0;
    }
    else{
        this->number_of_led=led_number;
    }
}

uint32_t Channel::getNumberOfLeds()
{
    return number_of_led;
}