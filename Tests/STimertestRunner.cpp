#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <iostream>
using namespace std;


#include "../SoftwareTimer.h"

static bool isCalled=false,isExpired=false;
static uint32_t callerCounter=0;
static void TimerCb(void)
{
    isCalled=true;
    callerCounter++;
    isExpired=true;
}

TEST(SoftwareTimerTests,CanCreateTimer)
{
    SoftwareTimer_t* t=SoftwareTimerCreate((uint8_t*)"task1",TimerCb,1000,SINGLE_SHOOT_TIMER);
    SoftwareTimerResetAllTimers();
}


TEST(SoftwareTimerTests,CanInvokeCallback)
{
    SoftwareTimer_t* t=SoftwareTimerCreate((uint8_t*)"task1",TimerCb,1000,SINGLE_SHOOT_TIMER);
    t->callback();
    ASSERT_TRUE(isCalled);
    SoftwareTimerResetAllTimers();
    isExpired=false;
    callerCounter=0;
}


TEST(SoftwareTimerTests,CanSetTimeout)
{
    SoftwareTimer_t* t=SoftwareTimerCreate((uint8_t*)"task1",TimerCb,1000,SINGLE_SHOOT_TIMER);
    ASSERT_EQ(1000,SoftwareTimerGetTimeout(t));
    SoftwareTimerResetAllTimers();

}


TEST(SoftwareTimerTests,TimerTypeSetCorrectly)
{
    SoftwareTimer_t* t=SoftwareTimerCreate((uint8_t*)"task1",TimerCb,1000,SINGLE_SHOOT_TIMER);
    ASSERT_EQ(SINGLE_SHOOT_TIMER,SoftwareTimerGetTimerType(t));
    SoftwareTimerResetAllTimers();
}


TEST(SoftwareTimerTests,TimerisActivatedAfterInitialization)
{
    SoftwareTimer_t* t=SoftwareTimerCreate((uint8_t*)"task1",TimerCb,1000,PERIODIC_TIMER);
    ASSERT_TRUE(SoftwareTimerGetTimerState(t));
    SoftwareTimerResetAllTimers();
}


TEST(SoftwareTimerTests,CanCreateMaxNumberOfTimer)
{
    const uint32_t nofTimers=MAX_NUMBER_OF_TIMER;
    SoftwareTimer_t *timerObjects[nofTimers];
    for (size_t i = 0; i < nofTimers; i++)
    {
        timerObjects[i]=SoftwareTimerCreate((uint8_t*)"task1",TimerCb,1000,PERIODIC_TIMER);
        ASSERT_TRUE(timerObjects[i]!=NULL);
    }
    SoftwareTimerResetAllTimers();
}


TEST(SoftwareTimerTests,TickisZeroAfterInitialization)
{
    SoftwareTimer_t* t=SoftwareTimerCreate((uint8_t*)"task1",TimerCb,1000,PERIODIC_TIMER);
    ASSERT_EQ(0,SoftwareTimerGetTimerTick());
    SoftwareTimerResetAllTimers();

}

TEST(SoftwareTimerTests,CanIncreaseTickisWorking)
{
    for (size_t i = 0; i < 5; i++)
    {
        SoftwareTimerTick(1);
    }
    ASSERT_EQ(5,SoftwareTimerGetTimerTick());
    SoftwareTimerResetAllTimers();

}

TEST(SoftwareTimerTests,isPeriodicTimerWorks)
{
    callerCounter=0;
    SoftwareTimerResetAllTimers();
    SoftwareTimer_t *t1=SoftwareTimerCreate((uint8_t*)"task1",TimerCb,1000,PERIODIC_TIMER);

    for (size_t i = 0; i < 11; i++)
    {
        SoftwareTimerTick(200);
        SoftwareTimerLoop();
    }
    ASSERT_EQ(callerCounter,2);
    callerCounter=0;

}
TEST(SoftwareTimerTests,isShotTimerWorks)
{
    callerCounter=0;
    SoftwareTimerResetAllTimers();
    SoftwareTimer_t *t1=SoftwareTimerCreate((uint8_t*)"task1",TimerCb,1000,SINGLE_SHOOT_TIMER);
    for (size_t i = 0; i < 10; i++)
    {
        SoftwareTimerTick(200);
        SoftwareTimerLoop();
    }
    ASSERT_EQ(callerCounter,1);
}




int main(int argc,char **argv)
{
    
    testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();
    
}