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

#include <vclib/imgui/mesh_viewer_imgui_drawer.h>
#include <vclib/imgui/imgui_drawer.h>
#include "../imgui_texture_getter_979.h"

#include <vclib/glfw/window_manager.h>
#include <vclib/render/canvas.h>
#include <vclib/render/render_app.h>
#include "../drawable_mesh_979.h"
#include <vclib/render/drawable/drawable_object_vector.h>

#include <vclib/algorithms/mesh/update/color.h>
#include <vclib/algorithms/mesh/update/normal.h>
#include <vclib/io.h>
#include <vclib/meshes.h>

template <typename MeshType>
vcl::DrawableMeshBGFX979<MeshType> getDrawableMesh979(
    std::string filename              = "bimba.obj",
    bool        fromVCLibExamplesPath = true)
{
    if (fromVCLibExamplesPath) {
        filename = VCLIB_EXAMPLE_MESHES_PATH "/" + filename;
    }

    MeshType m = vcl::loadMesh<MeshType>(filename);
    vcl::updatePerVertexAndFaceNormals(m);

    // enable the vertex color of the mesh and set it to gray
    if (!m.isPerVertexColorEnabled()) {
        m.enablePerVertexColor();
        vcl::setPerVertexColor(m, vcl::Color::Gray);
    }

    // create a MeshRenderSettings object, that allows to set the rendering
    // options of the mesh
    // default is what we want: color per vertex, smooth shading, no wireframe
    vcl::MeshRenderSettings settings(m);

    // create a DrawableMesh object from the mesh
    vcl::DrawableMeshBGFX979<MeshType> drawable(m);

    // set the settings to the drawable mesh
    drawable.setRenderSettings(settings);

    return drawable;
}

int main(void)
{
    using MeshType = vcl::TriMesh;

    using RA = vcl::RenderApp<
        vcl::glfw::WindowManager,
        vcl::Canvas,
        vcl::imgui::ImGuiDrawer,
        ImguiTextureGetter979Wrapper<MeshType>::ImGuiTextureGetter979,
        vcl::imgui::MeshViewerDrawerImgui>;
    
    RA tw("Selection", 1024, 768);

    std::shared_ptr<vcl::DrawableObjectVector> vec =
        std::make_shared<vcl::DrawableObjectVector>();
    tw.setDrawableObjectVector(vec);

    vcl::DrawableMeshBGFX979<MeshType> msh = getDrawableMesh979<MeshType>();

    tw.pushDrawableObject(std::move(msh));
    tw.setMesh(tw.drawableObjectVector(), 0);

    tw.fitScene();

    tw.show();

    return 0;
}
