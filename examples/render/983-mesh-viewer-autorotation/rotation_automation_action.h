#ifndef ROTATION_AUTOMATION_ACTION_H
#define ROTATION_AUTOMATION_ACTION_H

#include "automation_action.h"
#include <vclib/render/viewer/desktop_trackball.h>
#include <vclib/space/core/quaternion.h>
#include <bx/bx.h>

class RotationAutomationAction: public AutomationAction
{
    static inline uint32_t activeCount = 0;

    static void notifyStarted()
    {
        activeCount++;
    }

    static void notifyEnded()
    {
        activeCount--;
    }

    static bool isAnyActive()
    {
        return activeCount != 0;
    }

    using Parent = AutomationAction;
    vcl::DesktopTrackBall<float> *trackball;
    float radiansPerNS;
    vcl::Point3f around;


    public:

    static RotationAutomationAction fromSecondsPerRotation(vcl::DesktopTrackBall<float> *trackball, float secondsPerRotation, vcl::Point3f axis)
    {
        return RotationAutomationAction(trackball, bx::kPi2 / secondsPerRotation, axis);
    }

    static RotationAutomationAction* ptrFromSecondsPerRotation(vcl::DesktopTrackBall<float> *trackball, float secondsPerRotation, vcl::Point3f axis)
    {
        return new RotationAutomationAction(trackball, bx::kPi2 / secondsPerRotation, axis);
    }

    RotationAutomationAction(vcl::DesktopTrackBall<float> *trackball, float radiansPerSecond, vcl::Point3f axis)
    : trackball{trackball},
    radiansPerNS{radiansPerSecond / 1e9f},
    around{axis}
    {};

    void start() override
    {
        Parent::start();
        notifyStarted();
        trackball->startIgnoringTrackBallEvents();
    };

    void update() override
    {
        std::chrono::high_resolution_clock::time_point now = 
            std::chrono::high_resolution_clock::now();
        
        auto deltaT = std::chrono::duration_cast<std::chrono::nanoseconds>(now-prev);

        auto rotation = vcl::Quaternion<float>(radiansPerNS * (float)deltaT.count(), around);

        trackball->rotate(rotation);

        prev = now;
    };

    void end() override
    {
        Parent::end();
        notifyEnded();
        if(!isAnyActive()){
            trackball->stopIgnoringTrackBallEvents();
        }
    };
};

#endif