#pragma once


#include "../hardwaredef/hardwaredef.hpp"
#include "../ledchannel/ledchannel.hpp"


class Effect
{
public:
    Effect();

    void setNormalTime(uint32_t time); 
    uint32_t getNormalTime();
    void setCurrentTime(uint32_t time);
    uint32_t getCurrentTime();

    void setParameter1(uint32_t param);
    uint32_t getParameter1();
    void setParameter2(uint32_t param);
    uint32_t getParameter2();


    void setColor1(RGBColor color);
    RGBColor getColor1();
    void setColor2(RGBColor color);
    RGBColor getColor2();


private:
    uint32_t normal_time;
    uint32_t current_time;
    
    uint32_t parameter1;
    uint32_t parameter2;

    RGBColor color1;
    RGBColor color2;
};