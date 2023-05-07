#include <inttypes.h>
#include <time.h>
#include "pico/stdlib.h"
#include "pico/stdio.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "hardware/irq.h"
#include "pico/cyw43_arch.h"

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
#define LED_CH1         2
#define LED_CH2         4
#define LED_CH3         3

//Contrtollsignals
#define CH1_ENB_CHN     12
#define CH2_ENB_CHN     13
#define CH3_ENB_CHN     14
#define CH1_CHG_VLT     15
#define CH2_CHG_VLT     16
#define CH3_CHG_VLT     17

//Pin Definitions for the Pico_W Hardware Revision 1

//Button driver
//SMR stands for Switch Matrix Row
#define SMC1 26 //onoff checked
#define SMC2 22 //constant checked
#define SMC3 28  //speed checked
#define SMC4 20 //remote checked
#define SMC5 21  //channelonoffchecked

//Shiftregisterlines

#define SHIFT_DATA      18
#define SHIFT_CLK       17
#define SHIFT_LATCH     16

//LED Channel1 

#define CH1_LED_R
#define CH1_LED_G
#define CH1_LED_B
#define CH1_CHG_MOD

//LED Channel2

#define CH2_LED_R
#define CH2_LED_G
#define CH2_LED_B
#define CH2_CHG_MOD

//LED Channel3

#define CH3_LED_R
#define CH3_LED_G
#define CH3_LED_B
#define CH3_CHG_MOD

//Rotary encoder input

#define QUAD_1  27
#define QUAD_2  28

//Shiftregister Mask Position
//Position of functions and elements in the chift register mask

//Switchmatrix Collums

//Switch Matrix Columm
#define SMCR1            1
#define SMCR2            0






/*
* Function Prototypes
*/

void turnOffSignal(uint8_t led);
void turnOnSignal(uint8_t led);


// CH3_ENB_CHN
//Channel 1-3
//Mode MODE_ANALOG or MODE_DIGITAL
void changeModeChannel(uint8_t channel, uint8_t mode);

// CH3_CHG_VLT
//Channel 1-3
//Voltage V_LOW or V_HIGH
void changeVoltageChannel(uint8_t channel, uint8_t voltage);

#define CHANNEL_1 1
#define CHANNEL_2 2
#define CHANNEL_3 3

#define VOLTAGE_LOW 5
#define VOLTAGE_HIGH 12

#define MODE_ANALOG  0
#define MODE_DIGITAL 1

void initGPIO(void);

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
    uint gpio;
    uint8_t row;
    uint32_t timestamp;
    ButtonAction longpress;
    ButtonAction shortpress;
}button_t;


extern button_t button_map[2][5];
extern ButtonAction lastinput;
extern int8_t pressed_button_lock;

void blinkOnce(uint32_t ms);