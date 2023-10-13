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

#ifndef VCL_TYPES_MESH_ELEMENTS_H
#define VCL_TYPES_MESH_ELEMENTS_H

#include "base.h"

namespace vcl {

enum ElementIDEnum : uint {
    VERTEX = 0,
    FACE,
    EDGE,
};

inline static constexpr uint ELEMENTS_NUMBER = 3;

inline static constexpr const char* ELEMENT_ENUM_STRINGS[ELEMENTS_NUMBER] = {
    "Vertex",
    "Face",
    "Edge",
};

/**
 * @brief The ElemenetString class is used to retrieve the string associated
 * to a ELEM_ID value, trough its member 'str'.
 *
 * If you use a custom element class, you should specialize this struct with
 * your ELEM_ID value (that is >= ELEMENTS_NUMBER).
 *
 * @tparam ELEM_ID: The ELEM_ID value associated to the string.
 */
template<uint ELEM_ID>
struct ElemenetString
{
    /**
     * @brief The string associated to the ELEM_ID.
     */
    const char* str =
        ELEM_ID < ELEMENTS_NUMBER ? ELEMENT_ENUM_STRINGS[ELEM_ID] : nullptr;
};

template<uint ELEM_ID>
constexpr const char* elementEnumString()
{
    static_assert(
        ElemenetString<ELEM_ID>().str != nullptr,
        "Invalid ElementIDEnum. You should specialize 'the ElementString' "
        "struct with your ELEM_ID value.");

    return ElemenetString<ELEM_ID>().str;
}

} // namespace vcl

#endif // VCL_TYPES_MESH_ELEMENTS_H
