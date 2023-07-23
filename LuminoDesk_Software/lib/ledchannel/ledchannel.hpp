#ifndef LEDCHANNELDEF_H
#define LEDCHANNELDEF_H

#include "../hardwaredef/hardwaredef.hpp"
#include "generated/ws2812.pio.h"
#include <vector>

enum class RGBColor {
    RED,
    GREEN,
    BLUE
};

class Channel {
public:
    Channel();
    Channel(uint8_t id, uint8_t mode, uint8_t voltage, uint8_t modeSignal, uint8_t voltageSignal, uint8_t enableSignal, uint8_t colorRSignal, uint8_t colorGSignal, uint8_t colorBSignal);


    //Inits the pins of the Device
    void initChannel();

    //Set the Channel enable or Disable
    void setEnable(bool enable);
    void putEnable();
    void toggleEnable();

    void setMode(uint8_t mode);
    void setVoltage(uint8_t voltage);
    void setModeSignal(uint8_t modeSignal);
    void setVoltageSignal(uint8_t voltageSignal);

    uint8_t getId();
    uint8_t getMode();
    bool getEnable();
    uint8_t getVoltage();

    //
    void setRGBChannelData(int32_t color_r, int32_t color_g, int32_t color_b);
    void setRGBChannelData(RGBColor color, int32_t value);
    void setRGBChannelData(RGBColor color, int32_t value,int32_t dir);
    void setRGBChannelData(std::vector<int32_t>& color_r,std::vector<int32_t>& color_g,std::vector<int32_t>& color_b,std::vector<bool>& ledNumber);
    
    void putincRGBChannelData(RGBColor color, int32_t value);
    
    void putRGBChannelData();
    void putRGBChannelData(RGBColor color);
    
    uint32_t getRGBChannelData(RGBColor color);

    //Digital Mode initialisation and usage methods

    void initDigital();
    void loadDigitalCore(uint8_t pio_select);
    void unLoadDigitalCore();
    bool isDigitalCoreloaded();
    inline void putDigitalLED(uint32_t data);
    inline void putDigitalLED(uint32_t data,uint16_t n);
    inline uint32_t convertRGBtoWS2812B(uint8_t r, uint8_t g, uint8_t b);

    uint32_t getNumberOfLeds();
    void setNumberOfLeds(uint32_t led_number);

    //Analog Mode initialiation Methods

    void initPWM();


private:
    uint8_t id;
    uint8_t mode;
    uint8_t voltage;
    bool enable;

    uint8_t mode_signal;
    uint8_t voltage_signal;
    uint8_t enable_signal;

    uint32_t color_r;
    uint32_t color_g;
    uint32_t color_b;

    uint8_t color_r_signal;
    uint8_t color_g_signal;
    uint8_t color_b_signal;

    uint32_t number_of_led;
    uint32_t *led_strip_data;

    bool effect;
    void *effect_func;
    

    uint8_t position_digital_core;
    PIO loaded_pio;
    uint8_t loaded_pio_offset;
    uint8_t loaded_pio_sm;
    bool is_loaded;
    bool is_rgbw;

    void getFirstElementOfColorVector(RGBColor color);
    uint32_t setFirstElementOfColorVector(RGBColor color);

};

#endif 