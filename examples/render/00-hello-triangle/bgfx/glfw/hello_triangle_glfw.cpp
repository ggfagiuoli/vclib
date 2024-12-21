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

#include "hello_triangle_glfw.h"

#include "../common.h"

HelloTriangleGLFW::HelloTriangleGLFW() :
        CanvasWindow("Hello Triangle GLFW")
{
    setUpBGFX(viewId(), vbh, program);
}

HelloTriangleGLFW::~HelloTriangleGLFW()
{
    bgfx::destroy(vbh);
    bgfx::destroy(program);
}

void HelloTriangleGLFW::onResize(vcl::uint width, vcl::uint height)
{
    std::cout << "Resize: " << width << "; " << height << ". Nothing to do\n";
}

void HelloTriangleGLFW::onDrawContent(vcl::uint)
{
    drawOnView(viewId(), vbh, program);
}
