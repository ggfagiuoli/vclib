/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "face_requirements.h"

#include <mgp/exception/mesh_exception.h>

#include "../container/face_container.h"

namespace mgp {

/********************
 * is/has functions *
 ********************/

template<typename MeshType>
bool constexpr hasPerFaceNormal()
{
	return hasFaces<MeshType>() && (mgp::face::hasNormal<typename MeshType::FaceType>() ||
									mgp::face::hasOptionalNormal<typename MeshType::FaceType>());
}

template<typename MeshType>
bool constexpr hasPerFaceNormal(const MeshType&)
{
	return hasPerFaceNormal<MeshType>();
}

template<typename MeshType>
bool isPerFaceNormalEnabled(const MeshType& m)
{
	if constexpr (hasFaces<MeshType>() && mgp::face::hasNormal<typename MeshType::FaceType>()) {
		return true;
	}
	else if constexpr (
		hasFaces<MeshType>() && mgp::face::hasOptionalNormal<typename MeshType::FaceType>()) {
		return m.isPerFaceNormalEnabled();
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool enableIfPerFaceNormalOptional(MeshType& m)
{
	if constexpr (
		hasFaces<MeshType>() && mgp::face::hasOptionalNormal<typename MeshType::FaceType>()) {
		m.enablePerFaceNormal();
		return true;
	}
	else {
		return hasPerFaceNormal(m);
	}
}

template<typename MeshType>
bool constexpr hasPerFaceColor()
{
	return hasFaces<MeshType>() && (mgp::face::hasColor<typename MeshType::FaceType>() ||
									mgp::face::hasOptionalColor<typename MeshType::FaceType>());
}

template<typename MeshType>
bool constexpr hasPerFaceColor(const MeshType&)
{
	return hasPerFaceColor<MeshType>();
}

template<typename MeshType>
bool isPerFaceColorEnabled(const MeshType& m)
{
	if constexpr (hasFaces<MeshType>() && mgp::face::hasColor<typename MeshType::FaceType>()) {
		return true;
	}
	else if constexpr (
		hasFaces<MeshType>() && mgp::face::hasOptionalColor<typename MeshType::FaceType>()) {
		return m.isPerFaceColorEnabled();
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool enableIfPerFaceColorOptional(MeshType& m)
{
	if constexpr (
		hasFaces<MeshType>() && mgp::face::hasOptionalColor<typename MeshType::FaceType>()) {
		m.enablePerFaceColor();
		return true;
	}
	else {
		return hasPerFaceColor(m);
	}
}

template<typename MeshType>
bool constexpr hasPerFaceScalar()
{
	return hasFaces<MeshType>() && (mgp::face::hasScalar<typename MeshType::FaceType>() ||
									mgp::face::hasOptionalScalar<typename MeshType::FaceType>());
}

template<typename MeshType>
bool constexpr hasPerFaceScalar(const MeshType&)
{
	return hasPerFaceScalar<MeshType>();
}

template<typename MeshType>
bool isPerFaceScalarEnabled(const MeshType& m)
{
	if constexpr (hasFaces<MeshType>() && mgp::face::hasScalar<typename MeshType::FaceType>()) {
		return true;
	}
	else if constexpr (
		hasFaces<MeshType>() && mgp::face::hasOptionalScalar<typename MeshType::FaceType>()) {
		return m.isPerFaceScalarEnabled();
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool enableIfPerFaceScalarOptional(MeshType& m)
{
	if constexpr (
		hasFaces<MeshType>() && mgp::face::hasOptionalScalar<typename MeshType::FaceType>()) {
		m.enablePerFaceScalar();
		return true;
	}
	else {
		return hasPerFaceScalar(m);
	}
}

template<typename MeshType>
bool constexpr hasPerFaceAdjacentFaces()
{
	return hasFaces<MeshType>() &&
		   (mgp::face::hasAdjacentFaces<typename MeshType::FaceType>() ||
			mgp::face::hasOptionalAdjacentFaces<typename MeshType::FaceType>());
}

template<typename MeshType>
bool constexpr hasPerFaceAdjacentFaces(const MeshType&)
{
	return hasPerFaceAdjacentFaces<MeshType>();
}

template<typename MeshType>
bool isPerFaceAdjacentFacesEnabled(const MeshType& m)
{
	if constexpr (
		hasFaces<MeshType>() && mgp::face::hasAdjacentFaces<typename MeshType::FaceType>()) {
		return true;
	}
	else if constexpr (
		hasFaces<MeshType>() &&
		mgp::face::hasOptionalAdjacentFaces<typename MeshType::FaceType>()) {
		return m.isPerFaceAdjacentFacesEnabled();
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool enableIfPerFaceAdjacentFacesOptional(MeshType& m)
{
	if constexpr (
		hasFaces<MeshType>() &&
		mgp::face::hasOptionalAdjacentFaces<typename MeshType::FaceType>()) {
		m.enablePerFaceAdjacentFaces();
		return true;
	}
	else {
		return hasPerFaceAdjacentFaces(m);
	}
}

template<typename MeshType>
bool constexpr hasPerFaceCustomProperties()
{
	return hasFaces<MeshType>() && mgp::face::hasCustomProperties<typename MeshType::FaceType>();
}

template<typename MeshType>
bool constexpr hasPerFaceCustomProperties(const MeshType&)
{
	return hasPerFaceCustomProperties<MeshType>();
}

template<typename MeshType>
bool constexpr hasPerFaceMutableBitFlags()
{
	return hasFaces<MeshType>() &&
		   (mgp::face::hasMutableBitFlags<typename MeshType::FaceType>() ||
			mgp::face::hasOptionalMutableBitFlags<typename MeshType::FaceType>());
}

template<typename MeshType>
bool constexpr hasPerFaceMutableBitFlags(const MeshType&)
{
	return hasPerFaceMutableBitFlags<MeshType>();
}

template<typename MeshType>
bool isPerFaceMutableBitFlagsEnabled(const MeshType& m)
{
	if constexpr (
		hasFaces<MeshType>() && mgp::face::hasMutableBitFlags<typename MeshType::FaceType>()) {
		return true;
	}
	else if constexpr (
		hasFaces<MeshType>() &&
		mgp::face::hasOptionalMutableBitFlags<typename MeshType::FaceType>()) {
		return m.isPerFaceMutableBitFlagsEnabled();
	}
	else {
		return false;
	}
}

template<typename MeshType>
bool enableIfPerFaceMutableBitFlagsOptional(MeshType& m)
{
	if constexpr (
		hasFaces<MeshType>() &&
		mgp::face::hasOptionalMutableBitFlags<typename MeshType::FaceType>()) {
		m.enablePerFaceMutableBitFlags();
		return true;
	}
	else {
		return hasPerFaceMutableBitFlags(m);
	}
}

/*********************
 * require functions *
 *********************/

template<typename MeshType>
void requirePerFaceNormal(const MeshType& m)
{
	requireFaces<MeshType>();
	static_assert(hasPerFaceNormal(m), "Mesh has no face normals.");
	if (!isPerFaceNormalEnabled(m))
		throw mgp::MissingPropertyException("Face normals not enabled.");
}

template<typename MeshType>
void requirePerFaceColor(const MeshType& m)
{
	requireFaces<MeshType>();
	static_assert(hasPerFaceColor(m), "Mesh has no face colors.");
	if (!isPerFaceColorEnabled(m))
		throw mgp::MissingPropertyException("Face colors not enabled.");
}

template<typename MeshType>
void requirePerFaceScalar(const MeshType& m)
{
	requireFaces<MeshType>();
	static_assert(hasPerFaceScalar(m), "Mesh has no face scalars.");
	if (!isPerFaceScalarEnabled(m))
		throw mgp::MissingPropertyException("Face scalars not enabled.");
}

template<typename MeshType>
void requirePerFaceAdjacentFaces(const MeshType& m)
{
	requireFaces<MeshType>();
	static_assert(hasPerFaceAdjacentFaces(m), "Mesh has no per face adjacent faces.");
	if (!isPerFaceAdjacentFacesEnabled(m))
		throw mgp::MissingPropertyException("Per face adjacent faces not enabled.");
}

template<typename MeshType>
void constexpr requirePerFaceCustomProperties()
{
	requireFaces<MeshType>();
	static_assert(hasPerFaceCustomProperties<MeshType>(), "Mesh has no face custom properties.");
}

template<typename MeshType>
void constexpr requirePerFaceCustomProperties(const MeshType&)
{
	requirePerFaceCustomProperties<MeshType>();
}

template<typename MeshType>
void requirePerFaceMutableBitFlags(const MeshType& m)
{
	requireFaces<MeshType>();
	static_assert(hasPerFaceMutableBitFlags(m), "Mesh has no per face mutable bit flags.");
	if (!isPerFaceMutableBitFlagsEnabled(m))
		throw mgp::MissingPropertyException("Face mutable bit flags not enabled.");
}

} // namespace mgp
