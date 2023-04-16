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

#include "selection.h"

#include <vclib/views/mesh.h>

namespace vcl {

template<MeshConcept MeshType>
uint vertexSelectionNumber(const MeshType &m)
{
#ifdef VCLIB_USES_RANGES
	return std::ranges::distance(m.vertices() | views::selected);
#else
	using VertexType = typename MeshType::VertexType;
	
	uint cnt = 0;
	for (const VertexType& v : m.vertices())
		if (v.isSelected())
			cnt++;
	
	return cnt;
#endif
}

template<FaceMeshConcept MeshType>
uint faceSelectionNumber(const MeshType &m)
{
#ifdef VCLIB_USES_RANGES
	return std::ranges::distance(m.faces() | views::selected);
#else
	using FaceType = typename MeshType::FaceType;
	
	uint cnt = 0;
	for (const FaceType& f : m.faces())
		if (f.isSelected())
			cnt++;
	
	return cnt;
#endif
}

} // namespace vcl
