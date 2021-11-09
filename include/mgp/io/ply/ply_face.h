/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_IO_PLY_FACE_H
#define MGP_IO_PLY_FACE_H

#include "ply_header.h"
#include <fstream>
#include <mgp/misc/tokenizer.h>

namespace mgp {
namespace ply {

namespace internal {

template<typename MeshType, typename FaceType>
void saveFaceIndices(
	std::ofstream&  file,
	Property        p,
	const MeshType& m,
	const FaceType& f,
	bool bin);

template <typename MeshType>
void loadFacesTxt(
	std::ifstream&              file,
	const PlyHeader&            header,
	MeshType& m);

template <typename MeshType>
void loadFacesBin(
	std::ifstream&              file,
	const PlyHeader&            header,
	MeshType& m);

} // namespace internal

template<typename MeshType>
void saveFaces(std::ofstream& file, const PlyHeader& header, const MeshType mesh);

template<typename MeshType>
void loadFaces(std::ifstream& file, const PlyHeader& header, MeshType& mesh);

} // namespace ply
} // namespace mgp

#include "ply_face.cpp"

#endif // MGP_IO_PLY_FACE_H
