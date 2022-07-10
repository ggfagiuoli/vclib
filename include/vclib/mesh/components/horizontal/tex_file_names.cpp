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

#include "tex_file_names.h"

namespace vcl::comp {

inline TexFileNames::TexFileNames()
{
}

inline uint TexFileNames::textureNumber() const
{
	return textureNames.size();
}

inline const std::string& TexFileNames::texture(uint i) const
{
	return textureNames[i];
}

inline std::string& TexFileNames::texture(uint i)
{
	return textureNames[i];
}

inline void TexFileNames::clearTextures()
{
	textureNames.clear();
}

inline void TexFileNames::pushTexture(const std::string& textName)
{
	textureNames.push_back(textName);
}

inline TexFileNames::TexFileNamesIterator TexFileNames::textureBegin()
{
	return textureNames.begin();
}

inline TexFileNames::TexFileNamesIterator TexFileNames::textureEnd()
{
	return textureNames.end();
}

inline TexFileNames::ConstTexFileNamesIterator TexFileNames::textureBegin() const
{
	return textureNames.begin();
}

inline TexFileNames::ConstTexFileNamesIterator TexFileNames::textureEnd() const
{
	return textureNames.end();
}

inline TexFileNames::TexFileNamesRangeIterator TexFileNames::textures()
{
	return TexFileNamesRangeIterator(
		*this, &TexFileNames::textureBegin, &TexFileNames::textureEnd);
}

inline TexFileNames::ConstTexFileNamesRangeIterator TexFileNames::textures() const
{
	return ConstTexFileNamesRangeIterator(
		*this, &TexFileNames::textureBegin, &TexFileNames::textureEnd);
}

template<typename Element>
void TexFileNames::importFrom(const Element& e)
{
	if constexpr (HasTexFileNames<Element>) {
		textureNames = e.TexFileNames::textureNames;
	}
}

} // namespace vcl::comp
