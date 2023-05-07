#include <inttypes.h>
#include <time.h>
#include "pico/stdlib.h"
#include "pico/stdio.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "hardware/irq.h"
#include "pico/cyw43_arch.h"

void msSinceBoot();