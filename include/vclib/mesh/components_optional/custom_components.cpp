/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual Computing Library                                        o     o   *
 *                                                                 _  O  _   *
 * Copyright(C) 2021-2022                                           \/)\/    *
 * Visual Computing Lab                                            /\/|      *
 * ISTI - Italian National Research Council                           |      *
 *                                                                    \      *
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

#include "custom_components.h"

namespace vcl::comp {

template<typename T>
bool CustomComponents<T>::hasCustomComponent(const std::string& attrName) const
{
	return B::optCont().componentExists(attrName);
}

template<typename T>
template<typename CompType>
const CompType& CustomComponents<T>::customComponent(const std::string& attrName) const
{
	return std::any_cast<const CompType&>(
		B::optCont().template componentVector<CompType>(attrName)[thisId()]);
}

template<typename T>
template<typename CompType>
CompType& CustomComponents<T>::customComponent(const std::string& attrName)
{
	return std::any_cast<CompType&>(
		B::optCont().template componentVector<CompType>(attrName)[thisId()]);
}

template<typename T>
template<typename Element>
void CustomComponents<T>::importFrom(const Element&)
{
	// todo
}

} // namespace vcl::comp
