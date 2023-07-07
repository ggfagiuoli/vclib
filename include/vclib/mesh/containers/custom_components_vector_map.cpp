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

#include "custom_components_vector_map.h"

#include <vclib/exceptions/mesh_exceptions.h>
#include <vclib/misc/compactness.h>

namespace vcl::mesh {

/**
 * @brief Removes all the custom component vectors stored in the map.
 */
inline void CustomComponentsVectorMap<true>::clear()
{
	map.clear();
	needToInitialize.clear();
	compType.clear();
}

/**
 * @brief For each custom component vector, it reserves the given size.
 * @param[in] size: the size to reserve for each custom component vector.
 */
inline void CustomComponentsVectorMap<true>::reserve(uint size)
{
	for (auto& p : map) {
		p.second.reserve(size);
	}
}

/**
 * @brief For each custom component vector, it resizes the vector to the given
 * size.
 * @param[in] size: the size to reserve for each custom component vector.
 */
inline void CustomComponentsVectorMap<true>::resize(uint size)
{
	for (auto& p : map) {
		// At this call, we don't know statically the types of each custom
		// component, therefore we cannot initialize them (std::any of each
		// vector will be in an invalid state). Therefore, we mark all the
		// resized custom components as 'needToInitialize'. Initiaization will
		// be made just on the uninitialized values of the vectors at the first
		// access of each custom component.
		if (p.second.size() < size)
			needToInitialize.at(p.first) = true;
		p.second.resize(size);
	}
}

/**
 * @brief Compacts each custom component vector according to the given indices.
 *
 * All the indices  of the newIndices vector with value UINT_NULL are considered
 * as deleted, and the corresponding elements in the custom component vectors
 * are removed. The remaining indices are compacted, and the custom component
 * vectors are resized to the new size.
 *
 * @param[in] newIndices: a vector that tells, for each element index, the
 * new index of the element, and UINT_NULL if the element has been deleted.
 */
inline void CustomComponentsVectorMap<true>::compact(
	const std::vector<uint>& newIndices)
{
	for (auto& p : map) {
		vcl::compactVector(p.second, newIndices);
	}
}

/**
 * @brief Adds a new vector of custom components having the given size, the
 * given name and with the template argumet CompType.
 *
 * Data is initialized with the empty constructor of CompType.
 *
 * @note If a custom component with the same name already exists, it is
 * replaced, no matter of the old type.
 *
 * @tparam CompType: the type of the newly added custom component.
 * @param[in] name: the unique name of the custom component.
 * @param[in] size: the initial size of the custom component vector.
 */
template<typename CompType>
void CustomComponentsVectorMap<true>::addNewComponent(
	const std::string& name,
	uint       size)
{
	std::vector<std::any>& v = map[name];
	v.resize(size, CompType());
	needToInitialize[name] = false;
	compType.emplace(name, typeid(CompType));
}

/**
 * @brief Deletes the custom component vector with the given name.
 * It does nothing if the element does not exist.
 * @param[in] name: the name of the custom component vector to delete.
 */
inline void CustomComponentsVectorMap<true>::deleteComponent(
	const std::string& name)
{
	map.erase(name);
	needToInitialize.erase(name);
	compType.erase(name);
}

/**
 * @brief Asserts that the compName component exists.
 *
 * @param[in] compName: the name of the custom component to check.
 */
inline void CustomComponentsVectorMap<true>::assertComponentExists(
	const std::string& compName) const
{
	(void) (compName);
	assert(map.find(compName) != map.end());
}

/**
 * @brief Returns true if the compName exists.
 * @param[in] compName: the name of the custom component to check.
 * @return true if the compName exists.
 */
inline bool CustomComponentsVectorMap<true>::componentExists(
	const std::string& compName) const
{
	return (map.find(compName) != map.end());
}

/**
 * @brief Returns a vector of std::string containing all the custom components,
 * regardless the types of the custom components.
 * @return a vector of std::string containing all the custom components.
 */
inline std::vector<std::string>
CustomComponentsVectorMap<true>::allComponentNames() const
{
	std::vector<std::string> names;
	names.reserve(map.size());
	for (const auto& p : map)
		names.push_back(p.first);
	return names;
}

/**
 * @brief Returns true if the type associated to the compName custom component
 * is the same of the given template argument CompType.
 *
 * @tparam CompType: the type to check.
 * @param[in] compName: the name of the custom component to check.
 * @return true if the type associated to the compName custom component
 * is the same of the given template argument CompType.
 */
template<typename CompType>
bool CustomComponentsVectorMap<true>::isComponentOfType(
	const std::string& compName) const
{
	std::type_index t(typeid(CompType));

	return t == compType.at(compName);
}

/**
 * @brief Returns the std::type_index of the type of the CustomComponent having
 * the input name.
 *
 * @throws std::out_of_range if the compName does not exist.
 * @param[in] compName: the name of the custom component.
 * @return the std::type_index of the type of the CustomComponent having
 * the input name.
 */
inline std::type_index
vcl::mesh::CustomComponentsVectorMap<true>::componentType(
	const std::string& compName) const
{
	return compType.at(compName);
}

/**
 * @brief Returns a vector of strings of all the custom components having as
 * type the given template argument CompType.
 *
 * @tparam CompType: the type of the custom components to return.
 * @return a vector of strings of all the custom components having as
 * type the given template argument CompType.
 */
template<typename CompType>
std::vector<std::string>
CustomComponentsVectorMap<true>::allComponentNamesOfType() const
{
	std::vector<std::string> names;
	std::type_index t(typeid(CompType));
	for (const auto& p : compType) {
		if (p.second == t)
			names.push_back(p.first);
	}
	return names;
}

/**
 * @brief Returns a const reference of std::vector<std::any> of the custom
 * component with the given name and the given template argument CompType.
 *
 * If the CompType does not mach with the type associated with compName, thows a
 * vcl::BadCustomComponentTypeException.
 *
 * @tparam CompType: the type of the custom component to return.
 * @param[in] compName: the name of the custom component to return.
 * @return a const reference of std::vector<std::any> of the custom component
 * with the given name and the given template argument CompType.
 */
template<typename CompType>
const std::vector<std::any>& CustomComponentsVectorMap<true>::componentVector(
	const std::string& compName) const
{
	checkComponentType<CompType>(compName);
	std::vector<std::any>& v =
		const_cast<std::vector<std::any>&>(map.at(compName));
	// if the component needs to be initialized (e.g. we made a resize)
	if (needToInitialize.at(compName)) {
		for (std::any& a : v) {
			if (!a.has_value())
				a = CompType();
		}
		needToInitialize.at(compName) = false;
	}
	return v;
}

/**
 * @brief Returns a reference of std::vector<std::any> of the custom component
 * with the given name and the given template argument CompType.
 *
 * If the CompType does not mach with the type associated with compName, thows a
 * vcl::BadCustomComponentTypeException.
 *
 * This function will initialize all the uninitialized values of the accessed
 * custom component.
 *
 * @tparam CompType: the type of the custom component to return.
 * @param[in] compName: the name of the custom component to return.
 * @return a reference of std::vector<std::any> of the custom component
 * with the given name and the given template argument CompType.
 */
template<typename CompType>
std::vector<std::any>&
CustomComponentsVectorMap<true>::componentVector(const std::string& compName)
{
	checkComponentType<CompType>(compName);
	std::vector<std::any>& v = map.at(compName);
	// if the component needs to be initialized (e.g. we made a resize)
	if (needToInitialize.at(compName)) {
		for (std::any& a : v) {
			if (!a.has_value())
				a = CompType();
		}
		needToInitialize.at(compName) = false;
	}
	return v;
}

template<typename CompType>
void CustomComponentsVectorMap<true>::checkComponentType(
	const std::string& compName) const
{
	std::type_index t(typeid(CompType));
	if (t != compType.at(compName)) {
		throw BadCustomComponentTypeException(
			"Expected type " + std::string(compType.at(compName).name()) +
			" for " + compName + ", but was " + std::string(t.name()) + ".");
	}
}

} // namespace vcl::mesh
