/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
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

#ifndef VCL_MESH_COMPONENTS_TEX_COORD_H
#define VCL_MESH_COMPONENTS_TEX_COORD_H

#include <vclib/space/tex_coord.h>

#include "concepts/tex_coord.h"
#include "internal/component_data.h"

namespace vcl::comp {

template<
	typename Scalar,
	typename ElementType = void,
	bool horizontal      = true,
	bool optional        = false>
class TexCoord
{
	using ThisType = TexCoord<Scalar, ElementType, horizontal, optional>;
public:
	// data that the component stores internally (or vertically)
	using DataValueType = vcl::TexCoord<Scalar>;
	// expose the type to allow access to this component
	using TexCoordComponent = ThisType;

	static const bool IS_VERTICAL = !horizontal;
	static const bool IS_OPTIONAL = optional;

	using TexCoordType = vcl::TexCoord<Scalar>;

	const TexCoordType& texCoord() const;
	TexCoordType&       texCoord();

	constexpr bool isTexCoordEnabled() const { return true; }

protected:
	template<typename Element>
	void importFrom(const Element& e);

private:
	// members that allow to access the texcoord, trough data (hor) or trough parent (vert)
	vcl::TexCoord<Scalar>& t();
	const vcl::TexCoord<Scalar>& t() const;

	// contians the actual data of the component, if the component is horizontal
	internal::ComponentData<DataValueType, horizontal> data;
};

template<typename ElementType = void, bool horizontal = true, bool optional = false>
using TexCoordf = TexCoord<float, ElementType, horizontal, optional>;

template<typename ElementType = void, bool horizontal = true, bool optional = false>
using TexCoordd = TexCoord<double, ElementType, horizontal, optional>;

} // namespace vcl::comp

#include "tex_coord.cpp"

#endif // VCL_MESH_COMPONENTS_TEXCOORD_H
