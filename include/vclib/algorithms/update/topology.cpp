/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual and Computer Graphics Library                            o     o   *
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

#include "topology.h"

#include <vclib/algorithms/internal/edge_sorterer.h>

#include <vclib/mesh/requirements.h>

namespace vcl {

/**
 * @brief Clears the adjacent faces of each vertex of the mesh.
 *
 * Since the number of adjacent faces per vertex is dynamic, at the end of this function each
 * vertex will have 0 adjacent Faces.
 *
 * Requirements:
 * - Mesh:
 *   - Vertices:
 *     - AdjacentFaces
 *
 * @param m: the mesh on which clear the per vertex adjacent faces.
 */
template<typename MeshType>
void clearPerVertexAdjacentFaces(MeshType& m)
{
	vcl::requirePerVertexAdjacentFaces(m);

	using VertexType = typename MeshType::Vertex;

	for (VertexType& v : m.vertices()) {
		v.clearAdjFaces();
	}
}

/**
 * @brief Updates the adjacent faces of each face of the mesh.
 *
 * Requirements:
 * - Mesh:
 *   - Vertices:
 *     - AdjacentFaces
 *   - Faces
 *
 * @param m:  the mesh on which update the per vertex adjacent faces.
 */
template<typename MeshType>
void updatePerVertexAdjacentFaces(MeshType& m)
{
	clearPerVertexAdjacentFaces(m);

	vcl::requireFaces<MeshType>();

	using VertexType = typename MeshType::Vertex;
	using FaceType   = typename MeshType::Face;

	for (VertexType& v : m.vertices()) {
		v.clearAdjFaces();
	}

	for (FaceType& f : m.faces()) {
		for (VertexType* v : f.vertices()) {
			v->pushAdjFace(&f);
		}
	}
}

/**
 * @brief Clears the adjacent faces of each face of the mesh.
 *
 * Since the number of adjacent faces per face is tied to the number of vertices of the face, at
 * the end of this function each face will have f->vertexNumber() adjacent faces set to nullptr.
 *
 * Requirements:
 * - Mesh:
 *   - Faces:
 *     - AdjacentFaces
 *
 * @param m:  the mesh on which clear the per face adjacent faces.
 */
template<typename MeshType>
void clearPerFaceAdjacentFaces(MeshType& m)
{
	vcl::requirePerFaceAdjacentFaces(m);

	using FaceType = typename MeshType::Face;

	for (FaceType& f : m.faces()) {
		for (FaceType*& adjF : f.adjFaces()) {
			adjF = nullptr;
		}
	}
}

/**
 * @brief Updates the per face adjacent face component.
 *
 * All the faces that does not have an adjacent face will have the adjacent face set to nullptr.
 *
 * If there are non-manifold edges (edges on which there are more than two incident faces), then
 * a chain-link composed of all the incident faces is built.
 *
 * Assuming that we have 3 faces f0, f1 and f2 on the same edge composed of vi and vj, we can define
 * the edge indices in the three faces as:
 *
 * @code{.cpp}
 * e0 = f0.indexOfEdge(vi, vj);
 * e1 = f1.indexOfEdge(vi, vj);
 * e2 = f2.indexOfEdge(vi, vj);
 * @endcode
 *
 * The adjacencies will cycle like:
 *
 * @code{.cpp}
 * f0->adjFace(e0) == f1;
 * f1->adjFace(e1) == f2;
 * f2->adjFace(e2) == f0;
 * @endcode
 *
 * or a similar permutation of the faces.
 *
 * In general, an edge is non-manifold if it is true the expression:
 *
 * @code{.cpp}
 * fj = fi->adjFace(ei);
 * fj->adjFace(ej) != f1; // if true, the edge is non-manifold
 * @endcode
 *
 *
 * Requirements:
 * - Mesh:
 *   - Faces:
 *     - AdjacentFaces
 *
 * @param m:  the mesh on which update the per face adjacent faces.
 */
template<typename MeshType>
void updatePerFaceAdjacentFaces(MeshType& m)
{
	vcl::requirePerFaceAdjacentFaces(m);

	// vector that contains edges sorted trough unordered vertex pointers
	// it contains clusters of "same" edges, but each one of them has its face pointer
	// note that in case on non-manifold mesh, clusters may be of size >= 2
	std::vector<internal::EdgeSorterer<MeshType>> vec = internal::fillAndSortEdgeVector(m);

	if (vec.size() > 0) {
		// in this loop, base will point to the first element of a cluster of edges
		for (auto base = vec.begin(); base != vec.end(); /* increment of clusters into loop*/) {
			auto first = base; // remember the first to set adj to the last

			// i and j will increment together, and if i == j, i will be adj to j, but j will not be
			// adj to i (to manage non manifold edges and make cyclic adj on the same edge)
			auto i = base;
			auto j = i + 1;

			if (*i != *j) { // case of cluster composed of one element. adj of i is nullptr
				i->f->adjFace(i->e) = nullptr;
			}
			else { // at least two edges in the cluster
				while (*i == *j && j != vec.end()) {
					i->f->adjFace(i->e) = j->f;
					++i;
					++j;
				}
				// i now is the last element that was equal to first
				i->f->adjFace(i->e) = first->f;
			}

			// j is the first different edge from first (or it is vec.end()!)
			base = j;
		}
	}
}

} // namespace vcl
