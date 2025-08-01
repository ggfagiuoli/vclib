/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
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

#ifndef VCL_MISC_SHUFFLE_H
#define VCL_MISC_SHUFFLE_H

#include <vclib/concepts.h>

#include <algorithm>
#include <random>

namespace vcl {

/**
 * @brief Shuffle the elements of a range.
 *
 * @tparam R: Type of the range.
 * @param[in] range: Range to shuffle.
 * @param[in] deterministic: If true, the shuffle will be deterministic.
 *
 * @ingroup miscellaneous
 */
template<Range R>
void shuffle(R&& range, bool deterministic = false)
{
    std::random_device rd;
    std::mt19937       generator(deterministic ? 0 : rd());
    std::shuffle(range.begin(), range.end(), generator);
}

} // namespace vcl

#endif // VCL_MISC_SHUFFLE_H
