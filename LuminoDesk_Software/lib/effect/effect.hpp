#pragma once

#include <random>

#include "../hardwaredef/hardwaredef.hpp"
#include "../ledchannel/ledchannel.hpp"

class Effect
{
public:
    Effect();

    //Norm and Current time are the timespan on which a effect is acted on
    //and the time in that normtime there the effect currently is
    void setNormTime(int32_t time); 
    uint32_t getNormTime();
    void setCurrentTime(int32_t time);
    uint32_t getCurrentTime();

    void incCurentTimeByTick();

    void setParameter1(int32_t param);
    uint32_t getParameter1();
    void setParameter0(int32_t param);
    uint32_t getParameter0();

    void setColor0(RGBColor color);
    void setColor0(RGBColorSelect chosen_color,int32_t value);
    RGBColor getColor0();

    void setColor1(RGBColor color);
    void setColor1(RGBColorSelect chosen_color,int32_t value);
    RGBColor getColor1();

    std::vector<RGBColor> getEffectColors();
    void setEffectColors(std::vector<RGBColor>);
    void resetEffectColors();

    void setEffect(EffectList effect);
    void toggleEffect();
    EffectList getEffect();

    void updateEffect();
    void setChannel(Channel* channel);

    RGBColor overlayColors(RGBColor n,RGBColor m);
    
    
    //Outputs the LEDs. Analog effects return a Vector with one element
    std::vector<RGBColor> getLEDs();



private:
    Channel* attached_channel;
    EffectList active_effect;

    std::vector<RGBColor> leds;
    std::vector<RGBColor> effect_color;

    std::random_device rd;

    int32_t normal_time=30;
    int32_t current_time=0;
    
    int32_t parameter1=1;
    int32_t parameter0=1;

    int32_t parameter1_max=255;
    int32_t parameter0_max=255;

    int32_t parameter1_min=0;
    int32_t parameter0_min=0;

    std::map<EffectList,std::vector<RGBColor>  (Effect::*)()> effect_function_list;

    std::vector<RGBColor> calcDisco();
    std::vector<RGBColor> calcGlow();
    std::vector<RGBColor> clacGlowMulti();
    std::vector<RGBColor> calcRainbow();
    std::vector<RGBColor> calcSnake();
    std::vector<RGBColor> calcBall();
    std::vector<RGBColor> calcRamp();


};

//extern std::map<EffectList,bool> is_effect_analog;
