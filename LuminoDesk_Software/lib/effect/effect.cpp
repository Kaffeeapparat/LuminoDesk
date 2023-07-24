#include "../effect/effect.hpp"

    Effect::Effect(){}

    void Effect::setNormalTime(uint32_t time) 
    {
        this->normal_time=time;
    }
    uint32_t Effect::getNormalTime()
    {
        return this->normal_time;
    }

    void Effect::setCurrentTime(uint32_t time)
    {
        this->current_time=time;
    }
    uint32_t Effect::getCurrentTime()
    {
        return this->current_time;
    }

    void Effect::setParameter1(uint32_t param)
    {
        this->parameter1=param;
    }
    uint32_t Effect::getParameter1()
    {
        return this->parameter1;
    }

    void Effect::setParameter2(uint32_t param)
    {
        this->parameter2=param;
    }
    uint32_t Effect::getParameter2()
    {
        return parameter2;
    }

    void Effect::setColor1(RGBColor color) 
    {
        this->color1=color;
    }
    RGBColor Effect::getColor1()
    {
        return this->color1;
    }
    void Effect::setColor2(RGBColor color)
    {
        this->color2=color;
    }
    RGBColor Effect::getColor2()
    {
        return this->color2;
    }

