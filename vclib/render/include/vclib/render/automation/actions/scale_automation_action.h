/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#ifndef VCL_SCALE_AUTOMATION_ACTION_H
#define VCL_SCALE_AUTOMATION_ACTION_H

#include <vclib/render/automation/actions/abstract_automation_action.h>

#include <vclib/misc/timer.h>

namespace vcl {

/**
 * The ScaleAutomationAction is an automation that represents the
 * scaling of a DesktopTrackball, with the strength of the scaling measured
 * per-second
 */
template<typename BmarkDrawer>
class ScaleAutomationAction : public AbstractAutomationAction<BmarkDrawer>
{
    using Parent = AbstractAutomationAction<BmarkDrawer>;
    using Parent::benchmarkDrawer;
    float mPixelDeltaPerSecond;
    float mTotalPixelDelta;
    Timer mTimer;

public:
    ScaleAutomationAction(float pixelDeltaPerSecond) :
            mPixelDeltaPerSecond {pixelDeltaPerSecond}, mTotalPixelDelta {0}
    {
    }

    void start() override
    {
        Parent::start();
        mTimer.start();
    }

    void doAction() override
    {
        Parent::doAction();
        mTotalPixelDelta += mPixelDeltaPerSecond * mTimer.delay();
        benchmarkDrawer->scale(mPixelDeltaPerSecond * mTimer.delay());
        mTimer.start();
    };

    void end() override
    {
        Parent::end();
        mTimer.stop();
        mTotalPixelDelta = 0;
    };

    std::shared_ptr<AbstractAutomationAction<BmarkDrawer>> clone()
        const& override
    {
        return std::make_shared<ScaleAutomationAction<BmarkDrawer>>(*this);
    }

    std::shared_ptr<AbstractAutomationAction<BmarkDrawer>> clone() && override
    {
        return std::make_shared<ScaleAutomationAction<BmarkDrawer>>(
            std::move(*this));
    }
};

} // namespace vcl

#endif
