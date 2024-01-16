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

#ifndef VCL_EXT_BGFX_MINIMAL_VIEWER_H
#define VCL_EXT_BGFX_MINIMAL_VIEWER_H

#include <vclib/math/min_max.h>
#include <vclib/space/box.h>

#include <vclib/gui/desktop_trackball.h>
#include <vclib/render/drawable_object_vector.h>
#include <vclib/render/generic_drawable_mesh.h>

#include "drawable_mesh_shader_program.h"
#include "uniforms/camera_uniforms.h"
#include "uniforms/directional_light_uniforms.h"
#include "uniforms/mesh_render_settings_uniforms.h"

namespace vcl::bgf {

class MinimalViewer : public vcl::DesktopTrackBall<float>
{
    // this Viewer does not normally own this drawList
    std::shared_ptr<DrawableObjectVector> drawList;

    CameraUniforms             cameraUniforms;
    DirectionalLightUniforms   directionalLightUniforms;
    MeshRenderSettingsUniforms meshRenderSettingsUniforms;

    // the program must be created after the uniforms - bgfx issue on OpenGL
    vcl::bgf::DrawableMeshShaderProgram program;

protected:
    using DTB = vcl::DesktopTrackBall<float>;

public:
    MinimalViewer()
    {
        cameraUniforms.updateCamera(DTB::camera());
        directionalLightUniforms.updateLight(DTB::light());
    }

    MinimalViewer(std::shared_ptr<DrawableObjectVector> v) : MinimalViewer()
    {
        setDrawableObjectVector(v);
    }

    std::shared_ptr<const DrawableObjectVector> drawableObjectVector() const
    {
        return drawList;
    }

    void setDrawableObjectVector(std::shared_ptr<DrawableObjectVector> v)
    {
        drawList = v;

        for (DrawableObject* obj : *drawList) {
            obj->init();

            GenericDrawableMesh* mesh = dynamic_cast<GenericDrawableMesh*>(obj);
            if (mesh) {
                mesh->setShaderProgram(program);
            }
        }
    }

    void fitScene()
    {
        Box3d   bb          = drawList->boundingBox();
        Point3f sceneCenter = bb.center().cast<float>();
        float   sceneRadius = bb.diagonal() / 2;

        DTB::setTrackBall(sceneCenter, sceneRadius);
    }

    void draw()
    {
        // This dummy draw call is here to make sure that view 0 is cleared
        // if no other draw calls are submitted to view 0.
        bgfx::touch(0);

        cameraUniforms.updateCamera(DTB::camera());
        cameraUniforms.setUniforms();

        directionalLightUniforms.setUniforms();

        for (DrawableObject* obj : *drawList)
            obj->draw();
    }
};

} // namespace vcl::bgf

#endif // VCL_EXT_BGFX_MINIMAL_VIEWER_H
