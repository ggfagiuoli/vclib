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

#ifndef VCL_MESH_COMPONENTS_NAME_H
#define VCL_MESH_COMPONENTS_NAME_H

#include <vclib/concepts/mesh/components/name.h>

#include "component.h"

namespace vcl::comp {

/**
 * @brief The Name component class represents a simple name stored as string. This class is
 * usually used as a component of a Mesh.
 *
 * The member functions of this class will be available in the instance of any Element or Mesh that
 * will contain this component.
 *
 * For example, if you have a Mesh `m` with the Name component, you'll be able to
 * access to this component member functions from `m`:
 *
 * @code{.cpp}
 * m.name();
 * @endcode
 *
 * @ingroup components
 */
template<typename ElementType = void, bool optional = false>
class Name : public Component<std::string, ElementType, optional>
{
	using Base = Component<std::string, ElementType, optional>;
	using ThisType = Name<ElementType, optional>;

public:
	using NameComponent = ThisType; // expose the type to allow access to this component

	std::string& name();
	const std::string& name() const;

protected:
	template<typename Element>
	void importFrom(const Element& e);
};

} // namespace vcl::comp

#include "name.cpp"

#endif // VCL_MESH_COMPONENTS_NAME_H
