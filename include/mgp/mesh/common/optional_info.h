/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_COMMON_OPTIONAL_INFO_H
#define MGP_MESH_COMMON_OPTIONAL_INFO_H

#include <type_traits>

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

protected:
	void setContainerPointer(mesh::ComponentsVector<T>* cp) { containerPointer = cp; }
	mesh::ComponentsVector<T>* containerPointer;
};

/**
 * Detector to check if a class has (inherits) OptionalInfo
 */

template<typename T>
using hasOptionalInfoT = std::is_base_of<OptionalInfo<T>, T>;

template <typename  T>
bool constexpr hasOptionalInfo() {return hasOptionalInfoT<T>::value;}

} // namespace mgp::common

#endif // MGP_MESH_COMMON_OPTIONAL_INFO_H
