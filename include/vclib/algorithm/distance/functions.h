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

#ifndef VCL_ALGORITHM_DISTANCE_FUNCTIONS_H
#define VCL_ALGORITHM_DISTANCE_FUNCTIONS_H

#include <vclib/mesh/requirements.h>
#include <vclib/space/point.h>

namespace vcl {

template<typename Obj1, typename Obj2, typename RetScalarType>
struct DistFunctionStruct
{
};

template<PointConcept Obj1, PointConcept Obj2, typename RetScalarType>
struct DistFunctionStruct<Obj1, Obj2, RetScalarType>
{
	static inline const std::function<RetScalarType(const Obj1&, const Obj2&)> distFun =
		[](const Obj1& o1, const Obj2& o2)
	{
		return o1.dist(o2);
	};
};

template<VertexConcept Obj1, PointConcept Obj2, typename RetScalarType>
struct DistFunctionStruct<Obj1, Obj2, RetScalarType>
{
	static inline const std::function<RetScalarType(const Obj1&, const Obj2&)> distFun =
		[](const Obj1& o1, const Obj2& o2)
	{
		return o1.coord().dist(o2);
	};
};

template<VertexConcept Obj1, PointConcept Obj2, typename RetScalarType>
struct DistFunctionStruct<Obj1*, Obj2, RetScalarType>
{
	static inline const std::function<RetScalarType(const Obj1* const&, const Obj2&)> distFun =
		[](const Obj1* const& o1, const Obj2& o2)
	{
		return o1->coord().dist(o2);
	};
};

template<PointConcept Obj1, VertexConcept Obj2, typename RetScalarType>
struct DistFunctionStruct<Obj1, Obj2, RetScalarType>
{
	static inline const std::function<RetScalarType(const Obj1&, const Obj2&)> distFun =
		[](const Obj1& o1, const Obj2& o2)
	{
		return o1.dist(o2.coord());
	};
};

template<PointConcept Obj1, VertexConcept Obj2, typename RetScalarType>
struct DistFunctionStruct<Obj1, Obj2*, RetScalarType>
{
	static const inline std::function<RetScalarType(const Obj1&, const Obj2* const&)> distFun =
		[](const Obj1& o1, const Obj2* const& o2)
	{
		return o1.dist(o2->coord());
	};
};

template<typename Obj1, typename Obj2, typename RetScalarType = double>
auto distFunction()
{
	return DistFunctionStruct<Obj1, Obj2, RetScalarType>::distFun;
}

} // namespace vcl

#endif // VCL_ALGORITHM_DISTANCE_FUNCTIONS_H
