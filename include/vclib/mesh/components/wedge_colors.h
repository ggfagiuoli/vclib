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

#ifndef VCL_MESH_COMPONENTS_WEDGE_COLORS_H
#define VCL_MESH_COMPONENTS_WEDGE_COLORS_H

#include "concepts/wedge_colors.h"
#include "internal/component_data.h"

#include <vclib/misc/random_access_container.h>

namespace vcl::comp {

template<int N, typename ElementType, bool horizontal>
class WedgeColors
{
	using ThisType = WedgeColors<N, ElementType, horizontal>;

	using Base = RandomAccessContainer<vcl::Color, N>;

public:
	using DataValueType = RandomAccessContainer<vcl::Color, N>; // data that the component stores internally (or vertically)
	using WedgeTexCoordsComponent = ThisType; // expose the type to allow access to this component

	static const bool IS_VERTICAL = !horizontal;

	static const int WEDGE_COLOR_NUMBER = Base::CONTAINER_SIZE;

	/* Iterator Types declaration */

	using WedgeColorsIterator           = typename Base::Iterator;
	using ConstWedgeColorsIterator      = typename Base::ConstIterator;
	using WedgeColorsRangeIterator      = typename Base::RACRangeIterator;
	using ConstWedgeColorsRangeIterator = typename Base::RACConstRangeIterator;

	WedgeColors();

	/* Member functions */

	vcl::Color&       wedgeColor(uint i);
	const vcl::Color& wedgeColor(uint i) const;

	vcl::Color&       wedgeColorMod(int i);
	const vcl::Color& wedgeColorMod(int i) const;

	void setWedgeColor(const vcl::Color& t, uint i);
	void setWedgeColors(const std::vector<vcl::Color>& list);

	constexpr bool isWedgeColorsEnabled() const { return true; }

	/* Iterator Member functions */

	WedgeColorsIterator           wedgeColorBegin();
	WedgeColorsIterator           wedgeColorEnd();
	ConstWedgeColorsIterator      wedgeColorBegin() const;
	ConstWedgeColorsIterator      wedgeColorEnd() const;
	WedgeColorsRangeIterator      wedgeColors();
	ConstWedgeColorsRangeIterator wedgeColors() const;

	// dummy member to discriminate between this component and the one simulated by the
	// FaceHalfEdgeReference component
	void __compWedgeColors() const {};

protected:
	/* Member functions specific for vector of wedge colors */

	void resizeWedgeColors(uint n) requires (N < 0);
	void pushWedgeColor(const vcl::Color& c) requires (N < 0);
	void insertWedgeColor(uint i, const vcl::Color& c) requires (N < 0);
	void eraseWedgeColor(uint i) requires (N < 0);
	void clearWedgeColor() requires (N < 0);

	template <typename Element>
	void importFrom(const Element& e);

private:
	template<typename Element>
	void importWedgeColorsFrom(const Element& e);

	RandomAccessContainer<vcl::Color, N>& colors();
	const RandomAccessContainer<vcl::Color, N>& colors() const;

	internal::ComponentData<DataValueType, horizontal> data;
};

} // namespace vcl::comp

#include "wedge_colors.cpp"

#endif // VCL_MESH_COMPONENTS_WEDGE_COLORS_H
