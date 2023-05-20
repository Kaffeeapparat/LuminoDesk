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
        case red:
            instance->active_color=green;
            break;
        case green:
            instance->active_color=blue;
            break;
        case blue:
            instance->active_color=red;
            break;
    }

}