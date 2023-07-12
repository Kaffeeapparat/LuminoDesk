#include "shiftregister.hpp"


Shiftregister::Shiftregister(uint8_t latch, uint8_t clk, uint8_t data, uint16_t delay)
    : shiftmask(shiftregisterbitmask), latch_pin(latch), clk_pin(clk), data_pin(data), delay_us(delay)
    {
    this->latch_pin=latch;
    this->clk_pin=clk;
    this->data_pin=data;
    this->delay_us=delay;

    gpio_init(this->latch_pin);
    gpio_init(this->clk_pin);
    gpio_init(this->data_pin);
    gpio_set_dir(this->latch_pin,1);
    gpio_set_dir(this->clk_pin,1);
    gpio_set_dir(this->data_pin,1);

    }
Shiftregister::Shiftregister(): shiftmask(shiftregisterbitmask) {}

void Shiftregister::toggleBit(uint8_t position){
    this->shiftmask^=(1<<position);
}

void Shiftregister::setBit(uint8_t position){
    this->shiftmask|=(1<<position);
}

void Shiftregister::unsetBit(uint8_t position){
    this->shiftmask&=~(1<<position);
}

void Shiftregister::setShiftmask(uint32_t& shiftmask)
{
    this->shiftmask=shiftmask;
}

void Shiftregister::transmit(){
    for(int k=0;k<16;k++)
        {
        gpio_put(this->clk_pin,0);
        if(shiftregisterbitmask & (1<< k)){
                gpio_put(this->data_pin,1);
        } 
        gpio_put(this->clk_pin,1);
        sleep_us(10);
        gpio_put(this->clk_pin,0);
        sleep_us(10);
        gpio_put(this->data_pin,0);
        sleep_us(10);     
    }
                     
    gpio_put(this->latch_pin,1);
    sleep_us(10);
    gpio_put(this->latch_pin,0);  

}

void Shiftregister::init(uint8_t latch, uint8_t clk, uint8_t data, uint16_t delay){
    this->latch_pin=latch;
    this->clk_pin=clk;
    this->data_pin=data;
    this->delay_us=delay;

    gpio_init(this->latch_pin);
    gpio_init(this->clk_pin);
    gpio_init(this->data_pin);
    gpio_init(this->delay_us);
    gpio_set_dir(this->latch_pin,1);
    gpio_set_dir(this->clk_pin,1);
    gpio_set_dir(this->data_pin,1);
    gpio_set_dir(this->delay_us,1);

}