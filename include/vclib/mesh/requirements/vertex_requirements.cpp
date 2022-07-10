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

#include "vertex_requirements.h"

#include <vclib/exception/mesh_exception.h>

#include "../containers/vertex_container.h"

namespace vcl {

/********************
 * is/has functions *
 ********************/

/**
 * @brief Returns `true` if the given mesh has its Vertex Container compact.
 * Returns `false` if the mesh has no Vertex Container.
 * @param[in] m: input mesh on which test if the its Vertex Container is compact.
 * @return `true` if the Vertex Container of the mesh is compact.
 */
template<typename MeshType>
bool isVertexContainerCompact(const MeshType& m)
{
	if constexpr (HasVertices<MeshType>) {
		return (m.vertexNumber() == m.vertexContainerSize());
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool isPerVertexAdjacentFacesEnabled(const MeshType& m)
{
	if constexpr (HasVertices<MeshType>) {
		if constexpr (vcl::vert::HasOptionalAdjacentFaces<typename MeshType::VertexType>) {
			return m.isPerVertexAdjacentFacesEnabled();
		}
		else {
			return vcl::vert::HasAdjacentFaces<typename MeshType::VertexType>;
		}
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool enableIfPerVertexAdjacentFacesOptional(MeshType& m)
{
	if constexpr (HasPerVertexAdjacentFaces<MeshType>) {
		if constexpr(vcl::vert::HasOptionalAdjacentFaces<typename MeshType::VertexType>) {
			m.enablePerVertexAdjacentFaces();
		}
		return true;
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool isPerVertexAdjacentVerticesEnabled(const MeshType& m)
{
	if constexpr (HasVertices<MeshType>) {
		if constexpr (vcl::vert::HasOptionalAdjacentVertices<typename MeshType::VertexType>) {
			return m.isPerVertexAdjacentVerticesEnabled();
		}
		else {
			return vcl::vert::HasAdjacentVertices<typename MeshType::VertexType>;
		}
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool enableIfPerVertexAdjacentVerticesOptional(MeshType& m)
{
	if constexpr (HasPerVertexAdjacentVertices<MeshType>) {
		if constexpr(vcl::vert::HasOptionalAdjacentVertices<typename MeshType::VertexType>) {
			m.enablePerVertexAdjacentVertices();
		}
		return true;
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool isPerVertexColorEnabled(const MeshType& m)
{
	if constexpr (HasVertices<MeshType>) {
		if constexpr (vcl::vert::HasOptionalColor<typename MeshType::VertexType>) {
			return m.isPerVertexColorEnabled();
		}
		else {
			return vcl::vert::HasColor<typename MeshType::VertexType>;
		}
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool enableIfPerVertexColorOptional(MeshType& m)
{
	if constexpr (HasPerVertexColor<MeshType>) {
		if constexpr(vcl::vert::HasOptionalColor<typename MeshType::VertexType>) {
			m.enablePerVertexColor();
		}
		return true;
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool isPerVertexMarkEnabled(const MeshType& m)
{
	if constexpr (HasVertices<MeshType>) {
		if constexpr (vcl::vert::HasOptionalMark<typename MeshType::VertexType>) {
			return m.isPerVertexMarkEnabled();
		}
		else {
			return vcl::vert::HasMark<typename MeshType::VertexType>;
		}
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool enableIfPerVertexMarkOptional(MeshType& m)
{
	if constexpr (HasPerVertexMark<MeshType>) {
		if constexpr(vcl::vert::HasOptionalMark<typename MeshType::VertexType>) {
			m.enablePerVertexMark();
		}
		return true;
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool isPerVertexNormalEnabled(const MeshType& m)
{
	if constexpr (HasVertices<MeshType>) {
		if constexpr (vcl::vert::HasOptionalNormal<typename MeshType::VertexType>) {
			return m.isPerVertexNormalEnabled();
		}
		else {
			return vcl::vert::HasNormal<typename MeshType::VertexType>;
		}
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool enableIfPerVertexNormalOptional(MeshType& m)
{
	if constexpr (HasPerVertexNormal<MeshType>) {
		if constexpr(vcl::vert::HasOptionalNormal<typename MeshType::VertexType>) {
			m.enablePerVertexNormal();
		}
		return true;
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool constexpr hasPerVertexPrincipalCurvature()
{
	if constexpr (hasVertices<MeshType>())  {
		return vcl::vert::hasPrincipalCurvature<typename MeshType::VertexType>();
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool isPerVertexPrincipalCurvatureEnabled(const MeshType& m)
{
	if constexpr (hasVertices<MeshType>()) {
		if constexpr (vcl::vert::hasOptionalPrincipalCurvature<typename MeshType::VertexType>()) {
			return m.isPerVertexPrincipalCurvatureEnabled();
		}
		else {
			return vcl::vert::hasPrincipalCurvature<typename MeshType::VertexType>();
		}
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool enableIfPerVertexPrincipalCurvatureOptional(MeshType& m)
{
	if constexpr (hasPerVertexPrincipalCurvature<MeshType>()) {
		if constexpr(vcl::vert::hasOptionalPrincipalCurvature<typename MeshType::VertexType>()) {
			m.enablePerVertexPrincipalCurvature();
		}
		return true;
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool constexpr hasPerVertexScalar()
{
	if constexpr (hasVertices<MeshType>())  {
		return vcl::vert::hasScalar<typename MeshType::VertexType>();
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool isPerVertexScalarEnabled(const MeshType& m)
{
	if constexpr (hasVertices<MeshType>()) {
		if constexpr (vcl::vert::hasOptionalScalar<typename MeshType::VertexType>()) {
			return m.isPerVertexScalarEnabled();
		}
		else {
			return vcl::vert::hasScalar<typename MeshType::VertexType>();
		}
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool enableIfPerVertexScalarOptional(MeshType& m)
{
	if constexpr (hasPerVertexScalar<MeshType>()) {
		if constexpr(vcl::vert::hasOptionalScalar<typename MeshType::VertexType>()) {
			m.enablePerVertexScalar();
		}
		return true;
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool constexpr hasPerVertexTexCoord()
{
	if constexpr (hasVertices<MeshType>())  {
		return vcl::vert::hasTexCoord<typename MeshType::VertexType>();
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool isPerVertexTexCoordEnabled(const MeshType& m)
{
	if constexpr (hasVertices<MeshType>()) {
		if constexpr (vcl::vert::hasOptionalTexCoord<typename MeshType::VertexType>()) {
			return m.isPerVertexTexCoordEnabled();
		}
		else {
			return vcl::vert::hasTexCoord<typename MeshType::VertexType>();
		}
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool enableIfPerVertexTexCoordOptional(MeshType& m)
{
	if constexpr (hasPerVertexTexCoord<MeshType>()) {
		if constexpr(vcl::vert::hasOptionalTexCoord<typename MeshType::VertexType>()) {
			m.enablePerVertexTexCoord();
		}
		return true;
	}
	else {
		return false;
	}
}

/*********************
 * require functions *
 *********************/

template<typename MeshType>
void requireVertexContainerCompactness(const MeshType& m)
{
	requireVertices<MeshType>();
	if (!isVertexContainerCompact(m))
		throw vcl::MissingCompactnessException("Vertex Container of the Mesh is not compact.");
}

template<typename MeshType>
void requirePerVertexAdjacentFaces(const MeshType& m)
	requires HasPerVertexAdjacentFaces<MeshType>
{
	if (!isPerVertexAdjacentFacesEnabled(m))
		throw vcl::MissingComponentException("Vertex adjacent faces not enabled.");
}

template<typename MeshType>
void requirePerVertexAdjacentVertices(const MeshType& m)
	requires HasPerVertexAdjacentVertices<MeshType>
{
	if (!isPerVertexAdjacentVerticesEnabled(m))
		throw vcl::MissingComponentException("Per vertex adjacent vertices not enabled.");
}

template<typename MeshType>
void requirePerVertexColor(const MeshType& m)
	requires HasPerVertexColor<MeshType>
{
	if (!isPerVertexColorEnabled(m))
		throw vcl::MissingComponentException("Vertex colors not enabled.");
}

template<typename MeshType>
void requirePerVertexMark(const MeshType& m)
	requires HasPerVertexMark<MeshType>
{
	if (!isPerVertexMarkEnabled(m))
		throw vcl::MissingComponentException("Vertex mark not enabled.");
}

template<typename MeshType>
void requirePerVertexNormal(const MeshType& m)
	requires HasPerVertexNormal<MeshType>
{
	if (!isPerVertexNormalEnabled(m))
		throw vcl::MissingComponentException("Vertex normals not enabled.");
}

template<typename MeshType>
void requirePerVertexPrincipalCurvature(const MeshType& m)
{
	requireVertices<MeshType>();
	static_assert(
		hasPerVertexPrincipalCurvature<MeshType>(), "Mesh has no vertex principal curvature.");
	if (!isPerVertexPrincipalCurvatureEnabled(m))
		throw vcl::MissingComponentException("Vertex principal curvature not enabled.");
}

template<typename MeshType>
void requirePerVertexScalar(const MeshType& m)
{
	requireVertices<MeshType>();
	static_assert(hasPerVertexScalar<MeshType>(), "Mesh has no vertex scalars.");
	if (!isPerVertexScalarEnabled(m))
		throw vcl::MissingComponentException("Vertex scalars not enabled.");
}

template<typename MeshType>
void requirePerVertexTexCoord(const MeshType& m)
{
	requireVertices<MeshType>();
	static_assert(hasPerVertexTexCoord<MeshType>(), "Mesh has no vertex texcoords.");
	if (!isPerVertexTexCoordEnabled(m))
		throw vcl::MissingComponentException("Vertex texcoords not enabled.");
}

} // namespace vcl
