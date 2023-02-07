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

#include "scalar.h"

namespace vcl::comp {

template<typename T, typename El, bool h, bool o>
const T& Scalar<T, El, h, o>::scalar() const
{
	return s();
}

template<typename T, typename El, bool h, bool o>
T& Scalar<T, El, h, o>::scalar()
{
	return s();
}

template<typename T, typename El, bool h, bool o>
template<typename Element>
void Scalar<T, El, h, o>::importFrom(const Element& e)
{
	if constexpr (HasScalar<Element>) {
		if (isScalarEnabledOn(e)){
			s() = e.scalar();
		}
	}
}

template<typename T, typename El, bool h, bool o>
T& Scalar<T, El, h, o>::s()
{
	return data.template get<El>(this);
}

template<typename T, typename El, bool h, bool o>
const T& Scalar<T, El, h, o>::s() const
{
	return data.template get<El>(this);
}

} // namespace vcl::comp
