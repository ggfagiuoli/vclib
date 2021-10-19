/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_FACE_VERTEX_REF_ARRAY_H
#define MGP_MESH_FACE_VERTEX_REF_ARRAY_H

#include <array>
#include <assert.h>

#include <mgp/misc/type_traits.h>

namespace mgp {
namespace face {

/**
 * @brief The VertexRefsArray class is a Component of the Mesh::Face class which will contain
 * a fixed size array of references to Mesh::Vertex.
 *
 * This class is intended to be used in Meshes with Faces at most N vertices, with N known at
 * compile time.
 */
template<class Vertex, int N>
class VertexRefsArray
{
public:
	constexpr static unsigned int N_VERTICES = N;

	VertexRefsArray();

	Vertex*&      v(unsigned int i);
	const Vertex* v(unsigned int i) const;

	void setVertex(Vertex* v, unsigned int i);
	void setVertices(const std::initializer_list<Vertex*>& list);

protected:
	std::array<Vertex*, N> refs;

	void updateVertexReferences(const Vertex* oldBase, const Vertex* newBase);
};

template<class Vertex>
class TriangleVertexRefsArray : public VertexRefsArray<Vertex, 3>
{
	Vertex*&      v0() { return VertexRefsArray<Vertex, 3>::refs[0]; }
	Vertex*&      v1() { return VertexRefsArray<Vertex, 3>::refs[1]; }
	Vertex*&      v2() { return VertexRefsArray<Vertex, 3>::refs[2]; }
	const Vertex* v0() const { return VertexRefsArray<Vertex, 3>::refs[0]; }
	const Vertex* v1() const { return VertexRefsArray<Vertex, 3>::refs[1]; }
	const Vertex* v2() const { return VertexRefsArray<Vertex, 3>::refs[2]; }

	void setV0(Vertex* v) { VertexRefsArray<Vertex, 3>::refs[0] = v; }
	void setV1(Vertex* v) { VertexRefsArray<Vertex, 3>::refs[1] = v; }
	void setV2(Vertex* v) { VertexRefsArray<Vertex, 3>::refs[2] = v; }
};

/**
 * Detector to check if a class has the method v(int)
 */

template<typename T>
using hasVertexRefsArray_t = decltype(std::declval<const T&>().v(std::declval<int>()));

template<typename T>
using hasVertexRefsArray = typename detector<hasVertexRefsArray_t, void, T>::type;

} // namespace face
} // namespace mgp

#include "vertex_ref_array.cpp"

#endif // MGP_MESH_FACE_VERTEX_REF_ARRAY_H
