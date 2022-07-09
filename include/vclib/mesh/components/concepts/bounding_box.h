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

#ifndef VCL_MESH_COMPONENTS_CONCEPTS_BOUNDING_BOX_H
#define VCL_MESH_COMPONENTS_CONCEPTS_BOUNDING_BOX_H

#include <vclib/misc/types.h>

namespace vcl::comp {

/**
 * @brief HasBoundingBox concept
 *
 * This concept is satisfied only if a class has a member function 'boundingBox()'.
 * No check is made on the return type.
 */
template<typename T>
concept HasBoundingBox = requires(T o)
{
	o.boundingBox();
};

/* Detector functions to check if a class has BoundingBox */

template<typename T>
bool constexpr hasBoundingBox()
{
	return HasBoundingBox<T>;
}

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_CONCEPTS_BOUNDING_BOX_H
