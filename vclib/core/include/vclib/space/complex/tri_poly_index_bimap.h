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

#ifndef VCL_SPACE_COMPLEX_TRI_POLY_INDEX_BIMAP_H
#define VCL_SPACE_COMPLEX_TRI_POLY_INDEX_BIMAP_H

#include <vclib/types.h>

#include <vector>

namespace vcl {

/**
 * @brief The TriPolyIndexBiMap class allows to store a bidirectional mapping
 * between a Polygon Mesh and a Triangle Mesh generated by the polygon mesh.
 *
 * The class stores, for each triangle index, the index of the polygon that
 * generated the triangle, and for each polygon, the first triangle index
 * generated by that polygon. Therefore, keep in mind that the mapping assumes
 * that, for each polygon that generates N triangles, the N triangles are
 * contiguous in the Triangle Mesh.
 *
 * Queries have O(1) complexity.
 * Insertion may have O(1) or O(n), depending on the size of the containers used
 * to store indices (same way of std::vector push_back() memeber function). You
 * can also reserve before, in order to be sure that the insertion will be O(1).
 *
 * @ingroup space_complex
 */
class TriPolyIndexBiMap
{
    std::vector<uint> mTriToPoly;
    std::vector<uint> mPolyToTri;

public:
    /**
     * @brief Creates an empty BiMap.
     */
    TriPolyIndexBiMap() = default;

    /**
     * @brief Returns the index of the polygon mapped to the triangle having the
     * index given as input argument.
     * @param[in] triangleIndex: the triangle index for which we need to know
     * its mapped polygon
     * @return the index of the polygon mapped to the given triangle
     */
    uint polygon(uint triangleIndex) const
    {
        assert(triangleIndex < mTriToPoly.size());
        return mTriToPoly[triangleIndex];
    }

    /**
     * @brief Returns the smallest index of set of triangles mapped to the
     * polygon having the index given as input argument. To know how many
     * (consecutive index) triangles are associated to the given polygon, you
     * can use the triangleNumber() member function.
     * @param[in] polygonIndex: the polygon index for which we need to know the
     * first mapped triangleBegin
     * @return the smallest index of the set of triangles mapped to the given
     * polygon
     */
    uint triangleBegin(uint polygonIndex) const
    {
        assert(polygonIndex < mPolyToTri.size());
        return mPolyToTri[polygonIndex];
    }

    /**
     * @brief Returns the number of (consecutive index) triangles mapped to a
     * polygon.
     *
     * You can use this function to know how many triangles are mapped to a
     * polygon.
     *
     * Assuming that you have a list of triangles "list_t", and a polygon "p":
     *
     * @code{.cpp}
     * firstTri = map.triangleBegin(p);
     * nTris = map.triangleNumber(p);
     *
     * // loop into the triangles generated by p
     * for (uint t = firstTri; t < firstTri + nTris; ++t) {
     *     // use list_t[t];
     * }
     * @endcode
     *
     * @param[in] polygonIndex: the polygon index for which we need to know the
     * mapped triangles number
     * @return the number of triangles mapped to the given polygon
     */
    uint triangleNumber(uint polygonIndex) const
    {
        assert(polygonIndex < mPolyToTri.size());
        if (polygonIndex < mPolyToTri.size() - 1) {
            return mPolyToTri[polygonIndex + 1] - mPolyToTri[polygonIndex];
        }
        else {
            // total number of triangles minus the first triangle index of the
            // polygon
            return mTriToPoly.size() - mPolyToTri[polygonIndex];
        }
    }

    /**
     * @brief Clears the BiMap.
     */
    void clear()
    {
        mTriToPoly.clear();
        mPolyToTri.clear();
    }

    /**
     * @brief Reserves enogh memory for the BiMap. Allows fast insertions.
     * @param[in] nTriangles: number of triangles that will be inserted in the
     * BiMap.
     * @param[in] nPolygons: number of polygons that will be inserted in the
     * BiMap.
     */
    void reserve(uint nTriangles, uint nPolygons)
    {
        mTriToPoly.reserve(nTriangles);
        mPolyToTri.reserve(nPolygons);
    }

    /**
     * @brief Performs an insertion into the BiMap, and associates:
     *
     * - the given polygon index to the given triangle
     * - the given triangle index to the given polygon, only if there was no a
     *   smaller triangle index already associated to the given polygon. In this
     *   way, there will always be the smallest triangle index associated to
     *   every polygon.
     *
     * @param[in] triangleIndex: the triangle index that will be inserted in the
     * BiMap.
     * @param[in] polygonIndex: the polygon index that will be inserted in the
     * BiMap.
     */
    void insert(uint triangleIndex, uint polygonIndex)
    {
        // add the index of the polygon associated to the triangle
        if (triangleIndex >= mTriToPoly.size()) {
            mTriToPoly.resize(triangleIndex + 1, UINT_NULL);
        }
        mTriToPoly[triangleIndex] = polygonIndex;

        // add the index of the triangle associated to the polygon,
        // but only if it is the first triangle index of the polygon!
        if (polygonIndex >= mPolyToTri.size()) {
            mPolyToTri.resize(polygonIndex + 1, UINT_NULL);
        }
        if (mPolyToTri[polygonIndex] == UINT_NULL ||
            triangleIndex < mPolyToTri[polygonIndex])
            mPolyToTri[polygonIndex] = triangleIndex;
    }

    /**
     * @brief Returns the number of triangles stored in the BiMap.
     * @return the number of triangles stored in the BiMap.
     */
    uint triangleNumber() const { return mTriToPoly.size(); }

    /**
     * @brief Returns the number of polygons stored in the BiMap.
     * @return the number of polygons stored in the BiMap.
     */
    uint polygonNumber() const { return mPolyToTri.size(); }
};

} // namespace vcl

#endif // VCL_SPACE_COMPLEX_TRI_POLY_INDEX_BIMAP_H
