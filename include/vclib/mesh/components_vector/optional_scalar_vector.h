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

#ifndef VCL_MESH_COMPONENTS_VECTOR_OPTIONAL_SCALAR_VECTOR_H
#define VCL_MESH_COMPONENTS_VECTOR_OPTIONAL_SCALAR_VECTOR_H

#include <vclib/mesh/components_optional/optional_scalar.h>

#include "optional_generic_vector.h"

namespace vcl::internal {

template<typename, typename = void>
class OptionalScalarVector
{
public:
	void clear() {}
	void reserve(uint) {}
	void resize(uint) {}
	void compact(const std::vector<int>&) {}
};

template<typename T>
class OptionalScalarVector<T, std::enable_if_t<comp::hasOptionalScalar<T>()>> :
		private OptionalGenericVector<typename T::ScalarType>
{
	using ScalarType = typename T::ScalarType;
	using Base = OptionalGenericVector<ScalarType>;

public:
	using Base::clear;
	using Base::reserve;
	using Base::resize;
	using Base::compact;
	bool              isScalarEnabled() const { return Base::isEnabled(); };
	void              enableScalar(uint size) { Base::enable(size); }
	void              disableScalar() { Base::disable(); }
	ScalarType&       scalar(uint i) { return Base::at(i); }
	const ScalarType& scalar(uint i) const { return Base::at(i); }
};

} // namespace vcl::internal

#endif // VCL_MESH_COMPONENTS_VECTOR_OPTIONAL_SCALAR_VECTOR_H
