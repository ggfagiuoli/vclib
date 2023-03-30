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

#ifndef VCL_MESH_ITERATORS_COMPONENT_COORD_ITERATOR_H
#define VCL_MESH_ITERATORS_COMPONENT_COORD_ITERATOR_H

#include "component_range.h"

#include <vclib/iterator/iterator_wrapper.h>

namespace vcl {

template<typename T>
class CoordIterator
{
	CoordIterator(T) {}
	static_assert(sizeof(T) != sizeof(T), "");
};

// Specialization when using a pointer instead of an actual iterator - forces to use the variant
// with an iterator, by wrapping the pointer type into a forward iterator
template<IsPointer PointerType>
class CoordIterator<PointerType> : public CoordIterator<IteratorWrapper<PointerType>>
{
public:
	using CoordIterator<IteratorWrapper<PointerType>>::CoordIterator;
};

/**
 * @brief The CoordIterator class allows to iterate over the coordinates of the elements given
 * an iterator It that iterates over the elements (or pointer to elements).
 */
template<IteratorConcept It>
class CoordIterator<It> : public It
{
	using ElementType = typename std::conditional_t<
		IteratesOverClass<It>,
		typename std::remove_pointer_t<typename It::pointer>,
		typename std::remove_pointer_t<typename It::value_type>>;

	using CompType = typename std::conditional_t<
		std::is_const_v<ElementType>,
		const typename ElementType::CoordType,
		typename ElementType::CoordType>;

public:
	using value_type = typename std::remove_const_t<CompType>;
	using reference  = CompType&;
	using pointer    = CompType*;

	using It::It;
	CoordIterator(const It& it) : It(it) {}

	reference operator*() const
	{
		if constexpr (IteratesOverClass<It>) {
			return It::operator*().coord();
		}
		else {
			return It::operator*()->coord();
		}
	}
	pointer operator->() const
	{
		return &(operator*());
	}
};

// todo: remove this when clang will support P1814R0 (https://clang.llvm.org/cxx_status.html)
#ifdef __clang__
template<typename Rng>
class CoordRange : public vcl::Range<CoordIterator<typename Rng::iterator>>
{
	using Base = vcl::Range<CoordIterator<typename Rng::iterator>>;
public:
	CoordRange(const Rng& r) :
			Base(CoordIterator(r.begin()), CoordIterator(r.end()))
	{
	}
};
#else
template<typename Rng>
using CoordRange = internal::ComponentRange<Rng, CoordIterator>;
#endif

} // namespace vcl

#endif // VCL_MESH_ITERATORS_COMPONENT_COORD_ITERATOR_H
