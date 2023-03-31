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

#ifndef VCL_ITERATOR_VIEW_H
#define VCL_ITERATOR_VIEW_H

namespace vcl {

/**
 * @brief The View class is a simple class that stores and exposes two iterators begin and end.
 *
 * It is useful for classes that expose multiple containers, and they do not expose the classic
 * member functions begin()/end().
 * In these cases, it is possible to expose the view of a selected container by returning a View
 * object initialized with the begin/end iterators.
 *
 * For example, a Mesh can expose Vertex and Face containers.
 * The mesh exposes the member functions:
 * - vertexBegin()
 * - vertexEnd()
 * - faceBegin()
 * - faceEnd()
 * To allow view iteration over vertices, the Mesh could expose a vertices() member function that
 * returns a View object that is constructed in this way: View(vertexBegin(), vertexEnd());
 */
template<typename It>
class View
{
public:
	using iterator = It;

	View(It begin, It end) : b(begin), e(end) {}

	It begin() const { return b; }

	It end() const { return e; }

protected:
	It b, e;
};

} // namespace vcl

#endif // VCL_ITERATOR_VIEW_H
