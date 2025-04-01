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

#ifndef VIEW_PROJECTION_H
#define VIEW_PROJECTION_H

#include <vclib/render/canvas.h>
#include <vclib/render/concepts/view_projection.h>
#include <vclib/render/drawers/trackball_event_drawer.h>
#include <vclib/render/render_app.h>
#include <vclib/render/viewer/camera.h>

#ifdef VCLIB_WITH_GLFW
#include <vclib/glfw/window_manager.h>

template<typename DR>
using WMG = vcl::glfw::WindowManager<DR>;
#endif // VCLIB_WITH_GLFW

void viewProjectionStaticAsserts()
{
    using namespace vcl;

    using Camerad = Camera<double>;

    static_assert(
        !ViewProjectionConcept<Camerad>,
        "Camera does satisfy the ViewProjectionConcept");
    static_assert(
        ViewProjectionConcept<const Camerad>,
        "const Camera does not satisfy the ViewProjectionConcept");
    static_assert(
        !ViewProjectionConcept<Camerad&>,
        "Camera& does satisfy the ViewProjectionConcept");
    static_assert(
        ViewProjectionConcept<const Camerad&>,
        "const Camera& does not satisfy the ViewProjectionConcept");
    static_assert(
        !ViewProjectionConcept<Camerad&&>,
        "Camera&& does satisfy the ViewProjectionConcept");

#if VCLIB_WITH_GLFW
    using RenderAppT = RenderApp<WMG, Canvas, TrackBallEventDrawer>;

    static_assert(
        ViewProjectionConcept<TrackBallEventDrawer<RenderAppT>>,
        "TrackBallEventDrawer does not satisfy the ViewProjectionConcept");
    static_assert(
        ViewProjectionConcept<const TrackBallEventDrawer<RenderAppT>>,
        "const TrackBallEventDrawer does not satisfy the "
        "ViewProjectionConcept");
    static_assert(
        ViewProjectionConcept<TrackBallEventDrawer<RenderAppT>&>,
        "TrackBallEventDrawer& does not satisfy the ViewProjectionConcept");
    static_assert(
        ViewProjectionConcept<const TrackBallEventDrawer<RenderAppT>&>,
        "const TrackBallEventDrawer& does not satisfy the "
        "ViewProjectionConcept");
    static_assert(
        ViewProjectionConcept<TrackBallEventDrawer<RenderAppT>&&>,
        "TrackBallEventDrawer&& does not satisfy the ViewProjectionConcept");
#endif
}

#endif // VIEW_PROJECTION_H
