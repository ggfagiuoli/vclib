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

#ifndef VCL_MESH_VERTICAL_COMPONENTS_VECTOR_H
#define VCL_MESH_VERTICAL_COMPONENTS_VECTOR_H


#include <vclib/misc/types.h>

namespace vcl::internal {

/**
 * @brief Helper class that allows to call the same member function to all the bases of the
 * class, automatically.
 *
 * Based from the tutorial:
 * https://devblogs.microsoft.com/oldnewthing/20210114-00/?p=104714
 */
template<typename... bases>
class VerticalComponentsAggregator : public bases...
{
public:
	void clear() { (bases::clear(), ...); }

	void resize(uint size) { (bases::resize(size), ...); }

	void reserve(uint size) { (bases::reserve(size), ...); }

	void compact(const std::vector<int>& newIndices) { (bases::compact(newIndices), ...); }
};

template<typename T>
class VerticalComponentsVector :
		public VerticalComponentsAggregator<
			>
{
};

} // namespace vcl::internal

#endif // VCL_MESH_VERTICAL_COMPONENTS_VECTOR_H
