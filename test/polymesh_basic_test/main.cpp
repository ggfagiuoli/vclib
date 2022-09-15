/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
 * Alessandro Muntoni                                                        *
 * VCLab - ISTI - Italian National Research Council                          *
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

#include <iostream>

#include <vclib/poly_mesh.h>
#include <vclib/io/load_ply.h>
#include <vclib/io/save_ply.h>
#include <vclib/algorithms/update/flag.h>
#include <vclib/algorithms/smooth.h>

int main()
{
	vcl::PolyMesh m;

	m.addVertex();

	assert(m.vertexNumber() == 1);

	m.addFace();

	assert(m.faceNumber() == 1);

	auto& f = m.face(0);

	assert(f.vertexNumber() == 0);

	f.pushVertex(&m.vertex(0));

	assert(f.vertexNumber() == 1);
	assert(m.face(0).vertexNumber() == 1);

	assert(&m.vertex(0) == f.vertex(0));

	m.enablePerFaceAdjacentFaces();

	assert(vcl::isPerFaceAdjacentFacesEnabled(m));

	f.clearVertices();

	assert(m.face(0).vertexNumber() == 0);

	return 0;
}
