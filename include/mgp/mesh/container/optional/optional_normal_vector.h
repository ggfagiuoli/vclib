/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_CONTAINER_OPTIONAL_OPTIONAL_NORMAL_VECTOR_H
#define MGP_MESH_CONTAINER_OPTIONAL_OPTIONAL_NORMAL_VECTOR_H

#include <mgp/mesh/common/optional_normal.h>

#include "optional_generic_vector.h"

namespace mgp::mesh {

template<typename, typename = void>
class OptionalNormalVector
{
public:
	void resize(unsigned int) {}
};

template<typename T>
class OptionalNormalVector<T, std::enable_if_t<common::hasOptionalNormal<T>::value>> :
		private OptionalGenericVector<typename T::NormalType>
{
	typedef typename T::NormalType            NormalType;
	typedef OptionalGenericVector<NormalType> Base;

public:
	using Base::resize;
	bool              isNormalEnabled() { return Base::isEnabled(); };
	void              enableNormal(unsigned int size) { Base::enable(size); }
	void              disableNormal() { Base::disable(); }
	NormalType&       normal(unsigned int i) { return Base::at(i); }
	const NormalType& normal(unsigned int i) const { return Base::at(i); }
};

} // namespace mgp::mesh

#endif // MGP_MESH_CONTAINER_OPTIONAL_OPTIONAL_NORMAL_VECTOR_H
