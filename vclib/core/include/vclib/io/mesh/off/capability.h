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

#ifndef VCL_IO_MESH_OFF_CAPABILITY_H
#define VCL_IO_MESH_OFF_CAPABILITY_H

#include <vclib/io/file_format.h>

#include <vclib/space/complex.h>

namespace vcl {

constexpr FileFormat offFileFormat()
{
    return FileFormat("off", "OFF Object File Format .off");
}

inline MeshInfo offFormatCapability()
{
    MeshInfo info;

    info.setPolygonMesh();

    info.setVertices();
    info.setFaces();

    info.setPerVertexPosition();
    info.setPerVertexNormal();
    info.setPerVertexColor();
    info.setPerVertexTexCoord();

    info.setPerFaceVertexReferences();
    info.setPerFaceColor();

    return info;
}

} // namespace vcl

#endif // VCL_IO_MESH_OFF_CAPABILITY_H
