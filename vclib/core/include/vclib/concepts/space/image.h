/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#ifndef VCL_CONCEPTS_SPACE_IMAGE_H
#define VCL_CONCEPTS_SPACE_IMAGE_H

#include <string>

#include <vclib/types.h>

#include "color.h"

namespace vcl {

template<typename T>
concept ImageConcept = requires (T&& obj) {
    { obj.isNull() } -> std::same_as<bool>;
    { obj.height() } -> std::same_as<int>;
    { obj.width() } -> std::same_as<int>;

    { obj.sizeInBytes() } -> std::same_as<std::size_t>;

    { obj.pixel(uint(), uint()) } -> ColorConcept;

    { obj.data() } -> std::same_as<const unsigned char*>;

    { obj.load(std::string()) } -> std::same_as<bool>;
    { obj.save(std::string()) } -> std::same_as<void>;
    { obj.save(std::string(), uint()) } -> std::same_as<void>;

    { obj.mirror() } -> std::same_as<void>;
    { obj.mirror(bool()) } -> std::same_as<void>;
    { obj.mirror(bool(), bool()) } -> std::same_as<void>;
};

} // namespace vcl

#endif // VCL_CONCEPTS_SPACE_IMAGE_H
