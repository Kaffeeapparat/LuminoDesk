#include "testroutines.h"
#include <stdio.h>

//Testroutine for checking the internal timing of the pico
void msSinceBoot(){
    uint32_t meassure=0;
    uint32_t last_meassure=0;
    gpio_init(1);
    gpio_set_dir(1,1);
    gpio_put(1,1);
    sleep_ms(1000);
    while(1==1){
        meassure=to_ms_since_boot (get_absolute_time());
        printf("meassure: %d\nlastmeassure: %d\n\n\n",meassure,last_meassure);

        if(((meassure%2)==0) ){
            gpio_put(1,1);
            last_meassure=meassure;
        }
        if(((meassure%2)!=0) ){
            gpio_put(1,0);
            last_meassure=meassure;
        }
        
    }
}

void blinkOnce(uint32_t ms){
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
    sleep_ms(ms);
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
    sleep_ms(ms);
}

void signalNTimes(uint32_t time_us,int8_t pulses, uint8_t pin)
{
    gpio_init(pin);
    gpio_set_dir(pin,1);
    
    do{
    gpio_put(pin,1);
    sleep_us(time_us);
    gpio_put(pin,0);
    sleep_us(time_us);
    pulses--;
    }while(pulses>0);
}