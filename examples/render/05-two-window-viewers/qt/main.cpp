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

#include <vclib/qt/viewer_widget.h>

#include <QApplication>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    vcl::qt::ViewerWidget viewer1("Viewer Qt 1");
    vcl::qt::ViewerWidget viewer2("Viewer Qt 2");

    // load and set up a drawable mesh
    vcl::DrawableMesh<vcl::TriMesh> drawable = getDrawableMesh<vcl::TriMesh>();

    // FIXME #1: Fix crash on Windows/DirectX when updating buffers
    // drawable.color() = vcl::Color::Yellow;
    // drawable.updateBuffers();

    auto mrs = drawable.renderSettings();
    // mrs.setSurfaceColorPerMesh();
    mrs.setSurfaceShadingFlat();
    drawable.setRenderSettings(mrs);

    // add the drawable mesh to the scene
    // the viewer will own **a copy** of the drawable mesh
    viewer1.pushDrawableObject(drawable);

    viewer1.fitScene();

    viewer1.show();

    mrs.setSurfaceShadingSmooth();
    drawable.setRenderSettings(mrs);

    // add the drawable mesh to the scene
    // the viewer will own **a copy** of the drawable mesh
    viewer2.pushDrawableObject(drawable);

    viewer2.fitScene();

    viewer2.show();

    return app.exec();
}
