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

#include "get_drawable_mesh.h"

#include <vclib/render/automation/actions.h>
#include <vclib/render/automation/actions/automation_action_factory.h>
#include <vclib/render/automation/metrics.h>
#include <vclib/render/automation/printers.h>
#include <vclib/render/canvas.h>
#include <vclib/render/drawers/benchmark_drawer.h>
#include <vclib/render/drawers/benchmark_viewer_drawer.h>
#include <vclib/render/drawers/viewer_drawer.h>
#include <vclib/render/render_app.h>

#include <vclib/imgui/imgui_drawer.h>
#include <vclib/imgui/mesh_viewer_imgui_drawer.h>

#include <vclib/qt/widget_manager.h>

#include <QApplication>

using ViewerWidget = vcl::RenderApp<
    vcl::qt::WidgetManager,
    vcl::Canvas,
    vcl::BenchmarkViewerDrawer,
    vcl::imgui::ImGuiDrawer>;

int main(int argc, char** argv)
{
#ifdef VCLIB_RENDER_BACKEND_BGFX

    vcl::Context::setResetFlags(BGFX_RESET_NONE);

#endif

    QApplication app(argc, argv);

    ViewerWidget tw("Mesh Viewer ImGui Qt");

    // load and set up a drawable mesh
    vcl::DrawableMesh<vcl::TriMesh> drawable =
        getDrawableMesh<vcl::TriMesh>("bunny.obj");

    // An automation action factory, to shorten the length of Automation
    // declarations
    vcl::AutomationActionFactory<ViewerWidget> aaf;

    // add the drawable mesh to the scene
    // the viewer will own **a copy** of the drawable mesh
    tw.pushDrawableObject(drawable);

    // Repeat all automations 2 times
    tw.setRepeatTimes(3);

    // Change the measured metric to FPS
    tw.addAutomation(
        aaf.MetricChanger(vcl::FpsBenchmarkMetric()),
        // We don't want to measure the metric for this automation
        false);

    // Rotate and scale at the same time for 2 seconds
    tw.addAutomation(aaf.TimeLimited(
        aaf.Simultaneous(
            {aaf.Rotation(5.f, {0.f, 0.f, 1.f}),
             aaf.ChangeScaleAbsolute(-0.2f)}),
        2.f));

    // Change the measured metric to time (seconds)
    tw.addAutomation(aaf.MetricChanger(vcl::TimeBenchmarkMetric()), false);

    // Rotate for 5000 frames and then scale for 5000 frames
    tw.addAutomation(aaf.Sequential(
        {aaf.FrameLimited(aaf.Rotation(5.f, {0.f, -1.f, 0.f}), 5000.f),
         aaf.FrameLimited(aaf.ChangeScaleAbsolute(1.0f), 5000.f)}));

    // Print the results in a json file
    tw.setPrinter(vcl::JsonBenchmarkPrinter("./test_out.json"));

    tw.fitScene();

    tw.show();

    return app.exec();
}
