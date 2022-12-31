/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
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

#ifndef VCL_SPACE_POINT_POINT2_H
#define VCL_SPACE_POINT_POINT2_H

#include "point_t.h"

namespace vcl {

/**
 * @brief The Point2 class represents a 2-dimensional point containing 2 scalar values.
 * Specializes the Point class, providing some additional member functions useful for 2D points.
 */
template<typename ScalarType>
class Point2 : public Point<ScalarType, 2>
{
public:
	using Point<ScalarType, 2>::Point;

	Point2() = default; // default empty constructor

	Point2(const Point<ScalarType, 2>& p); // from base class - will include all its constructors
	Point2(const ScalarType& x, const ScalarType& y);

	ScalarType x() const;
	ScalarType y() const;
	ScalarType& x();
	ScalarType& y();
};

using Point2i = Point2<int>;
using Point2f = Point2<float>;
using Point2d = Point2<double>;

// makes sure that Point2 classes satisfy PointConcept
static_assert(PointConcept<Point2i>, "Point2i is not a valid Point");
static_assert(PointConcept<Point2f>, "Point2f is not a valid Point");
static_assert(PointConcept<Point2d>, "Point2d is not a valid Point");

} // namespace vcl

// inject vcl::Point2 hash function in std namespace
namespace std {
template <typename Scalar>
struct hash<vcl::Point2<Scalar> >
{
	size_t operator()(const vcl::Point2<Scalar>& id) const noexcept;
};
} // namespace std

#include "point2.cpp"

#endif // VCL_SPACE_POINT_POINT2_H
