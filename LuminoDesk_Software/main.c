#include <stdio.h>
#include "lib/hardwaredef/hardwaredef.h"
#include "lib/testroutines/testroutines.h"

//extern char* button_map[2][5];
extern uint32_t shiftregisterbitmask;
extern ButtonAction lastinput;
extern int8_t pressed_button_lock;
extern uint8_t tick;

bool generateTick(struct repeating_timer *t);
void printLastInput();
void updateShiftregister();


int main()
{
    stdio_init_all();
    if (cyw43_arch_init()) {
        printf("WiFi init failed");
        gpio_init(1);
        gpio_set_dir(1,1);
        gpio_put(1,1);
        return -1;
    }
    initGPIO();
    shiftregisterbitmask ^=(1<<SHIFTMASK_SMR0);
    
    //set alarm to update the shiftregister routinely
    struct repeating_timer timer;
    add_repeating_timer_ms(100, generateTick, NULL, &timer);
    while(1==1){
        tight_loop_contents();

        if(tick==(10||30||50||70||90));
        {
            if(!pressed_button_lock){
                shiftregisterbitmask^=((1<<SHIFTMASK_SMR0)|(1<<SHIFTMASK_SMR1));
            }
            updateShiftregister();
            while(tick==10)
            {
                tight_loop_contents();
            }
        }
        if(tick==0)
        {
            printLastInput();
            while(tick==0)
            {
                tight_loop_contents();
            }
        }

        
    }
    
    /*
    while (1)
    {
        tight_loop_contents();
        printf("hello word\n"); 
        sleep_ms(100);
        blinkOnce(2999);
        }
    */

    return 0;    
}

bool generateTick(struct repeating_timer *t)
{


    if(tick<100){
        tick++;
    }
    else{
        tick=0;
        if(cyw43_arch_gpio_get(CYW43_WL_GPIO_LED_PIN)){
            cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN,0);
        }
        else{
            cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN,1);
        }
    }
    return true;
} 

void updateShiftregister() {


            //2.Vorschleife Clocksignale geben u. Ausgang
            
    for(int k=0;k<16;k++)
        {
        gpio_put(SHIFT_CLK,0);
        
        
        if(shiftregisterbitmask & (1<< k)){
            if(pressed_button_lock && ((k==SHIFTMASK_SMR0)||(k==SHIFTMASK_SMR1))){
            tight_loop_contents();
            }
                gpio_put(SHIFT_DATA,1);
            }
            
        gpio_put(SHIFT_CLK,1);
        busy_wait_us(10);
        gpio_put(SHIFT_CLK,0);
        busy_wait_us(10);
        gpio_put(SHIFT_DATA,0);
        busy_wait_us(10);
                
    }
                     
            gpio_put(SHIFT_LATCH,1);
            busy_wait_us(10);
            gpio_put(SHIFT_LATCH,0);  
}

void printLastInput(){

    printf("\n\nLast input was:");
    switch (lastinput) {
        case onoff_long:
            printf("onoff_long");
            break;
        case onoff_short:
            printf("onoff_short");
            break;
        case constant_long:
            printf("constant_long");
            break;
        case constant_short:
            printf("constant_short");
            break;
        case remote_long:
            printf("remote_long");
            break;
        case remote_short:
            printf("remote_short");
            break;
        case channelonoff_short:
            printf("channelonoff_short");
            break;
        case channelonoff_long:
            printf("channelonoff_long");
            break;
        case effect_long:
            printf("effect_long");
            break;
        case effect_short:
            printf("effect_short");
            break;
        case speed_long:
            printf("speed_long");
            break;
        case speed_short:
            printf("speed_short");
            break;
        case rgbs_short:
            printf("rgbs_short");
            break;
        case rgbs_long:
            printf("rgbs_long");
            break;
        case channels_short:
            printf("channels_short");
            break;
        case channels_long:
            printf("channels_long");
            break;
        case empty_long:
            printf("empty_long");
            break;
        case empty_short:
            printf("empty_short");
            break;
        case dummy:
            printf("dummy");
            break;
        default:
            printf("Unknown ButtonAction");
            break;
    }
    printf("\n");
}

