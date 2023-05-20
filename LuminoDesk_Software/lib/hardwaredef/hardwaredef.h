#ifndef HARDWAREDEF_H
#define HARDWAREDEF_H

#include <inttypes.h>
#include <time.h>

#include "pico/stdlib.h"
#include "pico/stdio.h"
#include "pico/cyw43_arch.h"

#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "hardware/irq.h"
#include "hardware/pwm.h"


/*
*
*/

//Bitmask holding the stream of data for the shift register
extern uint32_t shiftregisterbitmask;


//Shiftmask describes the location of the functions
//in terms of shift register postition

//Buttonmatrix
#define SHIFTMASK_SMR0 0
#define SHIFTMASK_SMR1 1

//Frontpanel LEDs

#define LED_CONST       13
#define LED_FX          7
#define LED_EMPTY       6
#define LED_CHTOGGLE    15
#define LED_REMOTE      14
#define LED_SPEED       5
#define LED_R           12
#define LED_B           10
#define LED_G           11
#define LED_CH0         2
#define LED_CH1         4
#define LED_CH2         3

//Contrtollsignals
#define CH0_ENB_CHN     12
#define CH1_ENB_CHN     13
#define CH2_ENB_CHN     14
#define CH0_CHG_VLT     15
#define CH1_CHG_VLT     16
#define CH2_CHG_VLT     17

//Pin Definitions for the Pico_W Hardware Revision 1

//Button driver
//SMR stands for Switch Matrix Row
#define SMC1 26 //onoff checked
#define SMC2 22 //constant checked
#define SMC3 28  //speed checked
#define SMC4 20 //remote checked
#define SMC5 19  //channelonoffchecked

//Shiftregisterlines

#define SHIFT_DATA      18
#define SHIFT_CLK       17
#define SHIFT_LATCH     16

//LED Channel0

#define CH0_LED_R   0
#define CH0_LED_G   0
#define CH0_LED_B   0
#define CH0_CHG_MOD 0

//LED Channel2

#define CH1_LED_R   0
#define CH1_LED_G   0
#define CH1_LED_B   0
#define CH1_CHG_MOD 0

//LED Channel3

#define CH2_LED_R   0
#define CH2_LED_G   0
#define CH2_LED_B   0
#define CH2_CHG_MOD 0


//Rotary encoder input

#define QUAD_1  14
#define QUAD_2  15

//Shiftregister Mask Position
//Position of functions and elements in the chift register mask

//Switchmatrix Collums

//Switch Matrix Columm
#define SMCR1            1
#define SMCR2            0

//Channel Parameters
#define CHANNEL_0 0
#define CHANNEL_1 1
#define CHANNEL_2 2

#define VOLTAGE_LOW 5
#define VOLTAGE_HIGH 12

#define MODE_ANALOG  0
#define MODE_DIGITAL 1




/*
* Function Prototypes
*/

void turnOffSignal(uint8_t led);
void turnOnSignal(uint8_t led);

void initGPIO(void);

bool rotaryencoder1_isr();
void rotaryencoder1_init();

//PossibleButtonPresses
typedef enum {
    onoff_long,
    onoff_short,
    constant_long,
    constant_short,
    remote_long,
    remote_short,
    channelonoff_short,
    channelonoff_long,
    effect_long,
    effect_short,
    speed_long,
    speed_short,
    rgbs_short,
    rgbs_long,
    channels_short,
    channels_long,
    empty_long,
    empty_short,
    dummy
} ButtonAction;

typedef struct{
    //GPIOs of both phases

    //Phase 1 is the actual indent
    int8_t phase1;
    //phase two represents the directional component
    int8_t phase2;
    
    //Timestamps for debouncing
    uint32_t timestampp1;
    uint32_t timestampp2;

    //Saved States for both phases
    uint32_t laststatep1;
    uint32_t laststatep2;
}encoder_t;

extern int32_t tmpencoderval;


typedef struct{
    uint gpio;
    uint8_t row;
    uint32_t timestamp;
    //After the button was sucessfully pressed, this time constant blocks relasing debounce
    uint32_t debouncelock;
    ButtonAction longpress;
    ButtonAction shortpress;
}button_t;


extern button_t button_map[2][5];
extern encoder_t encoder_map[1];
extern ButtonAction lastinput;
extern int8_t pressed_button_lock;


//PWM Constants

//PWM Wrap for core frequency of 125Mhz and a PWM frequency of 20khz

#define PWM_WRAP 6250

#endif