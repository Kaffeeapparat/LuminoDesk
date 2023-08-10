#include "effect.hpp"

    Effect::Effect( )
    {
        //Setup colorarray for non remote operation
        this->effect_color.resize(2);

        //Setup functionpointers
        effect_function_list[EffectList::DISCO]=&Effect::calcDisco;
        effect_function_list[EffectList::RAMP]=&Effect::calcRamp;
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

        if(this->current_time>this->normal_time)
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

    void Effect::setParameter0(uint32_t param)
    {
        this->parameter0=param;
    }
    uint32_t Effect::getParameter0()
    {
        return parameter0;
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
    void Effect::toggleEffect()
    {
    switch(this->active_effect) {
        case EffectList::DISCO:
           this->active_effect=EffectList::RAMP;
            break;
        case EffectList::RAMP:
           this->active_effect=EffectList::GLOW;
            break;
        case EffectList::GLOW:
        //    this->active_effect=EffectList::GLOW_MULTI;
        //     break;
        //case EffectList::GLOW_MULTI:
        //   this->active_effect=EffectList::BALL;
        //    break;
        // case EffectList::BALL:
            this->active_effect=EffectList::SNAKE;
             break;
        case EffectList::SNAKE:
            //Loopback
           this->active_effect=EffectList::DISCO; 
            break;
        default:
            break;
    }
    }

    void Effect::updateEffect()
    {
            this->leds=(this->*effect_function_list[this->active_effect])();
    }

    std::vector<RGBColor> Effect::getLEDs()
    {
        return (this->*effect_function_list[this->active_effect])();
    }

    std::vector<RGBColor> Effect::calcRamp()
    {
        double timerratio=(((double)((this->current_time-(this->parameter0+this->parameter1))))/((double)this->normal_time-(this->parameter0+this->parameter1)));
        
        if(timerratio<0||timerratio>1)
        {
            timerratio=0;
        }
        RGBColor currentColor;
        std::vector<RGBColor> returnvector;

        returnvector.resize(this->attached_channel->getMaxNumberOfLeds());
     
        currentColor.red=this->effect_color[0].red*timerratio;
        currentColor.green=this->effect_color[0].green*timerratio;
        currentColor.blue=this->effect_color[0].blue*timerratio;

        if(this->current_time<this->parameter0)
        {
        currentColor.red=0;
        currentColor.green=0;
        currentColor.blue=0;
        }
        if(this->current_time+this->parameter0>(this->normal_time-this->parameter1))
        {
        currentColor.red=this->effect_color[0].red;
        currentColor.green=this->effect_color[0].green;
        currentColor.blue=this->effect_color[0].blue;
        }

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

    std::vector<RGBColor> Effect::calcGlow()
    {
        double timeratio=((double)this->current_time)/(double)this->normal_time;
        timeratio-=0.5;
        RGBColor currentColor;
        std::vector<RGBColor> returnvector;

        returnvector.resize(this->attached_channel->getMaxNumberOfLeds());
     

        currentColor.red=std::fabs(((double)this->effect_color[0].red)*timeratio*2);
        currentColor.green=std::fabs(((double)this->effect_color[0].green)*timeratio*2);
        currentColor.blue=std::fabs(((double)this->effect_color[0].blue)*timeratio*2);


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
        double timeratio=(double)this->current_time/(double)(this->normal_time);
        RGBColor currentColor;
        std::vector<RGBColor> returnvector;
        std::srand(to_us_since_boot (get_absolute_time()));
        returnvector.resize(this->attached_channel->getMaxNumberOfLeds());
        currentColor.red=255;
        currentColor.green=255;
        currentColor.blue=255;


        for(uint16_t led=0;led<this->attached_channel->getMaxNumberOfLeds();led++)
        {
            if(220<(1+std::rand() / ((RAND_MAX + 1u) / 255)))
            {
                while(currentColor.red > 254)
                {
                    currentColor.red=1+std::rand() / ((RAND_MAX + 1u) / 255);
                }
            }
            if(220<(1+std::rand() / ((RAND_MAX + 1u) / 255)))
            {
                while(currentColor.green > 254)
                {
                    currentColor.green=1+std::rand() / ((RAND_MAX + 1u) / 255);
                }
            }
            if(220<(1+std::rand() / ((RAND_MAX + 1u) / 255)))
            {
                while(currentColor.blue > 254)
                {
                    currentColor.blue=1+std::rand() / ((RAND_MAX + 1u) / 255);
                }
            }


            if(led<this->attached_channel->getNumberOfLeds())
            {
            returnvector[led]=currentColor;
            }

        }
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
        double timeratio=(double)this->current_time/((double)this->normal_time);
        double timeratio_min = 1.0/((double)this->normal_time);
        
        uint8_t snake_width=(double)this->attached_channel->getMaxNumberOfLeds()*(double)timeratio_min;
        if(snake_width==0)
        {
            snake_width=1;
        }

        RGBColor currentColor;
        currentColor.red=50;

        std::vector<RGBColor> returnvector;

        returnvector.resize(this->attached_channel->getMaxNumberOfLeds());

    for(uint8_t n=0; timeratio_min*n<1;n++)
    {
        if(timeratio_min*n==timeratio)
        {
        for(uint16_t led=0;led<this->attached_channel->getMaxNumberOfLeds();led++)
        {
            if(led>n*snake_width && led<(n+2)*snake_width)
            {
                returnvector[led]=currentColor;
            }
            
        }
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
    
    
    std::vector<RGBColor> Effect::calcBall()
            {
        std::vector<RGBColor> returnvector={{1,2,3}};
        return returnvector;
        }

std::map<EffectList,bool> is_effect_analog=
{
    {EffectList::RAMP,1},
    {EffectList::DISCO,1},
    {EffectList::GLOW,1},
    {EffectList::GLOW_MULTI,0},
    {EffectList::RAINBOW,1},
    {EffectList::SNAKE,0},
    {EffectList::BALL,0}  
};


