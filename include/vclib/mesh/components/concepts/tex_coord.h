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

#ifndef VCL_MESH_COMPONENTS_CONCEPTS_TEX_COORD_H
#define VCL_MESH_COMPONENTS_CONCEPTS_TEX_COORD_H

#include <vclib/misc/types.h>

namespace vcl::comp {

/**
 * @brief HasTexCoord concept is satisfied only if a Element class provides the types and member
 * functions specified in this concept. These types and member functions allow to access to a
 * TexCoord component of a given element.
 *
 * Note that this concept does not discriminate between the Horizontal TexCoord component
 * and the vertical OptionalTexCoord component, therefore it does not guarantee that a
 * template Element type that satisfies this concept provides TexCoord component at runtime
 * (it is guaranteed only that the proper member functions are available at compile time).
 *
 * To be completely sure that TexCoord is available at runtime, you need to call the member
 * function `isTexCoordEnabled()`.
 */
template<typename T>
concept HasTexCoord = requires(
	T o,
	const T& co)
{
	typename T::TexCoordType;
	{ o.texCoord() } -> std::same_as<typename T::TexCoordType&>;
	{ co.texCoord() } -> std::same_as<const typename T::TexCoordType&>;
	{ co.isTexCoordEnabled() } -> std::same_as<bool>;
};

/**
 * @brief HasOptionalTexCoord concept is satisfied only if a class satisfis the HasTexCoord concept
 * and has the additional member function '__optionalTexCoord()', which is the discriminator between
 * the non-optional and optional component.
 */
template<typename T>
concept HasOptionalTexCoord = HasTexCoord<T> && requires(T o)
{
	{ o.__optionalTexCoord() } -> std::same_as<void>;
};

/* Detector function to check if a class has TexCoord enabled */

template <typename T>
bool isTexCoordEnabledOn(const T& element)
{
	if constexpr (HasOptionalTexCoord<T>) {
		return element.isTexCoordEnabled();
	}
	else {
		return HasTexCoord<T>;
	}
}

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_CONCEPTS_TEX_COORD_H
