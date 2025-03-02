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

#ifndef VCL_BINDINGS_MESHES_TRI_MESH_VERTEX_CONTAINER_H
#define VCL_BINDINGS_MESHES_TRI_MESH_VERTEX_CONTAINER_H

#include <vclib/meshes.h>

#include <pybind11/pybind11.h>

namespace vcl::bind {

void initTriMeshVertexContainer(pybind11::class_<vcl::TriMesh>& c);

} // namespace vcl::bind

#endif // VCL_BINDINGS_MESHES_TRI_MESH_VERTEX_CONTAINER_H
