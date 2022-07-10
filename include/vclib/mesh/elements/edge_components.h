/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual Computing Library                                        o     o   *
 *                                                                 _  O  _   *
 * Copyright(C) 2021-2022                                           \/)\/    *
 * Visual Computing Lab                                            /\/|      *
 * ISTI - Italian National Research Council                           |      *
 *                                                                    \      *
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

#ifndef VCL_MESH_ELEMENTS_EDGE_COMPONENTS_H
#define VCL_MESH_ELEMENTS_EDGE_COMPONENTS_H

#include "../components/horizontal/adjacent_edges.h"
#include "../components/horizontal/adjacent_faces.h"
#include "../components/horizontal/bit_flags.h"
#include "../components/horizontal/color.h"
#include "../components/horizontal/mark.h"
#include "../components/horizontal/scalar.h"
#include "../components/horizontal/vertex_references.h"

namespace vcl::edge {

/* Port AdjacentEdges class into edge namespace */
template<typename EdgeType>
using AdjacentEdges = comp::AdjacentEdges<EdgeType, -1>;

/* Port AdjacentFaces class into edge namespace */
template<typename FaceType>
using AdjacentFaces = comp::AdjacentFaces<FaceType, -1>;

/* Port BitFlags class into edge namespace */
using BitFlags = comp::BitFlags;

template<typename EdgeType>
bool constexpr hasBitFlags()
{
	return comp::hasBitFlags<EdgeType>();
}

/* Port Color class into edge namespace */
using Color = comp::Color;

template<typename EdgeType>
bool constexpr hasColor()
{
	return comp::hasColor<EdgeType>();
}

/* Port Mark class into edge namespace */
using Mark = comp::Mark;

template<typename EdgeType>
bool constexpr hasMark()
{
	return comp::hasMark<EdgeType>();
}

/* Port Scalar class into edge namespace */
template<typename ScalarType>
using Scalar = comp::Scalar<ScalarType>;

using Scalarf = comp::Scalarf;
using Scalard = comp::Scalard;

template<typename EdgeType>
bool constexpr hasScalar()
{
	return comp::hasScalar<EdgeType>();
}

/* Port VertexReferences class into edge namespace */
template<typename VertexType>
using VertexReferences = comp::VertexReferences<VertexType, 2>;

template<typename EdgeType>
bool constexpr hasVertexReferences()
{
	return comp::hasVertexReferences<EdgeType>();
}

} // namespace vcl::edge

#endif // VCL_MESH_ELEMENTS_EDGE_COMPONENTS_H
