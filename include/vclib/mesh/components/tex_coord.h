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

#ifndef VCL_MESH_COMPONENTS_TEX_COORD_H
#define VCL_MESH_COMPONENTS_TEX_COORD_H

#include <vclib/concepts/mesh/components/tex_coord.h>
#include <vclib/space/tex_coord.h>

#include "bases/component.h"

namespace vcl::comp {

/**
 * @brief The TexCoord class
 *
 * @ingroup components
 */
template<typename Scalar, typename ElementType = void, bool OPTIONAL = false>
class TexCoord : public Component<vcl::TexCoord<Scalar>, ElementType, OPTIONAL>
{
	using Base = Component<vcl::TexCoord<Scalar>, ElementType, OPTIONAL>;
	using ThisType = TexCoord<Scalar, ElementType, OPTIONAL>;

public:
	// expose the type to allow access to this component
	using TexCoordComponent = ThisType;

	using TexCoordType = vcl::TexCoord<Scalar>;

	bool isEnabled() const;
	bool isTexCoordEnabled() const;

	const TexCoordType& texCoord() const;
	TexCoordType&       texCoord();

protected:
	// PointersComponent interface functions
	template<typename Element>
	void importFrom(const Element& e);
};

/* Detector function to check if a class has TexCoord enabled */

template <typename T>
bool isTexCoordEnabledOn(const T& element);

template<typename ElementType = void, bool horizontal = true, bool OPTIONAL = false>
using TexCoordf = TexCoord<float, ElementType, OPTIONAL>;

template<typename ElementType = void, bool horizontal = true, bool OPTIONAL = false>
using TexCoordd = TexCoord<double, ElementType, OPTIONAL>;

} // namespace vcl::comp

#include "tex_coord.cpp"

#endif // VCL_MESH_COMPONENTS_TEXCOORD_H
