#ifndef __SOFTWARE_TIMER_H__
#define __SOFTWARE_TIMER_H__

#include<stdint.h>
#include<stdlib.h>
#include <stdbool.h>

#define MAX_NUMBER_OF_TIMER 10

typedef void(*TimerCallback_t)(void);

typedef enum 
{
    SINGLE_SHOOT_TIMER=0,
    PERIODIC_TIMER=1
}TimerType;

struct SoftwareTimer
{
    uint8_t* name;
    uint64_t timeout,lasttimeout;
    TimerCallback_t callback;
    TimerType type;
    bool isActive;
};
typedef struct SoftwareTimer SoftwareTimer_t; 
static uint32_t timerIdCounter=0;
static uint64_t ticks=0;
static SoftwareTimer_t _timerPool[MAX_NUMBER_OF_TIMER];


SoftwareTimer_t* SoftwareTimerCreate(uint8_t *timerName,TimerCallback_t cb,uint32_t timeout,TimerType type)
{
    if(timerIdCounter < MAX_NUMBER_OF_TIMER)
    {   
        _timerPool[timerIdCounter].name=timerName;
        _timerPool[timerIdCounter].callback=cb;
        _timerPool[timerIdCounter].timeout=timeout;
        _timerPool[timerIdCounter].type=type;
        _timerPool[timerIdCounter].isActive=true;
        _timerPool[timerIdCounter].lasttimeout=0;
        return &_timerPool[timerIdCounter++];
    }
    return NULL;
}
void SoftwareTimerResetAllTimers()
{
    for (size_t i = 0; i < MAX_NUMBER_OF_TIMER; i++)
    {
        _timerPool[i].isActive=false;
        _timerPool[i].callback=NULL;
        _timerPool[i].name=NULL;
        _timerPool[i].timeout=0;
        _timerPool[i].type=SINGLE_SHOOT_TIMER;
        _timerPool[timerIdCounter].lasttimeout=0;

        timerIdCounter=0;
    }
}

uint32_t SoftwareTimerGetTimeout(SoftwareTimer_t *self)
{
    return self->timeout;
}
TimerType SoftwareTimerGetTimerType(SoftwareTimer_t *self)
{
    return self->type;
}
bool SoftwareTimerGetTimerState(SoftwareTimer_t *self)
{
    return self->isActive;
}
uint64_t SoftwareTimerGetTimerTick()
{
    return ticks;
}

void SoftwareTimerTick(uint32_t inc)
{
    ticks+=inc;
}

void SoftwareTimerLoop()
{   
    for (size_t i = 0; i < MAX_NUMBER_OF_TIMER; i++)
    {
        if(_timerPool[i].isActive)
        {
            if ((ticks-_timerPool[i].lasttimeout)>=_timerPool[i].timeout)
            {
                _timerPool[i].callback();
                if(_timerPool[i].type==SINGLE_SHOOT_TIMER)
                {
                    _timerPool[i].isActive=0;
                }
                _timerPool[i].lasttimeout=ticks;
            }
        }
    }
}

void SoftwareTimerActivate(SoftwareTimer_t *self)
{
    if(!self->isActive)
    {
        self->isActive=true;
        self->lasttimeout=ticks;
    }
}
void SoftwareTimerDeactivate(SoftwareTimer_t *self)
{
    if(self->isActive)
    {
        self->isActive=false;
    }
}



#endif
