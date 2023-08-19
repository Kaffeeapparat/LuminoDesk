#pragma once

#include <cstdint>
#include <ctime>
#include <map>
#include <vector>
#include <cmath>

#include "pico/stdlib.h"
#include "pico/stdio.h"
#include "pico/cyw43_arch.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "hardware/irq.h"
#include "hardware/pwm.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/divider.h"

#include "../testroutines/testroutines.hpp"


/*
*
*/

// Bitmask holding the stream of data for the shift register
extern uint32_t shiftregisterbitmask;

// Shiftmask describes the location of the functions
// in terms of shift register position

// Button matrix
constexpr uint8_t SHIFTMASK_SMR0 = 0;
constexpr uint8_t SHIFTMASK_SMR1 = 1;

// Frontpanel LEDs
constexpr uint8_t LED_CONST = 13;
constexpr uint8_t LED_FX = 7;
constexpr uint8_t LED_EMPTY = 6;
constexpr uint8_t LED_CHTOGGLE = 15;
constexpr uint8_t LED_REMOTE = 14;
constexpr uint8_t LED_SPEED = 5;
constexpr uint8_t LED_R = 12;
constexpr uint8_t LED_B = 10;
constexpr uint8_t LED_G = 11;
constexpr uint8_t LED_CH0 = 2;
constexpr uint8_t LED_CH1 = 4;
constexpr uint8_t LED_CH2 = 3;

// Control signals
constexpr uint8_t CH0_ENB_CHN = 5;
constexpr uint8_t CH1_ENB_CHN = 13;
constexpr uint8_t CH2_ENB_CHN = 14;
constexpr uint8_t CH0_CHG_VLT = 15;
constexpr uint8_t CH1_CHG_VLT = 16;
constexpr uint8_t CH2_CHG_VLT = 17;

// Pin Definitions for the Pico_W Hardware Revision 1

// Button driver
// SMR stands for Switch Matrix Row
constexpr uint SMC1 = 26; // onoff checked
constexpr uint SMC2 = 22; // constant checked
constexpr uint SMC3 = 28; // speed checked
constexpr uint SMC4 = 20; // remote checked
constexpr uint SMC5 = 19; // channelonoffchecked

// Shift register lines
constexpr uint SHIFT_DATA = 18;
constexpr uint SHIFT_CLK = 17;
constexpr uint SHIFT_LATCH = 16;

// LED Channel0
constexpr uint8_t CH0_LED_R = 2;
constexpr uint8_t CH0_LED_G = 3;
constexpr uint8_t CH0_LED_B = 4;
constexpr uint8_t CH0_CHG_MOD = 8;

// LED Channel2
constexpr uint8_t CH1_LED_R = 8;
constexpr uint8_t CH1_LED_G = 8;
constexpr uint8_t CH1_LED_B = 8;
constexpr uint8_t CH1_CHG_MOD = 8;

// LED Channel3
constexpr uint8_t CH2_LED_R = 8;
constexpr uint8_t CH2_LED_G = 8;
constexpr uint8_t CH2_LED_B = 8;
constexpr uint8_t CH2_CHG_MOD = 8;

// Rotary encoder input
constexpr uint8_t QUAD_1 = 14;
constexpr uint8_t QUAD_2 = 15;

// Shiftregister Mask Position
// Position of functions and elements in the shift register mask

// Switchmatrix Columns
constexpr uint8_t SMCR1 = 1;
constexpr uint8_t SMCR2 = 0;

// Channel Parameters
constexpr uint8_t CHANNEL_0 = 0;
constexpr uint8_t CHANNEL_1 = 1;
constexpr uint8_t CHANNEL_2 = 2;

constexpr uint8_t VOLTAGE_LOW = 5;
constexpr uint8_t VOLTAGE_HIGH = 12;

constexpr uint8_t MODE_ANALOG = 0;
constexpr uint8_t MODE_DIGITAL = 1;

/*
* Function Prototypes
*/

void turnOffSignal(uint8_t led);
void turnOnSignal(uint8_t led);

void initGPIO(void);

bool rotaryencoder1_isr(struct repeating_timer *t);
void rotaryencoder1_init();
bool generateTick(struct repeating_timer *t);

// PossibleButtonPresses
enum class ButtonAction {
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
    rotary_short,
    rotary_long,
    dummy
};

enum class RGBColorSelect {
    RED,
    GREEN,
    BLUE,
    VIOLET,
    YELLOW,
    TURQUOISE,
    WHITE
};

struct RGBColor {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

uint8_t RGBcolorReturnColor(RGBColor to_return,RGBColorSelect color);
void RGBcolorSetColor(RGBColor* to_set,RGBColorSelect color,uint8_t to_set_value);
//Holds the selectable Colors on the frontpanel and combines them with an 
//vector of the color compesition. Is done to build an iterator around
//every color combination and set the colors in hardware. 
extern std::map<RGBColorSelect,std::vector<RGBColorSelect>> rgb_color_weights;

struct Encoder {
    int8_t phase1;
    int8_t phase2;
    uint32_t timestampp1;
    uint32_t timestampp2;
    uint32_t laststatep1;
    uint32_t laststatep2;
};

extern int32_t tmpencoderval;

struct Button {
    uint gpio;
    uint8_t row;
    uint32_t timestamp;
    uint32_t debouncelock;
    ButtonAction longpress;
    ButtonAction shortpress;
};

enum class EffectList {
    DISCO,
    GLOW,
    GLOW_MULTI,
    RAINBOW,
    SNAKE,
    BALL,
    RAMP,
    COLORBAND
};



extern Button button_map[2][5];
extern Encoder encoder_map[1];
extern ButtonAction lastinput;
extern int8_t pressed_button_lock;
void checkButtonDebounceLock();

// PWM maximal value
constexpr uint16_t PWM_WRAP = 6250;
//Digital maximal value
constexpr uint16_t DIG_WRAP = 255;

