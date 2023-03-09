/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
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

#ifndef VCL_SPACE_SAMPLER_MESH_SAMPLER_H
#define VCL_SPACE_SAMPLER_MESH_SAMPLER_H

#include <vclib/concept/space/sampler.h>
#include <vclib/mesh/requirements.h>

namespace vcl {

template<MeshConcept MeshType>
class MeshSampler
{
public:
	using PointType  = typename MeshType::VertexType::CoordType;
	using ScalarType = typename PointType::ScalarType;

	MeshSampler();

	const MeshType& samples() const;

	void clear();
	void resize(uint n);
	void reserve(uint n);

	void add(const PointType& p);
	void set(uint i, const PointType& p);

	template<MeshConcept OMeshType>
	void add(const typename OMeshType::VertexType& v, const OMeshType& m);

	template<MeshConcept OMeshType>
	void set(uint i, const typename OMeshType::VertexType& v, const OMeshType& m);

	template<EdgeMeshConcept OMeshType>
	void add(
		const typename OMeshType::EdgeType& e,
		const OMeshType&,
		double u,
		bool   copyScalar = true);

	template<EdgeMeshConcept OMeshType>
	void set(
		uint i,
		const typename OMeshType::EdgeType& e,
		const OMeshType&,
		double u,
		bool   copyScalar = true);

	template<FaceMeshConcept OMeshType>
	void add(
		const typename OMeshType::FaceType& f,
		const OMeshType&,
		bool copyNormal = false,
		bool copyScalar = true);

	template<FaceMeshConcept OMeshType>
	void set(
		uint i,
		const typename OMeshType::FaceType& f,
		const OMeshType&,
		bool copyNormal = false,
		bool copyScalar = true);

	template<FaceMeshConcept OMeshType>
	void add(
		const typename OMeshType::FaceType& f,
		const OMeshType&,
		const std::vector<ScalarType>&      barCoords,
		bool                                copyNormal = false,
		bool                                copyScalar = true);

	template<FaceMeshConcept OMeshType>
	void set(
		uint                                i,
		const typename OMeshType::FaceType& f,
		const OMeshType&,
		const std::vector<ScalarType>&      barCoords,
		bool                                copyNormal = false,
		bool                                copyScalar = true);

	template<FaceMeshConcept OMeshType>
	void add(
		const typename OMeshType::FaceType&  f,
		const OMeshType&,
		const PointType&                     barCoords,
		bool                                 copyNormal = false,
		bool                                 copyScalar = true);

	template<FaceMeshConcept OMeshType>
	void set(
		uint i,
		const typename OMeshType::FaceType&  f,
		const OMeshType&,
		const PointType&                     barCoords,
		bool                                 copyNormal = false,
		bool                                 copyScalar = true);

private:

	template<FaceConcept FaceType>
	void copyComponents(uint vi, const FaceType&  f, bool copyNormal, bool copyScalar);

	void setBirthElement(uint vi, const std::string& key, uint value);

	MeshType m;
};

// makes sure that the MeshSampler satisfies SamplerConcept
static_assert(
	FaceSamplerConcept<MeshSampler<internal::TMPSimplePolyMesh>>,
	"MeshSampler is not a valid FaceSampler");

} // namespace vcl

#include "mesh_sampler.cpp"

#endif // VCL_SPACE_SAMPLER_MESH_SAMPLER_H
