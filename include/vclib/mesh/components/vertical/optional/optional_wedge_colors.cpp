/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
 * Alessandro Muntoni                                                        *
 * VCLab - ISTI - Italian National Research Council                          *
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

#include "optional_wedge_colors.h"

namespace vcl::comp {

template<int N, typename T>
OptionalWedgeColors<N, T>::OptionalWedgeColors()
{
}

template<int N, typename T>
vcl::Color& OptionalWedgeColors<N, T>::wedgeColor(uint i)
{
	assert(i < wedgeColorsNumber());
	return B::optCont().wedgeColors(thisId())[i];
}

template<int N, typename T>
const vcl::Color& OptionalWedgeColors<N, T>::wedgeColor(uint i) const
{
	assert(i < wedgeColorsNumber());
	return B::optCont().wedgeColors(thisId())[i];
}

template<int N, typename T>
vcl::Color& OptionalWedgeColors<N, T>::wedgeColorMod(int i)
{
	int n = wedgeColorsNumber();
	return B::optCont().wedgeColors(thisId())[(i % n + n) % n];
}

template<int N, typename T>
const vcl::Color& OptionalWedgeColors<N, T>::wedgeColorMod(int i) const
{
	int n = wedgeColorsNumber();
	return B::optCont().wedgeColors(thisId())[(i % n + n) % n];
}

template<int N, typename T>
void OptionalWedgeColors<N, T>::setWedgeColor(const vcl::Color& t, uint i)
{
	assert(i < wedgeColorsNumber());
	B::optCont().wedgeColors(thisId())[i] = t;
}

template<int N, typename T>
void OptionalWedgeColors<N, T>::setWedgeColors(const std::vector<vcl::Color>& list)
{
	if constexpr (N >= 0) {
		assert(list.size() == N);
		uint i = 0;
		for (const auto& t : list) {
			setWedgeColor(t, i);
			++i;
		}
	}
	else {
		B::optCont().wedgeColors(thisId()) = list;
	}
}

template<int N, typename T>
bool OptionalWedgeColors<N, T>::isWedgeColorsEnabled() const
{
	return B::optCont().isWedgeColorsEnabled();
}

template<int N, typename T>
typename OptionalWedgeColors<N, T>::WedgeColorsIterator OptionalWedgeColors<N, T>::wedgeColorBegin()
{
	return B::optCont().wedgeColors(thisId()).begin();
}

template<int N, typename T>
typename OptionalWedgeColors<N, T>::WedgeColorsIterator OptionalWedgeColors<N, T>::wedgeColorEnd()
{
	return B::optCont().wedgeColors(thisId()).end();
}

template<int N, typename T>
typename OptionalWedgeColors<N, T>::ConstWedgeColorsIterator
OptionalWedgeColors<N, T>::wedgeColorBegin() const
{
	return B::optCont().wedgeColors(thisId()).begin();
}

template<int N, typename T>
typename OptionalWedgeColors<N, T>::ConstWedgeColorsIterator
OptionalWedgeColors<N, T>::wedgeColorEnd() const
{
	return B::optCont().wedgeColors(thisId()).end();
}

template<int N, typename T>
typename OptionalWedgeColors<N, T>::WedgeColorsRangeIterator
OptionalWedgeColors<N, T>::wedgeColors()
{
	return WedgeColorsRangeIterator(
		*this,
		&OptionalWedgeColors::wedgeColorBegin,
		&OptionalWedgeColors::wedgeColorEnd);
}

template<int N, typename T>
typename OptionalWedgeColors<N, T>::ConstWedgeColorsRangeIterator
OptionalWedgeColors<N, T>::wedgeColors() const
{
	return ConstWedgeColorsRangeIterator(
		*this,
		&OptionalWedgeColors::wedgeColorBegin,
		&OptionalWedgeColors::wedgeColorEnd);
}

template<int N, typename T>
void OptionalWedgeColors<N, T>::resizeWedgeColors(uint n) requires (N < 0)
{
	B::optCont().wedgeColors(thisId()).resize(n);
}

template<int N, typename T>
void OptionalWedgeColors<N, T>::pushWedgeColor(const vcl::Color& t) requires (N < 0)
{
	B::optCont().wedgeColors(thisId()).push_back(t);
}

template<int N, typename T>
void OptionalWedgeColors<N, T>::insertWedgeColor(uint i, const vcl::Color& t) requires (N < 0)
{
	assert(i < wedgeColorsNumber() + 1);
	B::optCont().wedgeColors(thisId()).insert(B::optCont().wedgeColors(thisId()).begin() + i, t);
}

template<int N, typename T>
void OptionalWedgeColors<N, T>::eraseWedgeColor(uint i) requires (N < 0)
{
	assert(i < wedgeColorsNumber());
	B::optCont().wedgeColors(thisId()).erase(B::optCont().wedgeColors(thisId()).begin() + i);
}

template<int N, typename T>
void OptionalWedgeColors<N, T>::clearWedgeColor() requires (N < 0)
{
	B::optCont().wedgeColors(thisId()).clear();
}

template<int N, typename T>
template<typename Element>
void OptionalWedgeColors<N, T>::importFrom(const Element& e)
{
	if constexpr (HasWedgeColors<Element>) {
		if (isWedgeColorsEnabled() && isWedgeColorsEnabledOn(e)) {
			if constexpr(N > 0) {
				// same static size
				if constexpr (N == Element::WEDGE_COLOR_NUMBER) {
					importWedgeColorsFrom(e);
				}
				// from dynamic to static, but dynamic size == static size
				else if constexpr (Element::WEDGE_COLOR_NUMBER < 0){
					if (e.vertexNumber() == N){
						importWedgeColorsFrom(e);
					}
				}
				else {
					// do not import in this case: cannot import from dynamic size != static size
				}
			}
			else {
				// from static/dynamic to dynamic size: need to resize first, then import
				resizeWedgeColors(e.vertexNumber());
				importWedgeColorsFrom(e);
			}
		}
	}
}

template<int N, typename T>
template<typename Element>
void OptionalWedgeColors<N, T>::importWedgeColorsFrom(const Element& e)
{
	for (uint i = 0; i < e.vertexNumber(); ++i){
		wedgeColor(i) = e.wedgeColor(i);
	}
}

template<int N, typename T>
uint OptionalWedgeColors<N, T>::wedgeColorsNumber() const
{
	if constexpr (N >= 0) {
		return N;
	}
	else {
		return B::optCont().wedgeColors(thisId()).size();
	}
}

} // namespace vcl::comp
