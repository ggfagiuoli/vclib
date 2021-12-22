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

#include "save_obj.h"

#include <map>
#include <vclib/exception/io_exception.h>
#include <vclib/misc/file_info.h>

#include "internal/io_utils.h"
#include "internal/io_write.h"
#include "obj/material.h"

namespace vcl::io {

namespace internal {

template<typename VertexType, typename MeshType>
obj::Material materialFromVertex(const VertexType& v, const FileMeshInfo& fi)
{
	obj::Material mat;
	if (fi.hasVertexColors()) {
		mat.hasColor = true;
		mat.Kd.x()   = v.color().redF();
		mat.Kd.y()   = v.color().greenF();
		mat.Kd.z()   = v.color().blueF();
	}
	return mat;
}

template<typename VertexType, typename MeshType>
void writeVertexMaterial(
	const VertexType&                     v,
	const FileMeshInfo&                   fi,
	obj::Material&                        lastMaterial,
	std::map<obj::Material, std::string>& materialMap,
	std::ofstream&                        fp,
	std::ofstream&                        mtlfp)
{
	static const std::string MATERIAL_PREFIX = "MATERIAL_";
	// get the material from the vertex
	obj::Material mat = materialFromVertex(v, fi);
	if (!mat.isEmpty()) {
		std::string mname; // name of the material of the vertex
		auto        it = materialMap.find(mname);
		if (it == materialMap.end()) { // if it is a new material
			// add the new material to the map
			mname            = MATERIAL_PREFIX + std::to_string(materialMap.size());
			materialMap[mat] = mname;
			// save the material in the mtl file
			mtlfp << "newmtl " << mname << std::endl;
			mtlfp << mat << std::endl;
		}
		else { // get the name of the material
			mname = it->second;
		}
		// if the material of the vertex is different from the last used, need to add usemtl
		if (mat != lastMaterial) {
			lastMaterial = mat;
			fp << "usemtl " << mname << std::endl;
		}
	}
}

} // namespace internal

template<typename MeshType>
void saveObj(const MeshType& m, const std::string& filename)
{
	vcl::io::FileMeshInfo info(m);
	saveObj(m, filename, info);
}

template<typename MeshType>
void saveObj(const MeshType& m, const std::string& filename, const FileMeshInfo& info)
{
	FileMeshInfo meshInfo(m);

	// make sure that the given info contains only components that are actually available in the
	// mesh. meshInfo will contain the intersection between the components that the user wants to
	// save and the components that are available in the mesh.
	meshInfo = info.intersect(meshInfo);

	// if the mesh has both vertex and wedge texcords, will be save just wedges because obj does
	// not allow to save them both. In any case, also vertex texcoords will result saved as
	// wedge texcoords in the final file.
	if (meshInfo.hasVertexTexCoords() && meshInfo.hasFaceWedgeTexCoords()) {
		meshInfo.setVertexTexCoords(false);
	}

	std::ofstream fp = internal::saveFileStream(filename, "obj");

	std::ofstream                        mtlfp;
	std::map<obj::Material, std::string> materialMap;
	bool useMtl = meshInfo.hasVertexColors() || meshInfo.hasFaceColors() ||
				  (meshInfo.hasTextures() &&
				   (meshInfo.hasVertexTexCoords() || meshInfo.hasFaceWedgeTexCoords()));
	if (useMtl) {
		mtlfp                   = internal::saveFileStream(filename, "mtl");
		std::string mtlFileName = vcl::fileInfo::filenameWithoutExtension(filename) + ".mtl";
		fp << "mtllib ./" << mtlFileName << std::endl;
	}

	obj::Material lastMaterial;

	// vertices
	using VertexType = typename MeshType::VertexType;
	for (const VertexType& v : m.vertices()) {
		if (useMtl) { // mtl management
			internal::writeVertexMaterial(v, meshInfo, lastMaterial, materialMap, fp, mtlfp);
		}
		fp << "v ";
		internal::writeDouble(fp, v.coord().x(), false);
		internal::writeDouble(fp, v.coord().y(), false);
		internal::writeDouble(fp, v.coord().z(), false);
		fp << std::endl;

		if constexpr (hasPerVertexNormal<MeshType>()) {
			if (meshInfo.hasVertexNormals()) {
				fp << "vn ";
				internal::writeDouble(fp, v.normal().x(), false);
				internal::writeDouble(fp, v.normal().y(), false);
				internal::writeDouble(fp, v.normal().z(), false);
				fp << std::endl;
			}
		}
		if constexpr (hasPerVertexTexCoord<MeshType>()) {
			if (meshInfo.hasVertexTexCoords()) {
				fp << "vt ";
				internal::writeFloat(fp, v.texCoord().u(), false);
				internal::writeFloat(fp, v.texCoord().v(), false);
				fp << std::endl;
			}
		}
	}

	// faces
	if constexpr (vcl::hasFaces<MeshType>()) {

	}

	fp.close();
	if (useMtl) {
		mtlfp.close();
	}
}

} // namespace vcl::io
