#include "menu.h"

void updateActiveChannel(device_t* instance){

    if(instance->active_channel==NUMBER_OF_CHANNELS-1)
    {
        instance->active_channel=0;
    }
    else
    {
        instance->active_channel++;
    }
}

void updateActiveColor(device_t* instance ){

        switch (instance->active_color) {
        case RED:
            instance->active_color=GREEN;
            break;
        case GREEN:
            instance->active_color=blue;
            break;
        case BLUE:
            instance->active_color=RED;
            break;
        case RED:
            instance->active_color=VIOLET;
            break;
        case VIOLET:
            instance->active_color=TURQUOISE;
            break;
        case TURQUOISE:
            instance->active_color=WHITE;
            break;
        case WHITE:
            instance->active_color=RED;
            break;
    }

}