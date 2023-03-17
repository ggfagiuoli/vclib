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

#include "save.h"

namespace vcl {

template<MeshConcept MeshType>
void save(const MeshType& m, const std::string& filename, bool binary)
{
	FileMeshInfo info(m);
	save(m, filename, info, binary);
}

template<MeshConcept MeshType>
void save(const MeshType& m, const std::string& filename, const FileMeshInfo& info, bool binary)
{
	std::string name, ext;
	vcl::fileInfo::separateExtensionFromFilename(filename, name, ext);
	ext = vcl::str::toLower(ext);
	if (ext == ".obj") {
		io::saveObj(m, filename, info);
	}
	else if (ext == ".off") {
		io::saveOff(m, filename, info);
	}
	else if (ext == ".ply") {
		io::savePly(m, filename, info, binary);
	}
	else {
		throw vcl::UnknownFileFormatException(ext);
	}
}

} // namespace vcl
