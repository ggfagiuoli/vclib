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

#include "face_requirements.h"

#include <vclib/exception/mesh_exception.h>

#include "../containers/face_container.h"

namespace vcl {

/********************
 * is/has functions *
 ********************/

/**
 * @brief Returns `true` if the given mesh has its Face Container compact.
 * Returns `false` if the mesh has no Face Container.
 * @param[in] m: input mesh on which test if the its Face Container is compact.
 * @return `true` if the Face Container of the mesh is compact.
 */
template<typename MeshType>
bool isFaceContainerCompact(const MeshType& m)
{
	if constexpr (HasFaces<MeshType>) {
		return (m.faceNumber() == m.faceContainerSize());
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool isPerFaceAdjacentFacesEnabled(const MeshType& m)
{
	if constexpr (HasFaces<MeshType>) {
		if constexpr (vcl::face::HasOptionalAdjacentFaces<typename MeshType::FaceType>) {
			return m.isPerFaceAdjacentFacesEnabled();
		}
		else {
			return vcl::face::HasAdjacentFaces<typename MeshType::FaceType>;
		}
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool enableIfPerFaceAdjacentFacesOptional(MeshType& m)
{
	if constexpr (HasPerFaceAdjacentFaces<MeshType>) {
		if constexpr(vcl::face::HasOptionalAdjacentFaces<typename MeshType::FaceType>) {
			m.enablePerFaceAdjacentFaces();
		}
		return true;
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool isPerFaceColorEnabled(const MeshType& m)
{
	if constexpr (HasFaces<MeshType>) {
		if constexpr (vcl::face::HasOptionalColor<typename MeshType::FaceType>) {
			return m.isPerFaceColorEnabled();
		}
		else {
			return vcl::face::HasColor<typename MeshType::FaceType>;
		}
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool enableIfPerFaceColorOptional(MeshType& m)
{
	if constexpr (HasPerFaceColor<MeshType>) {
		if constexpr(vcl::face::HasOptionalColor<typename MeshType::FaceType>) {
			m.enablePerFaceColor();
		}
		return true;
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool isPerFaceMarkEnabled(const MeshType& m)
{
	if constexpr (HasFaces<MeshType>) {
		if constexpr (vcl::face::HasOptionalMark<typename MeshType::FaceType>) {
			return m.isPerFaceMarkEnabled();
		}
		else {
			return vcl::face::HasMark<typename MeshType::FaceType>;
		}
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool enableIfPerFaceMarkOptional(MeshType& m)
{
	if constexpr (HasPerFaceMark<MeshType>) {
		if constexpr(vcl::face::HasOptionalMark<typename MeshType::FaceType>) {
			m.enablePerFaceMark();
		}
		return true;
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool isPerFaceNormalEnabled(const MeshType& m)
{
	if constexpr (HasFaces<MeshType>) {
		if constexpr (vcl::face::HasOptionalNormal<typename MeshType::FaceType>) {
			return m.isPerFaceNormalEnabled();
		}
		else {
			return vcl::face::HasNormal<typename MeshType::FaceType>;
		}
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool enableIfPerFaceNormalOptional(MeshType& m)
{
	if constexpr (HasPerFaceNormal<MeshType>) {
		if constexpr(vcl::face::HasOptionalNormal<typename MeshType::FaceType>) {
			m.enablePerFaceNormal();
		}
		return true;
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool constexpr hasPerFacePrincipalCurvature()
{
	if constexpr (hasFaces<MeshType>())  {
		return vcl::face::hasPrincipalCurvature<typename MeshType::FaceType>();
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool isPerFacePrincipalCurvatureEnabled(const MeshType& m)
{
	if constexpr (hasFaces<MeshType>()) {
		if constexpr (vcl::face::hasOptionalPrincipalCurvature<typename MeshType::FaceType>()) {
			return m.isPerFacePrincipalCurvatureEnabled();
		}
		else {
			return vcl::face::hasPrincipalCurvature<typename MeshType::FaceType>();
		}
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool enableIfPerFacePrincipalCurvatureOptional(MeshType& m)
{
	if constexpr (hasPerFacePrincipalCurvature<MeshType>()) {
		if constexpr(vcl::face::hasOptionalPrincipalCurvature<typename MeshType::FaceType>()) {
			m.enablePerFacePrincipalCurvature();
		}
		return true;
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool constexpr hasPerFaceScalar()
{
	if constexpr (hasFaces<MeshType>())  {
		return vcl::face::hasScalar<typename MeshType::FaceType>();
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool isPerFaceScalarEnabled(const MeshType& m)
{
	if constexpr (hasFaces<MeshType>()) {
		if constexpr (vcl::face::hasOptionalScalar<typename MeshType::FaceType>()) {
			return m.isPerFaceScalarEnabled();
		}
		else {
			return vcl::face::hasScalar<typename MeshType::FaceType>();
		}
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool enableIfPerFaceScalarOptional(MeshType& m)
{
	if constexpr (hasPerFaceScalar<MeshType>()) {
		if constexpr(vcl::face::hasOptionalScalar<typename MeshType::FaceType>()) {
			m.enablePerFaceScalar();
		}
		return true;
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool constexpr hasPerFaceWedgeColors()
{
	if constexpr (hasFaces<MeshType>())  {
		return vcl::face::hasWedgeColors<typename MeshType::FaceType>();
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool isPerFaceWedgeColorsEnabled(const MeshType& m)
{
	if constexpr (hasFaces<MeshType>()) {
		if constexpr (vcl::face::hasOptionalWedgeColors<typename MeshType::FaceType>()) {
			return m.isPerFaceWedgeColorsEnabled();
		}
		else {
			return vcl::face::hasWedgeColors<typename MeshType::FaceType>();
		}
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool enableIfPerFaceWedgeColorsOptional(MeshType& m)
{
	if constexpr (hasPerFaceWedgeColors<MeshType>()) {
		if constexpr(vcl::face::hasOptionalWedgeColors<typename MeshType::FaceType>()) {
			m.enablePerFaceWedgeColors();
		}
		return true;
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool constexpr hasPerFaceWedgeTexCoords()
{
	if constexpr (hasFaces<MeshType>())  {
		return vcl::face::hasWedgeTexCoords<typename MeshType::FaceType>();
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool isPerFaceWedgeTexCoordsEnabled(const MeshType& m)
{
	if constexpr (hasFaces<MeshType>()) {
		if constexpr (vcl::face::hasOptionalWedgeTexCoords<typename MeshType::FaceType>()) {
			return m.isPerFaceWedgeTexCoordsEnabled();
		}
		else {
			return vcl::face::hasWedgeTexCoords<typename MeshType::FaceType>();
		}
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool enableIfPerFaceWedgeTexCoordsOptional(MeshType& m)
{
	if constexpr (hasPerFaceWedgeTexCoords<MeshType>()) {
		if constexpr(vcl::face::hasOptionalWedgeTexCoords<typename MeshType::FaceType>()) {
			m.enablePerFaceWedgeTexCoords();
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
void requireFaceContainerCompactness(const MeshType& m)
{
	requireFaces<MeshType>();
	if (!isFaceContainerCompact(m))
		throw vcl::MissingCompactnessException("Face Container of the Mesh is not compact.");
}

template<typename MeshType>
void requirePerFaceAdjacentFaces(const MeshType& m)
	requires HasPerFaceAdjacentFaces<MeshType>
{
	if (!isPerFaceAdjacentFacesEnabled(m))
		throw vcl::MissingComponentException("Per face adjacent faces not enabled.");
}

template<typename MeshType>
void requirePerFaceColor(const MeshType& m)
	requires HasPerFaceColor<MeshType>
{
	if (!isPerFaceColorEnabled(m))
		throw vcl::MissingComponentException("Face colors not enabled.");
}

template<typename MeshType>
void requirePerFaceMark(const MeshType& m)
	requires HasPerFaceMark<MeshType>
{
	if (!isPerFaceMarkEnabled(m))
		throw vcl::MissingComponentException("Face mark not enabled.");
}

template<typename MeshType>
void requirePerFaceNormal(const MeshType& m)
	requires HasPerFaceNormal<MeshType>
{
	if (!isPerFaceNormalEnabled(m))
		throw vcl::MissingComponentException("Face normals not enabled.");
}

template<typename MeshType>
void requirePerFacePrincipalCurvature(const MeshType& m)
{
	requireFaces<MeshType>();
	static_assert(hasPerFacePrincipalCurvature<MeshType>(), "Mesh has no face principal curvature.");
	if (!isPerFacePrincipalCurvatureEnabled(m))
		throw vcl::MissingComponentException("Face principal curvature not enabled.");
}

template<typename MeshType>
void requirePerFaceScalar(const MeshType& m)
{
	requireFaces<MeshType>();
	static_assert(hasPerFaceScalar<MeshType>(), "Mesh has no face scalars.");
	if (!isPerFaceScalarEnabled(m))
		throw vcl::MissingComponentException("Face scalars not enabled.");
}

template<typename MeshType>
void requirePerFaceWedgeColors(const MeshType& m)
{
	requireFaces<MeshType>();
	static_assert(hasPerFaceWedgeColors<MeshType>(), "Mesh has no face wedge colors.");
	if (!isPerFaceWedgeColorsEnabled(m))
		throw vcl::MissingComponentException("Face wedge colors not enabled.");
}

template<typename MeshType>
void requirePerFaceWedgeTexCoords(const MeshType& m)
{
	requireFaces<MeshType>();
	static_assert(hasPerFaceWedgeTexCoords<MeshType>(), "Mesh has no face wedge texcoords.");
	if (!isPerFaceWedgeTexCoordsEnabled(m))
		throw vcl::MissingComponentException("Face wedge texcoords not enabled.");
}

} // namespace vcl
