#include "hardwaredef.h"

char* button_map[2][5]={{"effect","empty","0","channelS","rgbS"},{"power","const","channelonoff","remote","speed"}};
uint32_t shiftregisterbitmask=0;

void turnOnLed(uint8_t led)
{
    shiftregisterbitmask |= (1 << led);
}

void turnOffLed(uint8_t led)
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

    void changeVoltageChannel(uint8_t channel, uint8_t voltage)
    {
        // Implement code to change the voltage of the specified channel
        // using the given voltage (V_LOW or V_HIGH)
    }
}

void initGPIO(void){

    gpio_init(SHIFT_CLK);
    gpio_set_dir(SHIFT_CLK, GPIO_OUT);

    gpio_init(SHIFT_DATA);
    gpio_set_dir(SHIFT_DATA, GPIO_OUT);

    gpio_init(SHIFT_LATCH);
    gpio_set_dir(SHIFT_LATCH, GPIO_OUT);

    int inputpins[]={SMC1,SMC2,SMC3,SMC4,SMC5,QUAD_1,QUAD_2};

    // Set GPIO 0-7 as inputs with pullup resistors and hardware IRQs
    for (int i = 0; i < 7; i++) {
        gpio_init(inputpins[i]);
        gpio_set_dir(inputpins[i], GPIO_IN);
        gpio_pull_up(inputpins[i]);

        //gpio_set_irq_enabled_with_callback(i, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
 }

}