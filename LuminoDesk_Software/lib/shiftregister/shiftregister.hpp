#pragma once

#include "../hardwaredef/hardwaredef.hpp"
#include "hardware/gpio.h"

class Shiftregister{
private:
    uint32_t& shiftmask;
    uint8_t latch_pin;
    uint8_t clk_pin;
    uint8_t data_pin;
    uint16_t delay_us;

public:
    Shiftregister();
    Shiftregister(uint8_t latch, uint8_t clk, uint8_t data, uint16_t delay);
    
    void toggleBit(uint8_t position);
    void setBit(uint8_t position);
    void unsetBit(uint8_t position);
    void transmit();
    void setShiftmask(uint32_t& shiftmask);
    void init(uint8_t latch, uint8_t clk, uint8_t data, uint16_t delay);
};
