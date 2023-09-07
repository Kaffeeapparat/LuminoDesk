#include "ledchannel.hpp"
#include <iostream>

Channel::Channel(uint8_t id, uint8_t mode, uint8_t voltage, uint8_t mode_signal, uint8_t voltage_signal, uint8_t enable_signal, uint8_t color_r_signal, uint8_t color_g_signal, uint8_t color_b_signal,Shiftregister * shiftregister,uint8_t pio, uint8_t sm)
    : id(id), mode(mode), voltage(voltage), enable(enable), mode_signal(mode_signal), 
    voltage_signal(voltage_signal), enable_signal(enable_signal),
    color_r_signal(color_r_signal), color_g_signal(color_g_signal), color_b_signal(color_b_signal),
    number_of_led(0),attached_shiftregister(shiftregister),used_pio(pio),loaded_pio_sm(sm)
    {
        this->led_strip_data.resize(1);
        this->is_loaded=false;
        this->led_strip_data[0]={0,0,0};
        this->max_number_of_led=1;
        this->number_of_led=1;
        initChannel();}
Channel::Channel(){}

void Channel::setEnable(bool enable)
{
    this->enable=enable;
    if(this->enable)
    {
        attached_shiftregister->setBit(enable_signal);
    }
    else
    {
        attached_shiftregister->unsetBit(enable_signal);
    }
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

bool Channel::getEffectEnable()
{
    return this->effect_enable;
}
void Channel::setEffectEnable(bool effect_on_off)
{
    this->effect_enable=effect_on_off;
}
void Channel::toggleEffectEnable()
{
    this->effect_enable=!effect_enable;
}


void Channel::toggleEnable()
{
    if(this->enable)
    {
        setEnable(0);
    }
    else if(!this->enable)
    {
        setEnable(1);
    }


}

void Channel::setMode(uint8_t mode)
{
    if(mode == MODE_ANALOG)
    {
        gpio_put(mode_signal,0);
        this->mode=MODE_ANALOG;
        initPWM();
    }
    else
    {
        gpio_put(mode_signal,1);
    this->mode=MODE_DIGITAL;
        initDigital();
    }
}

void Channel::setVoltage(uint8_t voltage)
{
    this->voltage=voltage;
    if(voltage==12)
    {
        attached_shiftregister->setBit(voltage_signal);
    }
    else
    {
        attached_shiftregister->unsetBit(voltage_signal);
    }
}

void Channel::setModeSignal(uint8_t modeSignal)
{
    this->mode_signal=modeSignal;
}

void Channel::setVoltageSignal(uint8_t voltageSignal)
{
    this->voltage_signal=voltageSignal;
}


void Channel::setRGBChannelData(int32_t colorR, int32_t colorG, int32_t colorB)
{

        this->led_strip_data[0].red=colorR;
        this->led_strip_data[0].green=colorG;
        this->led_strip_data[0].blue=colorB;
}

void Channel::setRGBChannelData(RGBColorSelect color, uint8_t value)
{
    switch(color) 
    {
        case RGBColorSelect::RED:
            // Handle red color
            this->led_strip_data[0].red=value;
            break;
        case RGBColorSelect::GREEN:
            // Handle green color
            this->led_strip_data[0].green=value;
            break;
        case RGBColorSelect::BLUE:
            // Handle blue color
            this->led_strip_data[0].blue=value;
            break;
        default:
            std::cout<<"Unknown color"<< std::endl;
            break;
    }
}

void Channel::setRGBChannelData(std::vector<RGBColor> led_vector)
{
    if(this->mode==MODE_DIGITAL)
    {
        this->led_strip_data=led_vector;
    }
    else
    {
        this->led_strip_data[0].red=led_vector[0].red;
        this->led_strip_data[0].green=led_vector[0].green;
        this->led_strip_data[0].blue=led_vector[0].blue;
    }
}


void Channel::putRGBChannelData()
{

        if(getMode()==MODE_ANALOG)
        {
            uint slice_num = pwm_gpio_to_slice_num(this->color_r_signal);
            uint channel_num = pwm_gpio_to_channel(this->color_r_signal);
            pwm_set_chan_level(slice_num, channel_num, led_strip_data[0].red*((uint16_t)(PWM_WRAP/DIG_WRAP)));
            pwm_set_enabled(slice_num, true);

            slice_num = pwm_gpio_to_slice_num(this->color_g_signal);
            channel_num = pwm_gpio_to_channel(this->color_g_signal);
            pwm_set_chan_level(slice_num, channel_num,led_strip_data[0].green*((uint16_t)(PWM_WRAP/DIG_WRAP)));
            pwm_set_enabled(slice_num, true);

            slice_num = pwm_gpio_to_slice_num(this->color_b_signal);
            channel_num = pwm_gpio_to_channel(this->color_b_signal);
            pwm_set_chan_level(slice_num, channel_num, led_strip_data[0].blue*((uint16_t)(PWM_WRAP/DIG_WRAP)));
            pwm_set_enabled(slice_num, true);
        }
        if(getMode()==MODE_DIGITAL)
        {
            putDigitalLED(convertRGBtoWS2812B(this->led_strip_data[0].red,
                                            this->led_strip_data[0].green,
                                            this->led_strip_data[0].blue),this->number_of_led);
        }
}

void Channel::putincRGBChannelData(RGBColorSelect chosen_color, int32_t value)
{
    auto it=rgb_color_weights.find(chosen_color);
        value*=4;

    for(RGBColorSelect color : rgb_color_weights[it->first])
    {
    if(getMode()==MODE_ANALOG)
    {
        if(value>=0)
        {
            if((getRGBChannelData(color))>(DIG_WRAP-value))
            {
                setRGBChannelData(color,DIG_WRAP);
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
            if(value>=0)
            {
                if((getRGBChannelData(color))>(DIG_WRAP-value))
                {
                    setRGBChannelData(color,DIG_WRAP);
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



uint8_t Channel::getRGBChannelData(RGBColorSelect color)
{
    switch (color)
    {
        case RGBColorSelect::RED:
            return this->led_strip_data[0].red;
            break;
        case RGBColorSelect::GREEN:
            return this->led_strip_data[0].green;
            break;
        case RGBColorSelect::BLUE:
            return this->led_strip_data[0].blue;
            break;
        default:
            return 0;
            break;
    }
    return 0;
}

std::vector<RGBColor> Channel::getRGBChannelData()
{
    return this->led_strip_data;
}
RGBColor Channel::getRGBChannelData(uint32_t pos)
{
    return this->led_strip_data[pos];
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
        if(isDigitalCoreloaded())
        {
            unLoadDigitalCore();
        }
    
        gpio_put(this->mode_signal,0);


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
    gpio_put(this->mode_signal,1);
    if(!isDigitalCoreloaded())
    {
    loadDigitalCore();
    }
}

/*
* false to use pio0
* true to use pio1
*/
void Channel::loadDigitalCore()
{
    if(isDigitalCoreloaded())
    {
        return;
    }
    
    if(this->used_pio==0){
        this->loaded_pio=pio0;
    }
    else
    {
        this->loaded_pio=pio1;
    }

    gpio_deinit(this->color_b_signal);
    gpio_deinit(this->color_g_signal);
    gpio_deinit(this->color_r_signal);

    this->loaded_pio_offset= pio_add_program(this->loaded_pio, &ws2812_program);
    this->is_rgbw=false;

    gpio_init(this->color_b_signal);
    gpio_set_dir(this->color_b_signal,1);
    gpio_set_function(this->color_b_signal,GPIO_FUNC_PIO0);
    ws2812_program_init(this->loaded_pio, this->loaded_pio_sm, this->loaded_pio_offset, this->color_b_signal, 800000, is_rgbw);


    gpio_init(this->color_g_signal);
    gpio_init(this->color_r_signal);
    gpio_set_function(this->color_r_signal,GPIO_FUNC_SIO);
    gpio_set_function(this->color_g_signal,GPIO_FUNC_SIO);
    gpio_set_dir(this->color_r_signal,1);
    gpio_set_dir(this->color_g_signal,1);

    gpio_put(this->color_r_signal,1);
    gpio_put(this->color_g_signal,1);
}
void Channel::unLoadDigitalCore()
{
    if(is_loaded){
    pio_remove_program(this->loaded_pio,&ws2812_program,this->loaded_pio_offset);
    this->is_loaded=false;
    this->loaded_pio=NULL;
    this->loaded_pio_offset=0;
    }


}


uint32_t Channel::convertRGBtoWS2812B(uint8_t r, uint8_t g, uint8_t b)
{
    uint32_t out= 0;
    out=((uint32_t) (g)<<16)|((uint32_t) (r)<<8)|((uint32_t)(b));
    return out;
}

void Channel::putDigitalLED()
{
    for(RGBColor c : this->led_strip_data)
    {
        putDigitalLED(convertRGBtoWS2812B(c.red,c.green,c.blue));
    }
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
        setMaxNumbersOfLeds(led_number);
    }
}

uint32_t Channel::getNumberOfLeds()
{
    return this->number_of_led;
}

void Channel::setMaxNumbersOfLeds(uint32_t led_number)
{
    if(this->max_number_of_led<led_number)
    {
        this->max_number_of_led=led_number;
    }
}

uint32_t Channel::getMaxNumberOfLeds()
{
    return this->max_number_of_led;
}