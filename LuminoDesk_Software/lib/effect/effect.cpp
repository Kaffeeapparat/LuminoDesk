#include "effect.hpp"

    Effect::Effect( )
    {
        effect_function_list[EffectList::DISCO]=&Effect::calcDisco;
        effect_function_list[EffectList::GLOW]=&Effect::calcGlow;
        effect_function_list[EffectList::GLOW_MULTI]=&Effect::clacGlowMulti;
        effect_function_list[EffectList::BALL]=&Effect::calcBall;
        effect_function_list[EffectList::SNAKE]=&Effect::calcSnake;

    }

    void Effect::setChannel(Channel * channel)
    {
        this->attached_channel=channel;
    }

    void Effect::setNormTime(uint32_t time) 
    {
        this->normal_time=time;
    }
    uint32_t Effect::getNormTime()
    {
        return this->normal_time;
    }

    void Effect::setCurrentTime(uint32_t time)
    {
        this->current_time=time;
        if(this->current_time>=this->normal_time)
        {
            this->current_time=0;
        }
    }
    uint32_t Effect::getCurrentTime()
    {
        return this->current_time;
    }

    void Effect::incCurentTimeByTick()
    {
        this->current_time++;

        if(this->current_time>=this->normal_time)
        {
            this->current_time=0;
        }
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

    void Effect::setColor0(RGBColor color) 
    {
        this->effect_color[0]=color;
    }
    RGBColor Effect::getColor0()
    {
        return this->effect_color[0];
    }
    void Effect::setColor1(RGBColor color)
    {
        this->effect_color[1]=color;
    }
    RGBColor Effect::getColor1()
    {
        return this->effect_color[1];
    }

    void Effect::setEffect(EffectList effect)
    {
        this->active_effect=effect;
    }
    EffectList Effect::getEffect()
    {
        return this->active_effect;
    }

    void Effect::updateEffect()
    {
            this->leds=(this->*effect_function_list[this->active_effect])();
    }

    std::vector<RGBColor> Effect::getLEDs()
    {
        return (this->*effect_function_list[this->active_effect])();
    }

    std::vector<RGBColor> Effect::calcGlow()
    {
        double timeratio=(double)this->current_time/(double)this->normal_time;
        RGBColor currentColor;
        std::vector<RGBColor> returnvector;

        returnvector.resize(this->attached_channel->getMaxNumberOfLeds());
     

        currentColor.red=this->effect_color[0].red*timeratio;
        currentColor.green=this->effect_color[0].green*timeratio;
        currentColor.blue=this->effect_color[0].blue*timeratio;
        

        RGBColor test1;
        RGBColor test2;
        RGBColor test3;

        for(uint16_t led=0;led<this->attached_channel->getMaxNumberOfLeds();led++)
        {
            if(led<this->attached_channel->getNumberOfLeds())
            {
            returnvector[led]=currentColor;
            }

        }
/*

        returnvector.empty();
        returnvector.push_back(test1);
        returnvector.push_back(test2);
        returnvector.push_back(test3);
*/

        return returnvector;
    }



    std::vector<RGBColor> Effect::calcDisco()
    {
        std::vector<RGBColor> returnvector={{1,2,3}};
        return returnvector;
    }
    std::vector<RGBColor> Effect::clacGlowMulti()
        {
        std::vector<RGBColor> returnvector={{1,2,3}};
        return returnvector;
        }
    std::vector<RGBColor> Effect::calcRainbow()
            {
        std::vector<RGBColor> returnvector={{1,2,3}};
        return returnvector;
        }
    std::vector<RGBColor> Effect::calcSnake()
            {
        std::vector<RGBColor> returnvector={{1,2,3}};
        return returnvector;
        }
    std::vector<RGBColor> Effect::calcBall()
            {
        std::vector<RGBColor> returnvector={{1,2,3}};
        return returnvector;
        }

std::map<EffectList,bool> is_effect_analog=
{
    {EffectList::DISCO,1},
    {EffectList::GLOW,1},
    {EffectList::GLOW_MULTI,0},
    {EffectList::RAINBOW,1},
    {EffectList::SNAKE,0},
    {EffectList::BALL,0}  
};


