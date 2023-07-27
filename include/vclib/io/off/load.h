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

#ifndef VCL_IO_OFF_LOAD_H
#define VCL_IO_OFF_LOAD_H

#include <vclib/misc/logger.h>
#include <vclib/misc/mesh_info.h>

namespace vcl::io {

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
MeshType loadOff(
	const std::string& filename,
	LogType&           log                      = nullLogger,
	bool               enableOptionalComponents = true);

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
MeshType loadOff(
	const std::string& filename,
	MeshInfo&          loadedInfo,
	LogType&           log                      = nullLogger,
	bool               enableOptionalComponents = true);

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void loadOff(
	MeshType&          m,
	const std::string& filename,
	LogType&           log                      = nullLogger,
	bool               enableOptionalComponents = true);

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void loadOff(
	MeshType&          m,
	const std::string& filename,
	MeshInfo&          loadedInfo,
	LogType&           log                      = nullLogger,
	bool               enableOptionalComponents = true);

} // namespace vcl::io

#include "load.cpp"

#endif // VCL_IO_OFF_LOAD_H
