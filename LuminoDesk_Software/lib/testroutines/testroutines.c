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