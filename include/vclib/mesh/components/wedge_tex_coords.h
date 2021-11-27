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

#ifndef VCL_MESH_COMPONENTS_WEDGE_TEX_COORDS_H
#define VCL_MESH_COMPONENTS_WEDGE_TEX_COORDS_H

#include "internal/generic_container.h"

#include <vclib/space/tex_coord.h>

namespace vcl::comp {

class WedgeTexCoordsTriggerer
{
};

template<typename Scalar, int N>
class WedgeTexCoords :
		protected internal::GenericContainer<vcl::TexCoord<Scalar>, N>,
		public WedgeTexCoordsTriggerer
{
	using Base = internal::GenericContainer<vcl::TexCoord<Scalar>, N>;

public:
	static const int WEDGE_TEX_COORD_NUMBER = Base::CONTAINER_SIZE;

	/* Iterator Types declaration */

	using WedgeTexCoordsIterator           = typename Base::GCIterator;
	using ConstWedgeTexCoordsIterator      = typename Base::ConstGCIterator;
	using WedgeTexCoordsRangeIterator      = typename Base::GCRangeIterator;
	using ConstWedgeTexCoordsRangeIterator = typename Base::ConstGCRangeIterator;

	WedgeTexCoords();

	/* Member functions */

	vcl::TexCoord<Scalar>&       wedgeTexCoord(uint i);
	const vcl::TexCoord<Scalar>& wedgeTexCoord(uint i) const;

	vcl::TexCoord<Scalar>&       wedgeTexCoordMod(int i);
	const vcl::TexCoord<Scalar>& wedgeTexCoordMod(int i) const;

	void setWedgeTexCoord(const vcl::TexCoord<Scalar>& t, uint i);
	void setWedgeTexCoords(const std::vector<vcl::TexCoord<Scalar>>& list);

	/* Iterator Member functions */

	WedgeTexCoordsIterator           wedgeTexCoordBegin();
	WedgeTexCoordsIterator           wedgeTexCoordEnd();
	ConstWedgeTexCoordsIterator      wedgeTexCoordBegin() const;
	ConstWedgeTexCoordsIterator      wedgeTexCoordEnd() const;
	WedgeTexCoordsRangeIterator      wedgeTexCoordIterator();
	ConstWedgeTexCoordsRangeIterator wedgeTexCoordIterator() const;

protected:
	using WedgeTexCoordScalarType = Scalar;

	/* Member functions specific for vector */

	template<int M = N>
	VCL_ENABLE_IF(M < 0, void) resizeWedgeTexCoords(uint n);

	template<int M = N>
	VCL_ENABLE_IF(M < 0, void) pushWedgeTexCoord(const vcl::TexCoord<Scalar>& t);

	template<int M = N>
	VCL_ENABLE_IF(M < 0, void)
	insertWedgeTexCoord(uint i, const vcl::TexCoord<Scalar>& t);

	template<int M = N>
	VCL_ENABLE_IF(M < 0, void) eraseWedgeTexCoord(uint i);

	template<int M = N>
	VCL_ENABLE_IF(M < 0, void) clearWedgeTexCoord();
};

template<typename T>
using hasWedgeTexCoordsT = std::is_base_of<WedgeTexCoordsTriggerer, T>;

template<typename T>
bool constexpr hasWedgeTexCoords()
{
	return hasWedgeTexCoordsT<T>::value;
}

template <typename T>
bool constexpr sanityCheckWedgeTexCoords()
{
	if constexpr (hasWedgeTexCoords<T>()) {
		return T::VERTEX_NUMBER == T::WEDGE_TEX_COORD_NUMBER;
	}
	else {
		return true;
	}
}

} // namespace vcl::comp

#include "wedge_tex_coords.cpp"

#endif // VCL_MESH_COMPONENTS_WEDGE_TEX_COORDS_H
