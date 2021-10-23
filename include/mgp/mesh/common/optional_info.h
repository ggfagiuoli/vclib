/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_COMMON_OPTIONAL_INFO_H
#define MGP_MESH_COMMON_OPTIONAL_INFO_H

#include <mgp/misc/type_traits.h>

#include "info.h"


namespace mgp::mesh {

template<typename T>
class ComponentsVector;

}

namespace mgp::common {

template<typename T>
class OptionalInfo : virtual public Info
{
public:
	OptionalInfo() : Info(), containerPointer(nullptr) {}

	void __optional_info__() const {} // dummy member used just for detection of an OpionalInfo object

protected:
	void setContainerPointer(mesh::ComponentsVector<T>* cp) { containerPointer = cp; }
	mesh::ComponentsVector<T>* containerPointer;
};

/**
 * Detector to check if a class has (inherits) OptionalInfo
 */

template<typename T>
using hasOptionalInfo_t = decltype(std::declval<const T&>().__optional_info__());

template<typename T>
using hasOptionalInfo = typename detector<hasOptionalInfo_t, void, T>::type;

} // namespace mgp::common

#endif // MGP_MESH_COMMON_OPTIONAL_INFO_H
