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

#ifndef VCL_BGFX_SYSTEM_NATIVE_WINDOW_HANDLE_H
#define VCL_BGFX_SYSTEM_NATIVE_WINDOW_HANDLE_H

namespace vcl {

#ifdef __APPLE__
namespace detail {

void* cretateCocoaWindow(
    const char* title,
    int         width,
    int         height,
    bool        hidden = false);

void closeCocoaWindow(void* window);

} // namespace detail
#endif // __APPLE__

void* createWindow(
    const char* title,
    int         width,
    int         height,
    void*&      display,
    bool        hidden = false);

void* createWindow(
    const char* title,
    int         width,
    int         height,
    bool        hidden = false);

void closeWindow(void* window, void* display = nullptr);

} // namespace vcl

#endif // VCL_BGFX_SYSTEM_NATIVE_WINDOW_HANDLE_H
