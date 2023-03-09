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

#ifndef VCL_ALGORITHM_INTERSECTION_FUNCTIONS_H
#define VCL_ALGORITHM_INTERSECTION_FUNCTIONS_H

#include "element.h"
#include "misc.h"

#include <vclib/mesh/requirements.h>

namespace vcl {

// generic case - nothing is done here. It will fail because a specialization must exist
template<typename Obj1, typename Obj2>
struct IntersFunctionStruct
{
};

/**
 * @brief Return a proper intersect function between a Obj1 object and an Obj2 object.
 *
 * This function will return a function defined in a specialization of the struct
 * IntersFunctionStruct.
 *
 * The returned function is a std::function that takes in input two const references of the desired
 * types and returns a boolean that tells if the two objects intersect.
 *
 * If the intersection function for your types is not defined, you can write
 * your own IntersFunctionStruct specialization that defines a proper `static const inline` object
 * called `intersFun` of `std::function` type.
 */
template<typename Obj1, typename Obj2>
auto intersectFunction()
{
	return IntersFunctionStruct<Obj1, Obj2>::distFun;
}

} // namespace vcl

#endif // VCL_ALGORITHM_INTERSECTION_FUNCTIONS_H
