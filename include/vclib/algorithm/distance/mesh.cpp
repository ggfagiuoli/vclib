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

#include "mesh.h"

#include <vclib/algorithm/point_sampling.h>
#include <vclib/iterator/pointer_iterator.h>
#include <vclib/space/spatial_data_structures.h>
#include <vclib/misc/parallel.h>

#include <mutex>

namespace vcl {

namespace internal {

template<MeshConcept MeshType, SamplerConcept SamplerType, typename GridType, LoggerConcept  LogType>
HausdorffDistResult hausdorffDist(
	const MeshType& m,
	const SamplerType& s,
	const GridType& g,
	LogType& log)
{
	using PointSampleType = typename SamplerType::PointType;

	HausdorffDistResult res;
	res.histogram = Histogramd(0, m.boundingBox().diagonal() / 100, 100);

	if constexpr (vcl::isLoggerValid<LogType>()) {
		log.log(5, "Computing distances...");
	}

	uint logPerc = 0;
	const uint logPercStep = 10;
	uint logVertStep = s.size() / ((100 / logPercStep) - 1);
	if (logVertStep == 0)
		logVertStep = s.size();

	std::mutex mutex;

	uint ns = 0;
	uint i = 0;
	vcl::parallelFor(s.begin(), s.end(), [&](const PointSampleType& sample){
//	for (uint i = 0; i < s.size(); ++i) {
//		const PointSampleType& sample = s.sample(i);
		double dist;
		const auto iter = g.closestValue(sample, dist);

		if (iter != g.end()) {
			mutex.lock();
			ns++;
			if (dist > res.maxDist)
				res.maxDist = dist;
			if (dist < res.minDist)
				res.minDist = dist;
			res.meanDist += dist;
			res.RMSDist += dist*dist;
			res.histogram.addValue(dist);
			mutex.unlock();
		}

		if constexpr (vcl::isLoggerValid<LogType>()) {
			mutex.lock();
			++i;
			mutex.unlock();
			if (i % logVertStep == 0) {
				mutex.lock();
				logPerc += logPercStep;
				log.log(logPerc, "");
				mutex.unlock();
			}
		}
//	}
	});

	res.meanDist /= ns;
	res.RMSDist = std::sqrt(res.RMSDist / ns);

	return res;
}

template<MeshConcept MeshType, SamplerConcept SamplerType, LoggerConcept  LogType>
HausdorffDistResult samplerMeshHausdorff(
	const MeshType& m,
	const SamplerType& s,
	LogType& log)
	requires(!HasFaces<MeshType>)
{
	using VertexType = typename MeshType::VertexType;
	using VPI = vcl::PointerIterator<typename MeshType::VertexIterator>;

	std::string meshName = "first mesh";
	if constexpr (HasName<MeshType>){
		meshName = m.name();
	}
	if constexpr (vcl::isLoggerValid<LogType>()) {
		log.log(0, "Building Grid on " + meshName + " vertices...");
	}

	vcl::StaticGrid3<const VertexType*> grid(VPI(m.vertexBegin()), VPI(m.vertexEnd()));
	grid.build();

	if constexpr (vcl::isLoggerValid<LogType>()) {
		log.log(5, "Grid built.");
	}

	return hausdorffDist(m, s, grid, log);
}

template<FaceMeshConcept MeshType, SamplerConcept SamplerType, LoggerConcept  LogType>
HausdorffDistResult samplerMeshHausdorff(
	const MeshType& m,
	const SamplerType& s,
	LogType& log)
{
	using VertexType = typename MeshType::VertexType;
	using FaceType   = typename MeshType::FaceType;
	using VPI = vcl::ConstPointerIterator<typename MeshType::ConstVertexIterator>;
	using FPI = vcl::ConstPointerIterator<typename MeshType::ConstFaceIterator>;

	std::string meshName = "first mesh";
	if constexpr (HasName<MeshType>){
		meshName = m.name();
	}
	if (m.faceNumber() == 0) {
		if constexpr (vcl::isLoggerValid<LogType>()) {
			log.log(0, "Building Grid on " + meshName + " vertices...");
		}

		vcl::StaticGrid3<const VertexType*> grid(VPI(m.vertexBegin()), VPI(m.vertexEnd()));
		grid.build();

		if constexpr (vcl::isLoggerValid<LogType>()) {
			log.log(5, "Grid built.");
		}

		return hausdorffDist(m, s, grid, log);
	}
	else {
		if constexpr (vcl::isLoggerValid<LogType>()) {
			log.log(0, "Building Grid on " + meshName + " faces...");
		}

		vcl::StaticGrid3<const FaceType*> grid(FPI(m.faceBegin()), FPI(m.faceEnd()));
		grid.build();

		if constexpr (vcl::isLoggerValid<LogType>()) {
			log.log(5, "Grid built.");
		}

		return hausdorffDist(m, s, grid, log);
	}
}

template<
	MeshConcept    MeshType1,
	MeshConcept    MeshType2,
	SamplerConcept SamplerType,
	LoggerConcept  LogType>
HausdorffDistResult hausdorffDistanceVertexUniformSampling(
	const MeshType1&   m1,
	const MeshType2&   m2,
	uint               nSamples,
	bool               deterministic,
	SamplerType&       sampler,
	std::vector<uint>& birthVertices,
	LogType& log)
{
	std::string meshName1 = "first mesh";
	std::string meshName2 = "second mesh";
	if constexpr (HasName<MeshType1>) {
		meshName1 = m1.name();
	}
	if constexpr (HasName<MeshType2>) {
		meshName2 = m2.name();
	}

	if constexpr (vcl::isLoggerValid<LogType>()) {
		log.log(0, "Sampling " + meshName2 + "...");
	}

	sampler = vcl::vertexUniformPointSampling<SamplerType>(
		m2, nSamples, birthVertices, false, deterministic);

	if constexpr (vcl::isLoggerValid<LogType>()) {
		log.log(5, meshName2 + " sampled.");
		log.startNewTask(5, 100, "Computing distance between samples and " + meshName1 + "...");
	}

	auto res = samplerMeshHausdorff(m1, sampler, log);

	if constexpr (vcl::isLoggerValid<LogType>()) {
		log.endTask("Distance between samples and " + meshName1 + " computed.");
	}

	return res;
}

} // namespace vcl::internal

template<MeshConcept MeshType1, MeshConcept MeshType2, LoggerConcept LogType>
HausdorffDistResult hausdorffDistance(
	const MeshType1& m1,
	const MeshType2& m2,
	LogType& log,
	uint nSamples,
	HausdorffSamplingMethod sampMethod,
	bool deterministic)
{
	if (nSamples == 0)
		nSamples = m2.vertexNumber();

	std::vector<uint> birth;

	switch (sampMethod) {
	case VERTEX_UNIFORM: {
		//ConstVertexSampler<typename MeshType2::VertexType> sampler;
		PointSampler sampler;

		return internal::hausdorffDistanceVertexUniformSampling(
			m1, m2, nSamples, deterministic, sampler, birth, log);
	}

	case EDGE_UNIFORM: {
		// todo
		return HausdorffDistResult();
	}
	case MONTECARLO: {
		// todo
		return HausdorffDistResult();
	default:
		assert(0);
		return HausdorffDistResult();
	}
	}
}

} // namespace vcl
