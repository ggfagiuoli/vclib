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

#ifndef VCL_MESH_MESH_H
#define VCL_MESH_MESH_H

#include <vclib/concepts/mesh.h>

#include "mesh_components.h"
#include "mesh_containers.h"

namespace vcl {

/**
 * @defgroup mesh Mesh
 *
 * @brief List of classes, aliases, concepts and functions used for the creation, customization,
 * usage and manipulation of Mesh classes.
 */

/**
 * @defgroup components Components
 * @ingroup mesh
 *
 * @brief List of all the Component classes, along with their concepts and functions.
 */

/**
 * @brief The Mesh class represents a generic 3D mesh. A mesh is composed of a generic number of
 * containers of Elements (which can be vertices, faces, edges...), plus some other components.
 *
 * The Mesh class will expose all the public members of its containers and is components, and its
 * role is to implement all the functionalities that allow these containers to comunicate
 * (e.g. an operation on the vertex container that requires to update also some face information).
 *
 * Therefore, in this page are documented only the functions that in some way need to modify the
 * status of more than one Container of the Mesh. Other functions are inherited by the Container
 * classes of the Elements of the Mesh, or from its Components, and depend on all the templates
 * that compose a specific Mesh definition.
 *
 * @ingroup mesh
 */
template<typename... Args> requires HasVertices<Args...>
class Mesh : public Args...
{
	template<typename El, bool b>
	friend struct comp::internal::CustomComponentsData;

	template<typename El, bool b>
	friend struct comp::internal::ComponentData;

	template<ElementConcept T>
	friend class mesh::ElementContainer;

	template <uint ELEM_TYPE, typename MeshType, typename... Comps>
	friend class Element;

public:
	/**
	 * @brief Containers is a vcl::TypeWrapper that wraps all the types from which the Mesh inherits
	 * (Args) that are ElementContainers (they satisfy the ElementContainerConcept).
	 */
	using Containers = typename vcl::
		FilterTypesByCondition<mesh::IsElementContainerPred, vcl::TypeWrapper<Args...>>::type;

	/**
	 * @brief Components is a vcl::TypeWrapper that wraps all the types from which the Mesh inherits
	 * (Args) that are Components (they satisfy the ComponentConcept).
	 */
	using Components = typename vcl::
		FilterTypesByCondition<comp::IsComponentPred, vcl::TypeWrapper<Args...>>::type;

	Mesh();
	Mesh(const Mesh& oth);
	Mesh(Mesh&& oth);

	void clear();

	void compact();

	template<ElementConcept El>
	static constexpr bool hasContainerOf();

	template<uint EL_TYPE>
	static constexpr bool hasContainerOf();

	template<uint EL_TYPE, uint COMP_TYPE>
	static constexpr bool hasPerElementOptionalComponent();

	template<typename OtherMeshType>
	void enableSameOptionalComponentsOf(const OtherMeshType& m);

	template<typename OtherMeshType>
	void importFrom(const OtherMeshType& m);

	void swap(Mesh& m2);

	Mesh& operator=(Mesh oth);

	/*** Generic Element functions ***/
	template<ElementConcept El>
	uint index(const El& e) const requires (hasContainerOf<El>());

	template<ElementConcept El>
	uint index(const El* e) const requires (hasContainerOf<El>());

	template<uint EL_TYPE>
	const auto& element(uint i) const requires (hasContainerOf<EL_TYPE>());

	template<uint EL_TYPE>
	auto& element(uint i) requires (hasContainerOf<EL_TYPE>());

	template<uint EL_TYPE>
	uint elementNumber() const requires (hasContainerOf<EL_TYPE>());

	template<uint EL_TYPE>
	uint elementContainerSize() const requires (hasContainerOf<EL_TYPE>());

	template<uint EL_TYPE>
	uint deletedElementNumber() const requires (hasContainerOf<EL_TYPE>());

	template<uint EL_TYPE>
	uint addElement() requires (hasContainerOf<EL_TYPE>());

	template<uint EL_TYPE>
	uint addElements(uint n) requires (hasContainerOf<EL_TYPE>());

	template<uint EL_TYPE>
	void reserveElements(uint n) requires (hasContainerOf<EL_TYPE>());

	template<uint EL_TYPE>
	void compactElements() requires (hasContainerOf<EL_TYPE>());
	
	template<uint EL_TYPE>
	void deleteElement(uint i) requires (hasContainerOf<EL_TYPE>());

	template<ElementConcept El>
	void deleteElement(const El* e) const requires (hasContainerOf<El>());

	template<ElementConcept El>
	void deleteElement(const El& e) const requires (hasContainerOf<El>());

	template<uint EL_TYPE>
	std::vector<uint> elementConpactIndices() const requires (hasContainerOf<EL_TYPE>());

	template<uint EL_TYPE>
	void updateElementIndices(const std::vector<uint>& newIndices) requires (hasContainerOf<EL_TYPE>());

	template<uint EL_TYPE, uint COMP_TYPE>
	bool isPerElementComponentEnabled() const
		requires (hasPerElementOptionalComponent<EL_TYPE, COMP_TYPE>());

	template<uint EL_TYPE, uint COMP_TYPE>
	void enablePerElementComponent() requires (hasPerElementOptionalComponent<EL_TYPE, COMP_TYPE>());

	template<uint EL_TYPE, uint COMP_TYPE>
	void disablePerElementComponent() requires (hasPerElementOptionalComponent<EL_TYPE, COMP_TYPE>());

protected:
	template<typename Cont>
	void compactContainer();

	template<typename Cont>
	void clearContainer();

	template<ElementConcept Element>
	void updateAllPointers(const Element* oldBase, const Element* newBase);

	template<typename Cont, typename Element>
	void updatePointers(
		const Element* oldBase,
		const Element* newBase);

	template<ElementConcept Element>
	void updateAllPointersAfterCompact(const Element* base, const std::vector<uint>& newIndices);

	template<typename Cont, typename Element>
	void updatePointersAfterCompact(
		const Element*          base,
		const std::vector<uint>& newIndices);

private:
	// hide init and isEnabled members
	void init() {};
	bool isEnabled() { return true; }

	// enable optional components

	template<typename Cont, typename OtherMeshType>
	void enableSameOptionalComponentsOf(const OtherMeshType& m);

	// private parent mesh pointers functions

	void updateAllParentMeshPointers();

	template<typename Cont>
	void setParentMeshPointers();
	
	// private import member functions

	template<typename Cont, typename OthMesh>
	void importPointers(const OthMesh& m);

	template<typename Cont, typename ElemCont, typename OthMesh>
	void importPointersOfElement(const OthMesh& m);

	// private copy and swap member functions

	template<uint i, typename Cont, typename Array, typename... A>
	static void setContainerBase(const Mesh<A...>& m, Array& bases);

	template<typename... A>
	static auto getContainerBases(const Mesh<A...>& m);

	template<typename Cont, typename Array, typename... A>
	static void updatePointersOfContainerType(Mesh<A...>& m, const Array& bases);

	// member functions used by friends

	template<uint EL_TYPE, typename T>
	uint elementIndex(const T* el) const;

	template<typename El>
	auto& customComponents();

	template<typename El>
	const auto& customComponents() const;
	
	template<typename El>
	auto& verticalComponents();
	
	template<typename El>
	const auto& verticalComponents() const;

	// Predicate structures

	template<uint EL_TYPE>
	struct ContainerOfElement : public mesh::ContainerOfElement<EL_TYPE, Mesh<Args...>> {};

	/**
	 * @brief The ContainerOf struct allows to get the Container of an Element on this Mesh.
	 *
	 * Usage:
	 *
	 * ```cpp
	 * using Container = ContainerOf<ElementType>::type;
	 * ```
	 */
	template<ElementConcept El>
	struct ContainerOf : public ContainerOfElement<El::ELEMENT_TYPE>
	{
	};
};

} // namespace vcl

#include "mesh.cpp"

#endif // VCL_MESH_MESH_H
