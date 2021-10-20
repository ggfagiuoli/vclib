/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_VERTEX_FACE_ADJ_REF_H
#define MGP_MESH_VERTEX_FACE_ADJ_REF_H

#include <vector>

#include "common.h"

namespace mgp {
namespace vert {

template<class Face>
class FaceAdjacencyRef : virtual public Info
{
public:
	void addFaceAdjacency(Face* f) {refs.push_back(f);};

private:
	std::vector<Face*> refs;
};

} // namespace vert
} // namespace mgp

#endif // MGP_MESH_VERTEX_FACE_ADJ_REF_H
