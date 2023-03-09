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

#ifndef VCL_ALGORITHM_UPDATE_TRANSFORM_H
#define VCL_ALGORITHM_UPDATE_TRANSFORM_H

#include <vclib/math/transform.h>
#include <vclib/mesh/requirements.h>

namespace vcl {

template<MeshConcept MeshType, typename ScalarM>
void applyTransformMatrix(
	MeshType&                mesh,
	const Matrix44<ScalarM>& matrix,
	bool                     updateNormals = true);

template<MeshConcept MeshType, PointConcept PointType>
void translate(MeshType& mesh, const PointType& t);

template<MeshConcept MeshType, PointConcept PointType>
void scale(MeshType& mesh, const PointType& s);

template<MeshConcept MeshType, typename Scalar = double>
void scale(MeshType& mesh, const Scalar& s);

} // namespace vcl

#include "transform.cpp"

#endif // VCL_ALGORITHM_UPDATE_TRANSFORM_H
