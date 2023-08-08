#pragma once

#include "../hardwaredef/hardwaredef.hpp"
#include "../ledchannel/ledchannel.hpp"

class Effect
{
public:
    Effect();

    //Norm and Current time are the timespan on which a effect is acted on
    //and the time in that normtime there the effect currently is
    void setNormTime(uint32_t time); 
    uint32_t getNormTime();
    void setCurrentTime(uint32_t time);
    uint32_t getCurrentTime();

    void incCurentTimeByTick();

    void setParameter1(uint32_t param);
    uint32_t getParameter1();
    void setParameter2(uint32_t param);
    uint32_t getParameter2();

    void setColor1(RGBColor color);
    RGBColor getColor1();
    void setColor0(RGBColor color);
    RGBColor getColor0();

    std::vector<RGBColor> getEffectColors();
    void setEffectColors(std::vector<RGBColor>);
    void resetEffectColors();

    void setEffect(EffectList effect);
    EffectList getEffect();

    void updateEffect();

    void setChannel(Channel* channel);
    
    
    //Outputs the LEDs. Analog effects return a Vector with one element
    std::vector<RGBColor> getLEDs();



private:
    Channel* attached_channel;
    EffectList active_effect;

    std::vector<RGBColor> leds;
    std::vector<RGBColor> effect_color;

    uint32_t normal_time=30;
    uint32_t current_time=0;
    
    uint32_t parameter1=1;
    uint32_t parameter2=1;

    std::map<EffectList,std::vector<RGBColor>  (Effect::*)()> effect_function_list;

    std::vector<RGBColor> calcDisco();
    std::vector<RGBColor> calcGlow();
    std::vector<RGBColor> clacGlowMulti();
    std::vector<RGBColor> calcRainbow();
    std::vector<RGBColor> calcSnake();
    std::vector<RGBColor> calcBall();


};

//extern std::map<EffectList,bool> is_effect_analog;
