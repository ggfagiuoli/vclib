/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#include <vclib/ext/bgfx/context.h>

#include <vclib/types/base.h>
#include <vclib/gui/native_window_handle.h>

namespace vcl::bgf {

Context& Context::instance()
{
    static Context ctx;
    return ctx;
}

bgfx::ViewId Context::requestViewId()
{
    bgfx::ViewId viewId = instance().viewStack.top();
    instance().viewStack.pop();
    return viewId;
}

void Context::releaseViewId(bgfx::ViewId viewId)
{
    instance().viewStack.push(viewId);
}

Context::Context()
{
    windowHandle = vcl::createWindow("", 1, 1, displayHandle, true);
#ifdef __APPLE__
    bgfx::renderFrame(); // needed for macos
#endif                   // __APPLE__

    bgfx::Init init;
    init.platformData.nwh  = windowHandle;
    init.type              = renderType;
    init.platformData.ndt  = displayHandle;
    init.resolution.width  = 1;
    init.resolution.height = 1;
    init.resolution.reset  = BGFX_RESET_NONE;
    init.callback          = &cb;
    bgfx::init(init);

    vcl::closeWindow(windowHandle, displayHandle);

    uint mv = bgfx::getCaps()->limits.maxViews;

    while (mv != 0) {
        viewStack.push((bgfx::ViewId) mv--);
    }
    viewStack.push((bgfx::ViewId) 0);
}

Context::~Context()
{
    bgfx::shutdown();
}

} // namespace vcl::bgf
