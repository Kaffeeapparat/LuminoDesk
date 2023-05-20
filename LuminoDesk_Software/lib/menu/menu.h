#include "../hardwaredef/hardwaredef.h"
#include "../ledchannel/ledchannel.h"

struct typedef 
{
    uint8_t state;
    
    uint8_t active_channel;
    RGBcolor active_color;

}device_t;

typedef enum {
    init,
    waitforaction,
    effectselsect,
    constant,
    mode,
    remoteisconnected,
    constant,
    keepconnection,
    recievedcommando,


} Mainstate;

typedef enum {
    waitforinput,
    selectchannel,
    rgbselselect,
} Sidestate;

void updateActiveColor();
void updateActiveChannel();