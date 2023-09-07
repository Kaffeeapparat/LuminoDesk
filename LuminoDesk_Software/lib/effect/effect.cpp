#include "effect.hpp"

    Effect::Effect( )
    {
        //Setup colorarray for non remote operation
        this->effect_color.resize(2);
        effect_color[0]={0,0,0};
        effect_color[1]={0,0,0};


        //Setup functionpointers
        effect_function_list[EffectList::DISCO]=&Effect::calcDisco;
        effect_function_list[EffectList::RAMP]=&Effect::calcRamp;
        effect_function_list[EffectList::GLOW]=&Effect::calcGlow;
        effect_function_list[EffectList::GLOW_MULTI]=&Effect::clacGlowMulti;
        effect_function_list[EffectList::BALL]=&Effect::calcBall;
        effect_function_list[EffectList::SNAKE]=&Effect::calcSnake;
        effect_function_list[EffectList::COLORBAND]=&Effect::calcColorband;
        effect_function_list[EffectList::SPARKLE]=&Effect::calcSparkle;
        

    }

    void Effect::setChannel(Channel * channel)
    {
        this->attached_channel=channel;
    }

    void Effect::setNormTime(int32_t time) 
    {
        if(time>1)
        {
        this->normal_time=time;
        }
    }
    uint32_t Effect::getNormTime()
    {
        return this->normal_time;
    }

    void Effect::setCurrentTime(int32_t time)
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

    void Effect::setParameter1(int32_t param)
    {
        if(param<=this->parameter1_max&&param>=this->parameter1_min)
        {
        this->parameter1=param;
        }
    }
    uint32_t Effect::getParameter1()
    {
        return this->parameter1;
    }

    void Effect::setParameter0(int32_t param)
    {
        if(param<=this->parameter0_max&&param>=this->parameter0_min)
        {
        this->parameter0=param;
        }
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

    void Effect::setColor1(RGBColorSelect chosen_color,int32_t value)
    {
        auto it=rgb_color_weights.find(chosen_color);
        value*=4;
        for(RGBColorSelect color : rgb_color_weights[it->first])
        {

        if(value>=0)
        {
            if((RGBcolorReturnColor(this->effect_color[1],color))>(DIG_WRAP-value))
            {
                RGBcolorSetColor(&(this->effect_color[1]),color,DIG_WRAP);
            }
            else{
                RGBcolorSetColor(&(this->effect_color[1]),color,RGBcolorReturnColor(this->effect_color[1],color)+value);
            }
        }
        else if(((int32_t)RGBcolorReturnColor(this->effect_color[1],color)+value)>0)
        {
            RGBcolorSetColor(&(this->effect_color[1]),color,RGBcolorReturnColor(this->effect_color[1],color)+value);
        }
        else
        {
            RGBcolorSetColor(&(this->effect_color[1]),color,0);
        }
        }

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
           this->parameter0=0;
           this->parameter1=0;
           
           this->parameter1_max=300;
           this->parameter1_min=0;
           this->parameter0_max=300;
           this->parameter0_min=0;
            break;
        case EffectList::RAMP:
           this->active_effect=EffectList::GLOW;
           this->parameter0=0;
           this->parameter1=0;
           
           this->parameter1_max=300;
           this->parameter1_min=0;
           this->parameter0_max=300;
           this->parameter0_min=0;
            break;
        case EffectList::GLOW:
            this->active_effect=EffectList::GLOW_MULTI;
            break;
        case EffectList::GLOW_MULTI:
        //this->active_effect=EffectList::BALL;
        //    break;
        // case EffectList::BALL:
            this->active_effect=EffectList::SNAKE;
             break;
        case EffectList::SNAKE:
            //Loopback
           this->active_effect=EffectList::COLORBAND; 
           this->parameter0=100;
           this->parameter1=200;
           
            this->parameter1_max=300;
            this->parameter1_min=150;
            this->parameter0_max=150;
            this->parameter0_min=0;
           
           break;
        case EffectList::COLORBAND:
            this->active_effect=EffectList::SPARKLE;
            this->parameter0=0;
            this->parameter1=0;
            break;
        case EffectList::SPARKLE:
            this->active_effect=EffectList::RAMP;
            this->parameter0=0;
            this->parameter1=0;
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
        double to_decide;
        double norm_rand=300.0/255.0;
        RGBColor currentColor={0,0,0};
        std::vector<RGBColor> returnvector;

        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<> dist(0, 255);

        returnvector.resize(this->attached_channel->getMaxNumberOfLeds());

        if(timeratio==1)
        {
        for(uint16_t led=0;led<this->attached_channel->getMaxNumberOfLeds();led++)
        {
            to_decide=dist(gen)*norm_rand;

            if(to_decide<this->parameter0&&((uint32_t)this->parameter0!=(uint32_t)this->parameter0_min))
            {
                currentColor.red=dist(gen);
            }
            if(to_decide>this->parameter1&&((uint32_t)this->parameter1!=(uint32_t)this->parameter1_max))
            {
                currentColor.green=dist(gen);
            }
            if((to_decide<this->parameter1)&&(to_decide>this->parameter0)&&((uint32_t)this->parameter1!=(uint32_t)this->parameter0))
            {
                currentColor.blue=dist(gen);

            }


            if(led<this->attached_channel->getNumberOfLeds())
            {
            returnvector[led]=currentColor;
            }
            else
            {
            returnvector[led]={0,0,0};
            }

        }
        }
        else
        {
            return this->attached_channel->getRGBChannelData();
        }
        return returnvector;
    }
    std::vector<RGBColor> Effect::clacGlowMulti()
        {
        double timeratio=((double)this->current_time)/(double)this->normal_time;
        timeratio*=2;
        RGBColor currentColor;
        std::vector<RGBColor> returnvector;
        returnvector.resize(this->attached_channel->getMaxNumberOfLeds());
     
        if(timeratio<1.0)
        {
        timeratio-=0.5;
            if(timeratio<0)
            {
            timeratio+=0.5;
            currentColor.red=std::fabs(((double)this->effect_color[0].red)*timeratio*2);
            currentColor.green=std::fabs(((double)this->effect_color[0].green)*timeratio*2);
            currentColor.blue=std::fabs(((double)this->effect_color[0].blue)*timeratio*2);
            }
            else
            {
            timeratio-=0.5;
            currentColor.red=std::fabs(((double)this->effect_color[0].red)*timeratio*2);
            currentColor.green=std::fabs(((double)this->effect_color[0].green)*timeratio*2);
            currentColor.blue=std::fabs(((double)this->effect_color[0].blue)*timeratio*2);
            }
        }
        else if(timeratio>=1.0)
        {
        timeratio-=1.5;
            if(timeratio<0)
            {
            timeratio+=0.5;
            currentColor.red=std::fabs(((double)this->effect_color[1].red)*timeratio*2);
            currentColor.green=std::fabs(((double)this->effect_color[1].green)*timeratio*2);
            currentColor.blue=std::fabs(((double)this->effect_color[1].blue)*timeratio*2);
            }
            else
            {
            timeratio-=0.5;
            currentColor.red=std::fabs(((double)this->effect_color[1].red)*timeratio*2);
            currentColor.green=std::fabs(((double)this->effect_color[1].green)*timeratio*2);
            currentColor.blue=std::fabs(((double)this->effect_color[1].blue)*timeratio*2);
            } 
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





    std::vector<RGBColor> Effect::calcColorband()
    {
        double timeratio=(double)this->current_time/((double)this->normal_time);
        double ledratio=0;
        double color_passages=this->effect_color.size();
        std::vector<RGBColor> returnvector;
        double boarder_min,boarder_max=0;
        returnvector.resize(this->attached_channel->getMaxNumberOfLeds());

        for(double n=0;n<color_passages;n++)
        {
        boarder_min=(1/color_passages)*n;
        boarder_max=(1/color_passages)*(n+1.0);

        for(int i=0;i<this->attached_channel->getMaxNumberOfLeds();i++)
        {
            if(i<this->attached_channel->getNumberOfLeds())
            {
                ledratio=(double)i/(double)this->attached_channel->getNumberOfLeds();

                if((ledratio>=boarder_min)&&(ledratio<boarder_max))
                {
                    returnvector[i]=effect_color[n];
                }
            }
            else
            {
                returnvector[i]={0,0,0};
            }


        }
        }
        double rotationstep=(double)1.0/(double)this->normal_time;
        int32_t number_of_rotationsteps=this->attached_channel->getMaxNumberOfLeds()/color_passages;

        number_of_rotationsteps-=number_of_rotationsteps*(3/2);
        number_of_rotationsteps+=this->attached_channel->getMaxNumberOfLeds()/color_passages*timeratio;

        if(number_of_rotationsteps>1)
        {
        std::rotate(returnvector.begin(),(returnvector.begin()+number_of_rotationsteps*2),returnvector.begin()+this->attached_channel->getNumberOfLeds());
        }
        else
        {
        number_of_rotationsteps=std::abs(number_of_rotationsteps);
        std::rotate(returnvector.begin()+this->attached_channel->getNumberOfLeds(),(returnvector.begin()+number_of_rotationsteps*2),returnvector.begin());
        }
        return returnvector;
    }



    std::vector<RGBColor> Effect::calcSnake()
    {
        double timeratio=(double)this->current_time/((double)this->normal_time);
        double timeratio_min = 1.0/((double)this->normal_time);
        
        uint8_t snake_width=(double)this->attached_channel->getNumberOfLeds()*(double)timeratio_min;
        if(snake_width==0)
        {
            snake_width=1;
        }

        RGBColor currentColor;
        currentColor=this->effect_color[0];

        std::vector<RGBColor> returnvector;
        returnvector.resize(this->attached_channel->getMaxNumberOfLeds());

    for(uint8_t n=0; timeratio_min*n<1;n++)
    {
        if(timeratio_min*n==timeratio)
        {
        for(uint16_t led=0;led<this->attached_channel->getMaxNumberOfLeds();led++)
        {
            if(led<this->attached_channel->getNumberOfLeds())
            {
            if(led>n*snake_width && led<(n+2)*snake_width)
            {
                returnvector[led]=currentColor;
            }
            }
            else
            {
                returnvector[led]={0,0,0};
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

    
    std::vector<RGBColor> Effect::calcSparkle()
    {
        double timeratio=(double)this->current_time/((double)this->normal_time);
        double timeratio_min = 1.0/((double)this->normal_time);
        double rate=0.3;

        RGBColor currentColor;
        currentColor=this->effect_color[0];

        std::vector<RGBColor> returnvector=this->attached_channel->getRGBChannelData();
        returnvector.resize(this->attached_channel->getMaxNumberOfLeds());

        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<> dist(0, 100);

        if(this->leds.size()!=this->attached_channel->getMaxNumberOfLeds())
        {
            this->leds.resize(this->attached_channel->getMaxNumberOfLeds());
        }


        if(timeratio==1)
        {
        for(int n=0;n<this->leds.size();n++)
        {
            if(dist(gen)>50)
            {    
            this->leds[n]=currentColor;
            }
            else
            {
            this->leds[n]={0,0,0};
            }
        }
        }

        for(int n=0;n<this->attached_channel->getMaxNumberOfLeds();n++)
        {
            if(returnvector[n].red<this->leds[n].red)
            {
                if((returnvector[n].red==0)&&(this->leds[n].red!=0))
                {
                    returnvector[n].red=5;
                }
                else
                {
                returnvector[n].red*=(1+rate);
                }
            }
            else if((this->leds[n].red==0))
            {
                returnvector[n].red*=(1-rate);
            }

            if(returnvector[n].green<this->leds[n].green)
            {
                if((returnvector[n].green==0)&&(this->leds[n].green!=0))
                {
                    returnvector[n].green=5;
                }
                else
                {
                returnvector[n].green*=(1+rate);
                }
            }
            else if((this->leds[n].green==0))
            {
                returnvector[n].green*=(1-rate);
            }

            if(returnvector[n].blue<this->leds[n].blue)
            {
                if((returnvector[n].blue==0)&&(this->leds[n].blue!=0))
                {
                    returnvector[n].blue=5;
                }
                else
                {
                returnvector[n].blue*=(1+rate);
                }
            }
            else if((this->leds[n].blue==0))
            {
                returnvector[n].blue*=(1-rate);
            }
        }

        
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
    {EffectList::BALL,0},
    {EffectList::COLORBAND,1},
    {EffectList::SPARKLE,1}
};


RGBColor Effect::overlayColors(RGBColor n,RGBColor m)
{
    RGBColor return_color={0,0,0};

    if(n.red <= m.red)
        return_color.red=m.red;
    else
        return_color.red=n.red;

    if(n.green <= m.green)
        return_color.green=m.green;
    else
        return_color.green=n.green;

    if(n.blue <= m.blue)
        return_color.blue=m.blue;
    else
        return_color.blue=n.blue;

    return return_color;
}