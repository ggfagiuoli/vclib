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

#ifndef VCL_MESH_ELEMENTS_FACE_COMPONENTS_H
#define VCL_MESH_ELEMENTS_FACE_COMPONENTS_H

#include "../components/adjacent_edges.h"
#include "../components/adjacent_faces.h"
#include "../components/bit_flags.h"
#include "../components/color.h"
#include "../components/face_half_edge_reference.h"
#include "../components/mark.h"
#include "../components/normal.h"
#include "../components/parent_mesh_pointer.h"
#include "../components/polygon_bit_flags.h"
#include "../components/principal_curvature.h"
#include "../components/scalar.h"
#include "../components/triangle_bit_flags.h"
#include "../components/vertex_references.h"
#include "../components/wedge_colors.h"
#include "../components/wedge_tex_coords.h"

namespace vcl::face {

/* Port AdjacentEdges class into face namespace */
template<typename EdgeType>
using AdjacentPolygonEdges = comp::AdjacentEdges<EdgeType, -1, void, true>;

template<typename EdgeType>
using AdjacentTriangleEdges = comp::AdjacentEdges<EdgeType, 3, void, true>;

/* Port AdjacentFaces class into face namespace */
template<typename FaceType>
using AdjacentPolygons = comp::AdjacentFaces<FaceType, -1, void, true>;

template<typename FaceType>
using AdjacentTriangles = comp::AdjacentFaces<FaceType, 3, void, true>;

/* Port BitFlags class into face namespace */
using BitFlags = comp::BitFlags<void, true>;

/* Port Color class into face namespace */
using Color = comp::Color<void, true>;

/* Port Half Edge reference into face namespace*/
template<typename HalfEdge>
using HalfEdgeReference = comp::FaceHalfEdgeReference<HalfEdge, void, true>;

/* Port Mark class into face namespace */
using Mark = comp::Mark<void, true>;

/* Port Normal classes into face namespace */
template<typename ScalarType, int N>
using Normal = comp::Normal<ScalarType, N, void, true>;

template<typename ScalarType>
using Normal3 = comp::Normal3<ScalarType, void, true>;

using Normal3f = comp::Normal3f<void, true>;
using Normal3d = comp::Normal3d<void, true>;

/* Port ParentMeshPointer class into vert namespace */
template<typename MeshType>
using ParentMeshPointer = comp::ParentMeshPointer<MeshType>;

/* Port PolygonBitFlags class into face namespace */
using PolygonBitFlags = comp::PolygonBitFlags<void, true>;

/* Port PrincipalCurvature class into face namespace */
template<typename FaceType>
using PrincipalCurvature = comp::PrincipalCurvature<FaceType, void, true>;

using PrincipalCurvaturef = comp::PrincipalCurvaturef<void, true>;
using PrincipalCurvatured = comp::PrincipalCurvatured<void, true>;

/* Port Scalar class into face namespace */
template<typename ScalarType>
using Scalar = comp::Scalar<ScalarType, void, true>;

using Scalarf = comp::Scalarf<void, true>;
using Scalard = comp::Scalard<void, true>;

/* Port TriangleBitFlags class into face namespace */
using TriangleBitFlags = comp::TriangleBitFlags<void, true>;

/* Port VertexReferences class into face namespace */
template<typename Vertex, int N>
using VertexReferences = comp::VertexReferences<Vertex, N, void, true>;

template<typename Vertex>
using TriangleVertexRefs = comp::VertexReferences<Vertex, 3, void, true>;

template<typename Vertex>
using PolygonVertexRefs = comp::VertexReferences<Vertex, -1, void, true>;

/* Port WedgeColors class into face namespace */
using PolygonWedgeColors = comp::WedgeColors<-1>;

using TriangleWedgeColors = comp::WedgeColors<3>;

/* Port WedgeTexCoords class into face namespace */
template<typename ScalarType>
using PolygonWedgeTexCoords = comp::WedgeTexCoords<ScalarType, -1, void, true>;

template<typename ScalarType>
using TriangleWedgeTexCoords = comp::WedgeTexCoords<ScalarType, 3, void, true>;

using PolygonWedgeTexCoordsf = PolygonWedgeTexCoords<float>;
using PolygonWedgeTexCoordsd = PolygonWedgeTexCoords<double>;
using TriangleWedgeTexCoordsf = TriangleWedgeTexCoords<float>;
using TriangleWedgeTexCoordsd = TriangleWedgeTexCoords<double>;

} // namespace vcl::face

#endif // VCL_MESH_ELEMENTS_FACE_COMPONENTS_H
