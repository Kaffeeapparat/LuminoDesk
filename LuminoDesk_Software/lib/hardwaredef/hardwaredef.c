#include "hardwaredef.h"
#include <stdio.h>


//All "private" function protoypes
void button_isr(uint gpio, uint32_t events);

//char* button_map[2][5]={{"effect","empty","0","channelS","rgbS"},{"power","const","channelonoff","remote","speed"}};
uint8_t tick;
uint32_t shiftregisterbitmask=0;
ButtonAction lastinput;


int inputpins[]={SMC1,SMC2,SMC3,SMC4,SMC5,QUAD_1,QUAD_2};


button_t button_map[2][5] ={
    {
    {SMC1,SMCR1,0, onoff_long, onoff_short},
    {SMC2,SMCR1,0, constant_long, constant_short},
    {SMC5,SMCR1,0, channelonoff_long, channelonoff_short},
    {SMC3,SMCR1,0, speed_long, speed_short},
    {SMC4,SMCR1,0, remote_long, remote_short},
    },{
    {SMC1,SMCR2,0, effect_long, effect_short},
    {SMC2,SMCR2,0, empty_long, empty_short},
    {SMC3,SMCR2,0,dummy,dummy},
    {SMC4,SMCR2,0, rgbs_long, rgbs_short},
    {SMC5,SMCR2,0, channels_long, channels_short},
}};
const int num_buttons = sizeof(button_map) / sizeof(button_t);



void turnOnSignal(uint8_t led)
{
    shiftregisterbitmask |= (1 << led);
}

void turnOffSignal(uint8_t led)
{
    shiftregisterbitmask &= ~(1 << led);
}


void changeModeChannel(uint8_t channel, uint8_t mode)
{
    if (channel == CHANNEL_1)
    {
        if (channel == CHANNEL_3)
        {
            if (mode == MODE_DIGITAL)
            {
            }
            if (mode == MODE_ANALOG)
            {
            }
        }
    }
    if (channel == CHANNEL_2)
    {
        if (channel == CHANNEL_3)
        {
            if (mode == MODE_DIGITAL)
            {
            }
            if (mode == MODE_ANALOG)
            {
            }
        }
        if (channel == CHANNEL_3)
        {
            if (mode == MODE_DIGITAL)
            {
            }
            if (mode == MODE_ANALOG)
            {
            }
        }
    }
}

void changeVoltageChannel(uint8_t channel, uint8_t voltage)
    {
        // Implement code to change the voltage of the specified channel
        // using the given voltage (V_LOW or V_HIGH)
    }

void initGPIO(void){

    gpio_init(SHIFT_CLK);
    gpio_set_dir(SHIFT_CLK, GPIO_OUT);
blinkOnce(125);
    gpio_init(SHIFT_DATA);
    gpio_set_dir(SHIFT_DATA, GPIO_OUT);
blinkOnce(125);

    gpio_init(SHIFT_LATCH);
    gpio_set_dir(SHIFT_LATCH, GPIO_OUT);


    button_t buttonpresses[10];

    // Set GPIO 0-7 as inputs with pullup resistors and hardware IRQs
    for (int i = 0; i < 6; i++) {
        blinkOnce(125);
        gpio_init(inputpins[i]);
        gpio_set_dir(inputpins[i], GPIO_IN);
        gpio_pull_up(inputpins[i]);
        //gpio_add_raw_irq_handler(inputpins[i],button_isr);
         gpio_set_irq_enabled_with_callback(inputpins[i],GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &button_isr);

    }   

    //Enable GPIO Interrupts
        //Populate buttonpresses

}

//Variable definition for button presses

//If one button is in evaluation another buttonpress is ignored
//If -1 a buttonpress is in evaluation
int8_t pressed_button_lock=0;
uint32_t pressed_button_time;

button_t pressed_button;



const uint32_t debounce_threshold =50;
const uint32_t longpress_threshold=3000;
const uint32_t failure_threshold=10000;


void button_isr(uint gpio, uint32_t events)
{
    //Needs to be set correctly when checking the switch matrix

    //GPIO Interrupt triggers and pressed_button_lock is free
    //Bad for debouncing
    int i=0;
    int pos;
    int row;
    if((shiftregisterbitmask >> SHIFTMASK_SMR0)  & 0x01)
    {
        row=1;
    }
    if((shiftregisterbitmask >> SHIFTMASK_SMR1)  & 0x01)
    {
        row=0;
    }


    for(int j=0;j<5;j++){
        if(((uint)button_map[row][j].gpio)==gpio){
            pos=j;
            printf("Does this match?\nbutton_map[1][%d].%zu==buttonmap[1][%zu].%zu\n",pos,button_map[row][pos].gpio,pos,gpio);
            printf("found pair at gpio %zu and pos %zu\n",gpio,j);
            }
    }
    i=pos;
    uint32_t current_time=to_ms_since_boot (get_absolute_time());

    if(GPIO_IRQ_EDGE_FALL==events)
    
{

                printf("\n\n----PRESSED:\nButton  is Absolute Time %zu\n",button_map[row][pos].timestamp);

            gpio_acknowledge_irq(gpio, GPIO_IRQ_EDGE_FALL);
            cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
            pressed_button_lock=1;

            button_map[row][pos].timestamp=current_time;
            

        }
    
    //Routines to handle the raising edge
    if(GPIO_IRQ_EDGE_RISE==events)
    {

        gpio_acknowledge_irq(gpio, GPIO_IRQ_EDGE_FALL);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        uint32_t deltat= current_time-button_map[row][i].timestamp;
        printf("\n\n----RAISED:\nButton  is Absolute Time %zu\n",button_map[row][pos].timestamp);
        printf("Time: %zu ",deltat);

        if(deltat<=debounce_threshold)
        {
            tight_loop_contents();
        }
        
        else if(deltat>=failure_threshold)
        {
            button_map[row][pos].timestamp=0;
    pressed_button_lock=0;

        }
        else if (deltat>longpress_threshold)
        {

            button_map[row][pos].timestamp=0;
            lastinput=button_map[row][pos].longpress;
            pressed_button_lock=0;


            
        }
        else if(deltat<longpress_threshold)
        {
            button_map[row][pos].timestamp=0;
            lastinput=button_map[row][pos].shortpress;
        pressed_button_lock=0;
        }
    }
     

}
    void blinkOnce(uint32_t ms){
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        sleep_ms(ms);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        sleep_ms(ms);
    }

    void handle_button_event(){



        
    }