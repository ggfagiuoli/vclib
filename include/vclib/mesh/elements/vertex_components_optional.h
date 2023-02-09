/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
 * Alessandro Muntoni                                                        *
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

#ifndef VCL_MESH_ELEMENTS_VERTEX_COMPONENTS_OPTIONAL_H
#define VCL_MESH_ELEMENTS_VERTEX_COMPONENTS_OPTIONAL_H

#include "../components/vertical/optional/optional_adjacent_edges.h"
#include "../components/vertical/optional/optional_adjacent_faces.h"
#include "../components/vertical/optional/optional_adjacent_vertices.h"

namespace vcl::vert {

/* Port OptionalAdjacentEdges class into vert namespace */
template<typename EdgeType, typename VertexType>
using OptionalAdjacentEdges = comp::OptionalAdjacentEdges<EdgeType, -1, VertexType>;

/* Port OptionalAdjacentFaces class into vert namespace */
template<typename FaceType, typename VertexType>
using OptionalAdjacentFaces = comp::OptionalAdjacentFaces<FaceType, -1, VertexType>;

/* Port OptionalAdjacentVertices class into vert namespace */
template<typename VertexType>
using OptionalAdjacentVertices = comp::OptionalAdjacentVertices<VertexType, VertexType>;

/* Port VerticalComponent class into vert namespace */
template<typename VertexType>
using VerticalComponent = comp::VerticalComponent<VertexType>;

} // namespace vcl::vert

#endif // VCL_MESH_ELEMENTS_VERTEX_COMPONENTS_OPTIONAL_H
