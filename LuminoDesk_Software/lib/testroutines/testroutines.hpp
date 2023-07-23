#ifndef HARDWAREDEF_H
#define HARDWAREDEF_H

#include <inttypes.h>
#include <time.h>
#include "pico/stdlib.h"
#include "pico/stdio.h"
#include "hardware/gpio.h"
#include "pico/cyw43_arch.h"

void msSinceBoot();
void blinkOnce(uint32_t time);
void signalNTimes(uint32_t time_us, int8_t pulses, uint8_t pin);

#endif