/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_COMPONENTS_VERTEX_REFERENCES_H
#define MGP_MESH_COMPONENTS_VERTEX_REFERENCES_H

#include <array>
#include <assert.h>
#include <type_traits>
#include <vector>

#include "../iterators/range_iterator.h"

#include "element_references.h"

namespace mgp::comp {

class VertexReferencesTriggerer
{
};

template<class Vertex, int N>
class VertexReferences : protected ElementReferences<Vertex, N>, public VertexReferencesTriggerer
{
	using Base = ElementReferences<Vertex, N>;

public:
	static const int VERTEX_NUMBER = Base::CONTAINER_SIZE;

	/** Iterator Types declaration **/

	using VertexIterator           = typename Base::GCIterator;
	using ConstVertexIterator      = typename Base::ConstGCIterator;
	using VertexRangeIterator      = typename Base::GCRangeIterator;
	using ConstVertexRangeIterator = typename Base::ConstGCRangeIterator;

	/** Constructor **/

	VertexReferences() : ElementReferences<Vertex, N>() {}

	/** Member functions **/

	unsigned int vertexNumber() const { return Base::size(); }

	Vertex*&      v(unsigned int i) { return Base::at(i); }
	const Vertex* v(unsigned int i) const { return Base::at(i); }
	Vertex*&      vMod(int i) { return Base::atMod(i); }
	const Vertex* vMod(int i) const { return Base::atMod(i); }

	void setVertex(Vertex* v, unsigned int i) { Base::set(v, i); }
	void setVertices(const std::vector<Vertex*>& list) { Base::set(list); }

	using Base::contains;

	/** Member functions specific for vector **/

	template<int U = N>
	internal::ReturnIfIsVector<U, void> resizeVertices(unsigned int n)
	{
		Base::resize(n);
	}

	template<int U = N>
	internal::ReturnIfIsVector<U, void> pushVertex(Vertex* v)
	{
		Base::pushBack(v);
	}

	template<int U = N>
	internal::ReturnIfIsVector<U, void> insertVertex(unsigned int i, Vertex* v)
	{
		Base::insert(i, v);
	}

	template<int U = N>
	internal::ReturnIfIsVector<U, void> eraseVertex(unsigned int i)
	{
		Base::erase(i);
	}

	template<int U = N>
	internal::ReturnIfIsVector<U, void> clearVertices()
	{
		Base::clear();
	}

	/** Iterator Member functions **/

	VertexIterator           vertexBegin() { return Base::begin(); }
	VertexIterator           vertexEnd() { return Base::end(); }
	ConstVertexIterator      vertexBegin() const { return Base::begin(); }
	ConstVertexIterator      vertexEnd() const { return Base::end(); }
	VertexRangeIterator      vertexIterator() { return Base::rangeIterator(); }
	ConstVertexRangeIterator vertexIterator() const { return Base::rangeIterator(); }

protected:
	void updateVertexReferences(const Vertex* oldBase, const Vertex* newBase)
	{
		Base::updateElementReferences(oldBase, newBase);
	}

	void updateVertexReferencesAfterCompact(const Vertex* base, const std::vector<int>& newIndices)
	{
		Base::updateElementReferencesAfterCompact(base, newIndices);
	}
};

template<class Vertex>
class TriVertexReferences : public VertexReferences<Vertex, 3>
{
private:
	using B = VertexReferences<Vertex, 3>;

public:
	Vertex*&      v0() { return B::container[0]; }
	Vertex*&      v1() { return B::container[1]; }
	Vertex*&      v2() { return B::container[2]; }
	const Vertex* v0() const { return B::container[0]; }
	const Vertex* v1() const { return B::container[1]; }
	const Vertex* v2() const { return B::container[2]; }

	void setV0(Vertex* v) { B::container[0] = v; }
	void setV1(Vertex* v) { B::container[1] = v; }
	void setV2(Vertex* v) { B::container[2] = v; }
};

template<typename T>
using hasVertexReferencesT = std::is_base_of<VertexReferencesTriggerer, T>;

template<typename T>
bool constexpr hasVertexReferences()
{
	return hasVertexReferencesT<T>::value;
}

} // namespace mgp::comp

#endif // MGP_MESH_COMPONENTS_VERTEX_REFERENCES_H
