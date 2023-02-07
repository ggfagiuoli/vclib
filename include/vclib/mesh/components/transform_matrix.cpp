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

#include "transform_matrix.h"

namespace vcl::comp {

template<typename Scalar, typename El, bool h, bool o>
void TransformMatrix<Scalar, El, h, o>::init()
{
	tr().setIdentity();
}

template<typename Scalar, typename El, bool h, bool o>
const typename TransformMatrix<Scalar, El, h, o>::TransformMatrixType&
TransformMatrix<Scalar, El, h, o>::transformMatrix() const
{
	return tr();
}

template<typename Scalar, typename El, bool h, bool o>
typename TransformMatrix<Scalar, El, h, o>::TransformMatrixType&
TransformMatrix<Scalar, El, h, o>::transformMatrix()
{
	return tr();
}

template<typename Scalar, typename El, bool h, bool o>
template<typename Element>
void TransformMatrix<Scalar, El, h, o>::importFrom(const Element& e)
{
	if constexpr(HasTransformMatrix<Element>) {
		tr() = e.transformMatrix().template cast<Scalar>();
	}
}

template<typename Scalar, typename El, bool h, bool o>
Matrix44<Scalar>& TransformMatrix<Scalar, El, h, o>::tr()
{
	return data.template get<El>(this);
}

template<typename Scalar, typename El, bool h, bool o>
const Matrix44<Scalar>& TransformMatrix<Scalar, El, h, o>::tr() const
{
	return data.template get<El>(this);
}

} // namespace vcl::comp
