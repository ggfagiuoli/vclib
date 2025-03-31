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

#ifndef VCL_AUTOMATION_ACTION_H
#define VCL_AUTOMATION_ACTION_H

#include <chrono>
#include <memory>

namespace vcl {

/**
 * The AbstractAutomationAction is a class that represents something to be done
 * by a BenchmarkDrawer
 */
class AbstractAutomationAction
{
private:
    bool active = false;

public:
    /**
     * Only ever called by outside sources
     *
     * An automation SHOULD NOT call start on itself during an update
     * A call to start SHOULD guarantee that the action is considered active at
     * least until the next doAction
     */
    virtual void start() { active = true; };

    /**
     * Called every frame (as long as the action is active)
     *
     * An automation may call end() on itself during a doAction(),
     * and it is therefore important to check whether the automation is active
     * before calling doAction()
     *
     * @throws If it is called on an inactive action
     */
    virtual void doAction()
    {
        if (!isActive()) {
            throw "Do not call doAction() on inactive actions!";
        }
    };

    /**
     * Either called by the automation itself or by outside sources
     *
     * This method ALSO has to reset everything in THIS object so that a
     * subsequent start() call is like calling start() on a new object equal to
     * this one (unless it makes no sense for the action to be started multiple
     * times)
     */
    virtual void end() { active = false; };

    virtual bool isActive() { return active; }

    operator std::shared_ptr<AbstractAutomationAction>() { return clone(); };

    virtual std::shared_ptr<AbstractAutomationAction> clone() const& = 0;
    virtual std::shared_ptr<AbstractAutomationAction> clone() &&     = 0;
};

} // namespace vcl

#endif
