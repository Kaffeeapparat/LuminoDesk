#include "testroutines.hpp"

#include <cstdio>
#include <chrono>

#include "pico/stdio.h"

void msSinceBoot()
{
    uint32_t measure=0;
    uint32_t last_measure=0;
    gpio_init(1);
    gpio_set_dir(1,GPIO_OUT);
    gpio_put(1,1);
    sleep_ms(1000);
    while(true)
    {
        measure=to_ms_since_boot(get_absolute_time());
        printf("measure:%d\nlast_measure:%d\n\n\n",measure,last_measure);
        if((measure%2)==0){
            gpio_put(1,1);
            last_measure=measure;
        }
        if((measure%2)!=0){
            gpio_put(1,0);
            last_measure=measure;
        }
    }
}

void blinkOnce(uint32_t ms)
{
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN,1);
    sleep_ms(ms);
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN,0);
    sleep_ms(ms);
}

void signalNTimes(uint32_t time_us,int8_t pulses,uint8_t pin)
{
    gpio_init(pin);
    gpio_set_dir(pin,GPIO_OUT);
    do{
        gpio_put(pin,1);
        busy_wait_us(time_us);
        gpio_put(pin,0);
        busy_wait_us(time_us);
        pulses--;
    }while(pulses>0);
}