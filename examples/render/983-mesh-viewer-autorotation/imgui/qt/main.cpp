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

#include <vclib/imgui/mesh_viewer_imgui_drawer.h>

#include <vclib/imgui/imgui_drawer.h>

#include <vclib/qt/widget_manager.h>
#include <vclib/render/canvas.h>
#include <vclib/render/drawers/viewer_drawer.h>
#include <vclib/render/render_app.h>

#include <QApplication>

#include "../../automation_imgui_drawer.h"
#include "../../rotation_automation_action.h"
#include "../../benchmark_drawer.h"
#include "../../time_limited_automation_action.h"
#include "../../per_frame_rotation_automation_action.h"
#include "../../automation_action_group.h"
#include "../../scale_automation_action.h"
#include "../../per_frame_scale_automation_action.h"
#include "../../frame_limited_automation_action.h"
#include "../../mesh_changer_automation_action.h"
#include "../../time_delay_automation_action.h"
#include "../../frame_delay_automation_action.h"

using ViewerWidget = vcl::RenderApp<
    vcl::qt::WidgetManager,
    vcl::Canvas,
    vcl::imgui::ImGuiDrawer,
    vcl::ViewerDrawer,
    BenchmarkDrawer>;

int main(int argc, char** argv)
{
    vcl::Context::setResetFlags(BGFX_RESET_NONE);

    QApplication app(argc, argv);

    ViewerWidget tw("Mesh Viewer ImGui Qt");

    // load and set up a drawable mesh
    vcl::DrawableMesh<vcl::TriMesh> drawable = getDrawableMesh<vcl::TriMesh>("bunny.obj");
    vcl::DrawableMesh<vcl::TriMesh> drawable2 = getDrawableMesh<vcl::TriMesh>("ESTE_PRINT.ply");

    // add the drawable mesh to the scene
    // the viewer will own **a copy** of the drawable mesh
    tw.pushDrawableObject(drawable);
    tw.setRepeatTimes(2);
    tw.addAutomation(
        TimeLimitedAutomationAction(
            AutomationActionGroupBuilder()
            .addAutomation(
                RotationAutomationAction(
                    &tw, 5.f, {0.f,0.f,1.f}
                )
            )
            .addAutomation(
                ScaleAutomationAction(
                    &tw, -0.01f
                )
            )
            .finish(),
            1.f
        )
    );
    tw.addAutomation(
        TimeLimitedAutomationAction(
            AutomationActionGroupBuilder()
            .addAutomation(
                RotationAutomationAction(
                    &tw, 5.f, {0.f,-1.f,0.f}
                )
            )
            .addAutomation(
                ScaleAutomationAction(
                    &tw, -0.01f
                )
            )
            .finish(),
            1.f
        )
    );
    tw.addAutomation(
        MeshChangerAutomationAction(
            &tw, std::move(drawable2)
        ),
        false
    );
    
    tw.fitScene();

    tw.show();

    return app.exec();
}
