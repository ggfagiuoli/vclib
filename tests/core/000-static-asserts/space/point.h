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

#ifndef POINT_H
#define POINT_H

#include <vclib/space.h>

void pointStaticAsserts()
{
    using namespace vcl;

    using PointType  = Point<uint16_t, 5>;
    using Point2Type = Point2i;
    using Point3Type = Point3f;
    using Point4Type = Point4d;

    PointType p = Eigen::Matrix<uint16_t, 1, 5>();

    // point
    static_assert(
        PointConcept<PointType>, "Point does not satisfy the PointConcept");
    static_assert(
        PointConcept<const PointType>,
        "const Point does not satisfy the PointConcept");
    static_assert(
        PointConcept<PointType&>, "Point& does not satisfy the PointConcept");
    static_assert(
        PointConcept<const PointType&>,
        "const Point& does not satisfy the PointConcept");
    static_assert(
        PointConcept<PointType&&>, "Point&& does not satisfy the PointConcept");

    // point2
    static_assert(
        Point2Concept<Point2Type>, "Point2 does not satisfy the Point2Concept");
    static_assert(
        Point2Concept<const Point2Type>,
        "const Point2 does not satisfy the Point2Concept");
    static_assert(
        Point2Concept<Point2Type&>,
        "Point2& does not satisfy the Point2Concept");
    static_assert(
        Point2Concept<const Point2Type&>,
        "const Point2& does not satisfy the Point2Concept");
    static_assert(
        Point2Concept<Point2Type&&>,
        "Point2&& does not satisfy the Point2Concept");

    // point3
    static_assert(
        Point3Concept<Point3Type>, "Point3 does not satisfy the Point3Concept");
    static_assert(
        Point3Concept<const Point3Type>,
        "const Point3 does not satisfy the Point3Concept");
    static_assert(
        Point3Concept<Point3Type&>,
        "Point3& does not satisfy the Point3Concept");
    static_assert(
        Point3Concept<const Point3Type&>,
        "const Point3& does not satisfy the Point3Concept");
    static_assert(
        Point3Concept<Point3Type&&>,
        "Point3&& does not satisfy the Point3Concept");

    // point4
    static_assert(
        Point4Concept<Point4Type>, "Point4 does not satisfy the Point4Concept");
    static_assert(
        Point4Concept<const Point4Type>,
        "const Point4 does not satisfy the Point4Concept");
    static_assert(
        Point4Concept<Point4Type&>,
        "Point4& does not satisfy the Point4Concept");
    static_assert(
        Point4Concept<const Point4Type&>,
        "const Point4& does not satisfy the Point4Concept");
    static_assert(
        Point4Concept<Point4Type&&>,
        "Point4&& does not satisfy the Point4Concept");
}

#endif // POINT_H
