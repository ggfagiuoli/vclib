#ifndef WRAPPER_AUTOMATION_ACTION_H
#define WRAPPER_AUTOMATION_ACTION_H

#include "automation_action.h"

class WrapperAutomationAction : public AutomationAction
{
    using Parent = AutomationAction;

    protected:

    std::shared_ptr<AutomationAction> innerAction;

    WrapperAutomationAction(const AutomationAction &action)
    : innerAction{action.clone()}
    {}

    void start() override
    {
        Parent::start();
        innerAction->start();
    }

    void update() override
    {
        if(!innerAction->isActive()){
            return;
        }
        innerAction->update();
    };

    void end() override
    {
        if(innerAction->isActive()){
            innerAction->end();
        }
        Parent::end();
    }
};

#endif