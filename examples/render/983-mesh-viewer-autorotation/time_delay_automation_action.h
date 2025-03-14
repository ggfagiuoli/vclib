#ifndef TIME_DELAY_AUTOMATION_ACTION_H
#define TIME_DELAY_AUTOMATION_ACTION_H

#include "wrapper_automation_action.h"
#include <vclib/misc/timer.h>

class TimeDelayAutomationAction : public WrapperAutomationAction
{
    vcl::Timer timer;
    float delaySeconds;
    bool innerStarted = false;
    using Parent = WrapperAutomationAction;

    public:

    TimeDelayAutomationAction(AutomationAction *action, float delaySeconds)
    : Parent(action),
    delaySeconds{delaySeconds}
    {};

    void start() override
    {
        AutomationAction::start();
        timer.start();
    }

    void update() override
    {
        if(timer.delay() < delaySeconds){
            return;
        }
        if(!innerStarted){
            innerAction->start();
            innerStarted = true;
        }
        if(innerAction->isActive()){
            innerAction->update();
            return;
        }
        end();
    }

    void end() override
    {
        Parent::end();
        timer.stop();
        innerStarted = false;
    }
};

#endif