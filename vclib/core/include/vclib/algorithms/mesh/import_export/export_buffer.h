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

#ifndef VCL_ALGORITHMS_MESH_IMPORT_EXPORT_EXPORT_BUFFER_H
#define VCL_ALGORITHMS_MESH_IMPORT_EXPORT_EXPORT_BUFFER_H

#include <vclib/algorithms/core/polygon/ear_cut.h>
#include <vclib/mesh/requirements.h>
#include <vclib/space/complex/tri_poly_index_bimap.h>
#include <vclib/views/mesh.h>

/**
 * @defgroup export_buffer Export Mesh to Buffer Algorithms
 *
 * @ingroup import_export
 *
 * @brief List Export Mesh to Buffer algorithms.
 *
 * They allow to export mesh data to pre-allocated buffers.
 *
 * You can access these algorithms by including `#include
 * <vclib/algorithms/mesh/import_export.h>`
 */

namespace vcl {

namespace detail {

// returns a non-empty vector if the vertex container is not compact and the
// user wants compact indices
std::vector<uint> vertCompactIndices(const auto& mesh, bool wantCompact)
{
    std::vector<uint> vertCompIndices;

    bool isCompact = mesh.vertexNumber() == mesh.vertexContainerSize();

    if (wantCompact && !isCompact)
        vertCompIndices = mesh.vertexCompactIndices();
    return vertCompIndices;
}

// lambda to get the vertex index of a face (considering compact vertex indices)
auto vIndexLambda(const auto& mesh, const std::vector<uint>& vertCompIndices)
{
    // lambda to get the vertex index of a face (considering compact indices)
    auto vIndex = [&vertCompIndices](const auto& f, uint i) {
        if (vertCompIndices.size() > 0)
            return vertCompIndices[f.vertexIndex(i)];
        else
            return f.vertexIndex(i);
    };

    return vIndex;
}

inline static TriPolyIndexBiMap indexMap;

} // namespace detail

/**
 * @brief Export the vertex coordinates of a mesh to a buffer.
 *
 * This function exports the vertex coordinates of a mesh to a buffer. Vertices
 * are stored in the buffer following the order they appear in the mesh.  The
 * buffer must be preallocated with the correct size (number of vertices times
 * the number of coordinates per vertex).
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the vertex indices of the mesh. This scenario is possible
 * when the mesh has deleted vertices. To be sure to have a direct
 * correspondence, compact the vertex container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] storage: storage type of the matrix (row or column major)
 * @param[in] rowNumber: number of rows of the matrix (if different from the
 * number of vertices in the mesh) - used only when storage is column major
 *
 * @ingroup export_buffer
 */
template<MeshConcept MeshType>
void vertexCoordsToBuffer(
    const MeshType&   mesh,
    auto*             buffer,
    MatrixStorageType storage = MatrixStorageType::ROW_MAJOR,
    uint rowNumber = UINT_NULL)
{
    if (storage == MatrixStorageType::ROW_MAJOR) {
        for (uint i = 0; const auto& c : mesh.vertices() | views::coords) {
            buffer[i * 3 + 0] = c.x();
            buffer[i * 3 + 1] = c.y();
            buffer[i * 3 + 2] = c.z();
            ++i;
        }
    }
    else {
        const uint VERT_NUM =
            rowNumber == UINT_NULL ? mesh.vertexNumber() : rowNumber;
        for (uint i = 0; const auto& c : mesh.vertices() | views::coords) {
            buffer[0 * VERT_NUM + i] = c.x();
            buffer[1 * VERT_NUM + i] = c.y();
            buffer[2 * VERT_NUM + i] = c.z();
            ++i;
        }
    }
}

/**
 * @brief Export into a buffer the vertex indices for each triangle of a Mesh.
 *
 * This function exports the vertex indices of the triangles of a mesh to a
 * buffer. Indices are stored following the order the faces appear in the mesh.
 * The buffer must be preallocated with the correct size (number of faces times
 * 3). The function assumes that the input mesh is a triangle mesh (if there are
 * polygonal faces, only the first three vertices are considered).
 *
 * @note As a default behaviour (`getIndicesAsIfContainerCompact == true`) the
 * function stores the vertex indices as if the vertex container of the mesh is
 * compact. This means that, if the mesh has deleted vertices, the vertex
 * indices stored in the buffer may not correspond to the vertex indices of the
 * mesh. If you want to store the actual vertex indices in the input mesh, set
 * `getIndicesAsIfContainerCompact` to false.
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the face indices of the mesh. This scenario is possible when
 * the mesh has deleted faces. To be sure to have a direct correspondence,
 * compact the face container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] storage: storage type of the matrix (row or column major)
 * @param[in] getIndicesAsIfContainerCompact: if true, the function will
 * store the vertex indices as if the vertex container of the mesh is compact.
 * If false, the actual vertex indices in the input mesh will be stored.
 * @param[in] rowNumber: number of rows of the matrix (if different from the
 * number of faces in the mesh) - used only when storage is column major
 *
 * @ingroup export_buffer
 */
template<FaceMeshConcept MeshType>
void triangleIndicesToBuffer(
    const MeshType&   mesh,
    auto*             buffer,
    MatrixStorageType storage = MatrixStorageType::ROW_MAJOR,
    bool              getIndicesAsIfContainerCompact = true,
    uint              rowNumber = UINT_NULL)
{
    const std::vector<uint> vertCompIndices =
        detail::vertCompactIndices(mesh, getIndicesAsIfContainerCompact);

    // lambda to get the vertex index of a face (considering compact indices)
    auto vIndex = detail::vIndexLambda(mesh, vertCompIndices);

    if (storage == MatrixStorageType::ROW_MAJOR) {
        for (uint i = 0; const auto& f : mesh.faces()) {
            buffer[i * 3 + 0] = vIndex(f, 0);
            buffer[i * 3 + 1] = vIndex(f, 1);
            buffer[i * 3 + 2] = vIndex(f, 2);
            ++i;
        }
    }
    else {
        const uint FACE_NUM =
            rowNumber == UINT_NULL ? mesh.faceNumber() : rowNumber;
        for (uint i = 0; const auto& f : mesh.faces()) {
            buffer[0 * FACE_NUM + i] = vIndex(f, 0);
            buffer[1 * FACE_NUM + i] = vIndex(f, 1);
            buffer[2 * FACE_NUM + i] = vIndex(f, 2);
            ++i;
        }
    }
}

/**
 * @brief Export into a buffer the sizes of the faces of a Mesh, and return the
 * sum of the sizes.
 *
 * This function could be useful when dealing with polygonal meshes: it exports
 * the sizes of the faces of a mesh to a buffer. Sizes are stored following the
 * order the faces appear in the mesh. The buffer must be preallocated with the
 * correct size (number of faces).
 *
 * The return value is the sum of the sizes of the faces. This value is useful
 * when you need to allocate a buffer to store the vertex indices of the faces
 * (its size is the sum of the face sizes).
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the face indices of the mesh. This scenario is possible
 * when the mesh has deleted faces. To be sure to have a direct
 * correspondence, compact the face container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @return sum of the sizes of the faces
 *
 * @ingroup export_buffer
 */
template<FaceMeshConcept MeshType>
uint faceSizesToBuffer(const MeshType& mesh, auto* buffer)
{
    uint sum = 0;
    for (uint i = 0; const auto& f : mesh.faces()) {
        buffer[i] = f.vertexNumber();
        sum += f.vertexNumber();
        ++i;
    }
    return sum;
}

/**
 * @brief Export into a buffer the vertex indices for each face of a Mesh. Faces
 * can be polygons.
 *
 * This function exports the vertex indices of the polygonal faces of a mesh to
 * a buffer. Indices are stored consecutively in the buffer, following the order
 * the faces appear in the mesh. The buffer must be preallocated with the
 * correct size (sum of the sizes of the faces).
 *
 * You can use the function @ref vcl::faceSizesToBuffer to get the sizes of the
 * faces and allocate the buffer accordingly:
 *
 * @code{.cpp}
 * std::vector<uint> faceSizes(myMesh.faceNumber());
 * uint sum = vcl::faceSizesToBuffer(myMesh, sizes.data());
 * std::vector<uint> faceIndices(sum);
 * vcl::faceIndicesToBuffer(myMesh, faceIndices.data());
 *
 * // read indices for each face
 * uint offset = 0;
 * for (uint i = 0; i < myMesh.faceNumber(); ++i) {
 *     uint size = faceSizes[i];
 *     for (uint j = 0; j < size; ++j) {
 *         uint vIdx = faceIndices[offset + j];
 *         // do something with the vertex index
 *     }
 *     offset += size;
 * }
 * @endcode
 *
 * @note As a default behaviour (`getIndicesAsIfContainerCompact == true`) the
 * function stores the vertex indices as if the vertex container of the mesh is
 * compact. This means that, if the mesh has deleted vertices, the vertex
 * indices stored in the buffer may not correspond to the vertex indices of the
 * mesh. If you want to store the actual vertex indices in the input mesh, set
 * `getIndicesAsIfContainerCompact` to false.
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the face indices of the mesh. This scenario is possible when
 * the mesh has deleted faces. To be sure to have a direct correspondence,
 * compact the face container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] getIndicesAsIfContainerCompact: if true, the function will
 * store the vertex indices as if the vertex container of the mesh is compact.
 * If false, the actual vertex indices in the input mesh will be stored.
 *
 * @ingroup export_buffer
 */
template<FaceMeshConcept MeshType>
void faceIndicesToBuffer(
    const MeshType& mesh,
    auto*           buffer,
    bool            getIndicesAsIfContainerCompact = true)
{
    const std::vector<uint> vertCompIndices =
        detail::vertCompactIndices(mesh, getIndicesAsIfContainerCompact);

    // lambda to get the vertex index of a face (considering compact indices)
    auto vIndex = detail::vIndexLambda(mesh, vertCompIndices);

    for (uint i = 0; const auto& f : mesh.faces()) {
        for (uint j = 0; j < f.vertexNumber(); ++j) {
            buffer[i] = vIndex(f, j);
            ++i;
        }
    }
}

/**
 * @brief Export into a buffer the vertex indices for each face of a Mesh. Faces
 * can be polygons.
 *
 * This function exports the vertex indices of the polygonal faces of a mesh to
 * a buffer. Indices are stored following the order the faces appear in the
 * mesh. The buffer must be preallocated with the correct size (number of faces
 * times the size of the largest face size). For each face that has less
 * vertices than the largest face size, the remaining indices are set to -1.
 *
 * You can use the function @ref vcl::largestFaceSize to get the largest face
 * size and allocate the buffer accordingly:
 *
 * @code{.cpp}
 * uint lfs = vcl::largestFaceSize(myMesh);
 * Eigen::MatrixXi faceIndices(myMesh.faceNumber(), lfs);
 * vcl::faceIndicesToBuffer(
 *     myMesh, faceIndices.data(), lfs, MatrixStorageType::COLUMN_MAJOR);
 * @endcode
 *
 * @note As a default behaviour (`getIndicesAsIfContainerCompact == true`) the
 * function stores the vertex indices as if the vertex container of the mesh is
 * compact. This means that, if the mesh has deleted vertices, the vertex
 * indices stored in the buffer may not correspond to the vertex indices of the
 * mesh. If you want to store the actual vertex indices in the input mesh, set
 * `getIndicesAsIfContainerCompact` to false.
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the face indices of the mesh. This scenario is possible when
 * the mesh has deleted faces. To be sure to have a direct correspondence,
 * compact the face container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] largestFaceSize: size of the largest face in the mesh
 * @param[in] storage: storage type of the matrix (row or column major)
 * @param[in] getIndicesAsIfContainerCompact: if true, the function will
 * store the vertex indices as if the vertex container of the mesh is compact.
 * If false, the actual vertex indices in the input mesh will be stored.
 * @param[in] rowNumber: number of rows of the matrix (if different from the
 * number of faces in the mesh) - used only when storage is column major
 *
 * @ingroup export_buffer
 */
template<FaceMeshConcept MeshType>
void faceIndicesToBuffer(
    const MeshType&   mesh,
    auto*             buffer,
    uint              largestFaceSize,
    MatrixStorageType storage = MatrixStorageType::ROW_MAJOR,
    bool              getIndicesAsIfContainerCompact = true,
    uint              rowNumber = UINT_NULL)
{
    const std::vector<uint> vertCompIndices =
        detail::vertCompactIndices(mesh, getIndicesAsIfContainerCompact);

    // lambda to get the vertex index of a face (considering compact indices)
    auto vIndex = detail::vIndexLambda(mesh, vertCompIndices);

    if (storage == MatrixStorageType::ROW_MAJOR) {
        for (uint i = 0; const auto& f : mesh.faces()) {
            uint j = 0;
            for (; j < f.vertexNumber(); ++j)
                buffer[i * largestFaceSize + j] = vIndex(f, j);
            for (; j < largestFaceSize; ++j) // remaining vertices set to -1
                buffer[i * largestFaceSize + j] = -1;
            ++i;
        }
    }
    else {
        const uint FACE_NUM =
            rowNumber == UINT_NULL ? mesh.faceNumber() : rowNumber;
        for (uint i = 0; const auto& f : mesh.faces()) {
            uint j = 0;
            for (; j < f.vertexNumber(); ++j)
                buffer[j * FACE_NUM + i] = vIndex(f, j);
            for (; j < largestFaceSize; ++j) // remaining vertices set to -1
                buffer[j * FACE_NUM + i] = -1;
            ++i;
        }
    }
}

/**
 * @brief Export into a buffer the vertex indices for each triangle computed
 * by triangulating the faces of a Mesh.
 *
 * This function exports the vertex indices of the triangles computed by
 * triangulating the faces of a mesh to a buffer. Indices are stored following
 * the order the faces appear in the mesh. The buffer must be preallocated with
 * the correct size (number of *resulting triangles* times 3).
 *
 * You can use the function @ref vcl::countTriangulatedTriangles to get the
 * number of resulting triangles and allocate the buffer accordingly:
 *
 * @code{.cpp}
 * uint numTris = vcl::countTriangulatedTriangles(myMesh);
 * Eigen::MatrixXi triIndices(numTris, 3);
 * vcl::TriPolyIndexBiMap indexMap;
 * vcl::triangulatedFaceIndicesToBuffer(
 *     myMesh, triIndices.data(), indexMap, MatrixStorageType::COLUMN_MAJOR,
 *     numTris);
 * @endcode
 *
 * The input indexMap is used to map each triangle to the face index. If the
 * storage of the buffer is column major, the number of resulting triangles
 * (that should be known when calling this function) should be given as input.
 * If the number of resulting triangles is not given, the function will compute
 * it again.
 *
 * @note As a default behaviour (`getIndicesAsIfContainerCompact == true`) the
 * function stores the vertex indices as if the vertex container of the mesh is
 * compact. This means that, if the mesh has deleted vertices, the vertex
 * indices stored in the buffer may not correspond to the vertex indices of the
 * mesh. If you want to store the actual vertex indices in the input mesh, set
 * `getIndicesAsIfContainerCompact` to false.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[out] indexMap: map from triangle index to face index
 * @param[in] storage: storage type of the matrix (row or column major)
 * @param[in] numTriangles: number of resulting triangles (necessary only if
 * the storage is column major)
 * @param[in] getIndicesAsIfContainerCompact: if true, the function will
 * store the vertex indices as if the vertex container of the mesh is compact.
 * If false, the actual vertex indices in the input mesh will be stored.
 *
 * @ingroup export_buffer
 */
template<FaceMeshConcept MeshType>
void triangulatedFaceIndicesToBuffer(
    const MeshType&    mesh,
    auto*              buffer,
    TriPolyIndexBiMap& indexMap     = detail::indexMap,
    MatrixStorageType  storage      = MatrixStorageType::ROW_MAJOR,
    uint               numTriangles = UINT_NULL,
    bool               getIndicesAsIfContainerCompact = true)
{
    const std::vector<uint> vertCompIndices =
        detail::vertCompactIndices(mesh, getIndicesAsIfContainerCompact);

    // lambda to get the vertex index of a face (considering compact indices)
    auto vIndex = detail::vIndexLambda(mesh, vertCompIndices);

    // there will be at least a triangle for each polygon
    indexMap.clear();
    indexMap.reserve(mesh.faceNumber(), mesh.faceContainerSize());

    if constexpr (TriangleMeshConcept<MeshType>) {
        // construct the indexMap, which maps each triangle to the face index
        for (uint t = 0; const auto& f : mesh.faces()) {
            // map the ith triangle to the f face
            indexMap.insert(t, f.index());
            ++t;
        }

        return triangleIndicesToBuffer(
            mesh, buffer, storage, getIndicesAsIfContainerCompact);
    }
    else {
        // if the user did not give the number of triangles, and the buffer
        // storage is column major, we need to compute the number of resulting
        // triangles
        if (numTriangles == UINT_NULL &&
            storage == MatrixStorageType::COLUMN_MAJOR &&
            mesh.faceNumber() > 0) {
            numTriangles = countTriangulatedTriangles(mesh);
        }
        for (uint t = 0; const auto& f : mesh.faces()) {
            std::vector<uint> vind = vcl::earCut(f);

            // for each triangle of the triangulation (t is the triangle index)
            for (uint vi = 0; vi < vind.size(); vi += 3) {
                // map the t-th triangle to the f polygonal face
                indexMap.insert(t, f.index());

                if (storage == MatrixStorageType::ROW_MAJOR) {
                    buffer[t * 3 + 0] = vIndex(f, vind[vi + 0]);
                    buffer[t * 3 + 1] = vIndex(f, vind[vi + 1]);
                    buffer[t * 3 + 2] = vIndex(f, vind[vi + 2]);
                }
                else {
                    buffer[0 * numTriangles + t] = vIndex(f, vind[vi + 0]);
                    buffer[1 * numTriangles + t] = vIndex(f, vind[vi + 1]);
                    buffer[2 * numTriangles + t] = vIndex(f, vind[vi + 2]);
                }
                ++t;
            }
        }
    }
}

/**
 * @brief Export into a buffer the vertex indices for each edge of a Mesh.
 *
 * This function exports the vertex indices of the edges of a mesh to a
 * buffer. Indices are stored following the order the edges appear in the mesh.
 * The buffer must be preallocated with the correct size (number of edges times
 * 2).
 *
 * @note As a default behaviour (`getIndicesAsIfContainerCompact == true`) the
 * function stores the vertex indices as if the vertex container of the mesh is
 * compact. This means that, if the mesh has deleted vertices, the vertex
 * indices stored in the buffer may not correspond to the vertex indices of the
 * mesh. If you want to store the actual vertex indices in the input mesh, set
 * `getIndicesAsIfContainerCompact` to false.
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the edge indices of the mesh. This scenario is possible
 * when the mesh has deleted edges. To be sure to have a direct
 * correspondence, compact the edge container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] storage: storage type of the matrix (row or column major)
 * @param[in] getIndicesAsIfContainerCompact: if true, the function will
 * store the vertex indices as if the vertex container of the mesh is compact.
 * If false, the actual vertex indices in the input mesh will be stored.
 * @param[in] rowNumber: number of rows of the matrix (if different from the
 * number of edges in the mesh) - used only when storage is column major
 *
 * @ingroup export_buffer
 */
template<EdgeMeshConcept MeshType>
void edgeIndicesToBuffer(
    const MeshType&   mesh,
    auto*             buffer,
    MatrixStorageType storage = MatrixStorageType::ROW_MAJOR,
    bool              getIndicesAsIfContainerCompact = true,
    uint              rowNumber = UINT_NULL)
{
    const std::vector<uint> vertCompIndices =
        detail::vertCompactIndices(mesh, getIndicesAsIfContainerCompact);

    // lambda to get the vertex index of a edge (considering compact indices)
    auto vIndex = detail::vIndexLambda(mesh, vertCompIndices);

    if (storage == MatrixStorageType::ROW_MAJOR) {
        for (uint i = 0; const auto& e : mesh.edges()) {
            buffer[i * 2 + 0] = vIndex(e, 0);
            buffer[i * 2 + 1] = vIndex(e, 1);
            ++i;
        }
    }
    else {
        const uint EDGE_NUM =
            rowNumber == UINT_NULL ? mesh.edgeNumber() : rowNumber;
        for (uint i = 0; const auto& e : mesh.edges()) {
            buffer[0 * EDGE_NUM + i] = vIndex(e, 0);
            buffer[1 * EDGE_NUM + i] = vIndex(e, 1);
            ++i;
        }
    }
}

/**
 * @brief Export the selection status of the elements identified by `ELEM_ID` of
 * a mesh to a buffer.
 *
 * This function exports the selection status of the elements identified by
 * `ELEM_ID` of a mesh to a buffer. Values are stored in the buffer following
 * the order the elements appear in the mesh. The buffer must be preallocated
 * with the correct size (number of elements).
 *
 * Usage example with std::vector<bool>:
 *
 * @code{.cpp}
 * std::vector<bool> vec(myMesh.elementNumber<ElemId::VERTEX>());
 * vcl::elementSelectionToBuffer<ElemId::VERTEX>(myMesh, vec.data());
 * @endif
 *
 * @note This function does not guarantee that the rows of the buffer
 * correspond to the element indices of the mesh. This scenario is possible
 * when the mesh has deleted elements. To be sure to have a direct
 * correspondence, compact the element container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 *
 * @ingroup export_buffer
 */
template<uint ELEM_ID, MeshConcept MeshType>
void elementSelectionToBuffer(const MeshType& mesh, auto* buffer)
{
    for (uint i = 0; const auto& e : mesh.template elements<ELEM_ID>()) {
        buffer[i] = e.selected();
        ++i;
    }
}

/**
 * @brief Export the selection status of the vertices of a mesh to a buffer.
 *
 * This function exports the selection status of the vertices of a mesh to a
 * buffer. Values are stored in the buffer following the order the vertices
 * appear in the mesh. The buffer must be preallocated with the correct size
 * (number of elements).
 *
 * Usage example with std::vector<bool>:
 *
 * @code{.cpp}
 * std::vector<bool> vec(myMesh.vertexNumber());
 * vcl::vertexSelectionToBuffer(myMesh, vec.data());
 * @endif
 *
 * @note This function does not guarantee that the rows of the buffer
 * correspond to the vertex indices of the mesh. This scenario is possible
 * when the mesh has deleted vertices. To be sure to have a direct
 * correspondence, compact the vertex container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 *
 * @ingroup export_buffer
 */
template<MeshConcept MeshType>
void vertexSelectionToBuffer(const MeshType& mesh, auto* buffer)
{
    elementSelectionToBuffer<ElemId::VERTEX>(mesh, buffer);
}

/**
 * @brief Export the selection status of the faces of a mesh to a buffer.
 *
 * This function exports the selection status of the faces of a mesh to a
 * buffer. Values are stored in the buffer following the order the faces appear
 * in the mesh. The buffer must be preallocated with the correct size (number of
 * elements).
 *
 * Usage example with std::vector<bool>:
 *
 * @code{.cpp}
 * std::vector<bool> vec(myMesh.faceNumber());
 * vcl::faceSelectionToBuffer(myMesh, vec.data());
 * @endif
 *
 * @note This function does not guarantee that the rows of the buffer
 * correspond to the face indices of the mesh. This scenario is possible
 * when the mesh has deleted faces. To be sure to have a direct
 * correspondence, compact the face container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 *
 * @ingroup export_buffer
 */
template<FaceMeshConcept MeshType>
void faceSelectionToBuffer(const MeshType& mesh, auto* buffer)
{
    elementSelectionToBuffer<ElemId::FACE>(mesh, buffer);
}

/**
 * @brief Export the selection status of the edges of a mesh to a buffer.
 *
 * This function exports the selection status of the edges of a mesh to a
 * buffer. Values are stored in the buffer following the order the edges appear
 * in the mesh. The buffer must be preallocated with the correct size (number of
 * elements).
 *
 * Usage example with std::vector<bool>:
 *
 * @code{.cpp}
 * std::vector<bool> vec(myMesh.edgeNumber());
 * vcl::edgeSelectionToBuffer(myMesh, vec.data());
 * @endif
 *
 * @note This function does not guarantee that the rows of the buffer
 * correspond to the edge indices of the mesh. This scenario is possible
 * when the mesh has deleted edges. To be sure to have a direct
 * correspondence, compact the edge container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 *
 * @ingroup export_buffer
 */
template<EdgeMeshConcept MeshType>
void edgeSelectionToBuffer(const MeshType& mesh, auto* buffer)
{
    elementSelectionToBuffer<ElemId::EDGE>(mesh, buffer);
}

/**
 * @brief Export the element normals identified by `ELEM_ID` of a mesh to a
 * buffer.
 *
 * This function exports the element normals identified by `ELEM_ID` of a mesh
 * to a buffer. Normals are stored in the buffer following the order the
 * elements appear in the mesh. The buffer must be preallocated with the correct
 * size (number of elements times 3).
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the element indices of the mesh. This scenario is possible
 * when the mesh has deleted elements. To be sure to have a direct
 * correspondence, compact the element container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] storage: storage type of the matrix (row or column major)
 * @param[in] rowNumber: number of rows of the matrix (if different from the
 * number of elements in the mesh) - used only when storage is column major
 *
 * @ingroup export_buffer
 */
template<uint ELEM_ID, MeshConcept MeshType>
void elementNormalsToBuffer(
    const MeshType&   mesh,
    auto*             buffer,
    MatrixStorageType storage = MatrixStorageType::ROW_MAJOR,
    uint              rowNumber = UINT_NULL)
{
    requirePerElementComponent<ELEM_ID, CompId::NORMAL>(mesh);

    if (storage == MatrixStorageType::ROW_MAJOR) {
        for (uint i = 0; const auto& n :
                         mesh.template elements<ELEM_ID>() | views::normals) {
            buffer[i * 3 + 0] = n.x();
            buffer[i * 3 + 1] = n.y();
            buffer[i * 3 + 2] = n.z();
            ++i;
        }
    }
    else {
        const uint ELEM_NUM = rowNumber == UINT_NULL ?
                                  mesh.template number<ELEM_ID>() :
                                  rowNumber;
        for (uint i = 0; const auto& n :
                         mesh.template elements<ELEM_ID>() | views::normals) {
            buffer[0 * ELEM_NUM + i] = n.x();
            buffer[1 * ELEM_NUM + i] = n.y();
            buffer[2 * ELEM_NUM + i] = n.z();
            ++i;
        }
    }
}

/**
 * @brief Export the vertex normals of a mesh to a buffer.
 *
 * This function exports the vertex normals of a mesh to a buffer. Normals are
 * stored in the buffer following the order the vertices appear in the mesh. The
 * buffer must be preallocated with the correct size (number of vertices times
 * 3).
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the vertex indices of the mesh. This scenario is possible
 * when the mesh has deleted vertices. To be sure to have a direct
 * correspondence, compact the vertex container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] storage: storage type of the matrix (row or column major)
 * @param[in] rowNumber: number of rows of the matrix (if different from the
 * number of vertices in the mesh) - used only when storage is column major
 *
 * @ingroup export_buffer
 */
template<MeshConcept MeshType>
void vertexNormalsToBuffer(
    const MeshType&   mesh,
    auto*             buffer,
    MatrixStorageType storage = MatrixStorageType::ROW_MAJOR,
    uint              rowNumber = UINT_NULL)
{
    elementNormalsToBuffer<ElemId::VERTEX>(mesh, buffer, storage, rowNumber);
}

/**
 * @brief Export the face normals of a mesh to a buffer.
 *
 * This function exports the face normals of a mesh to a buffer. Normals are
 * stored in the buffer following the order the faces appear in the mesh. The
 * buffer must be preallocated with the correct size (number of faces times 3).
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the face indices of the mesh. This scenario is possible when
 * the mesh has deleted faces. To be sure to have a direct correspondence,
 * compact the face container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] storage: storage type of the matrix (row or column major)
 * @param[in] rowNumber: number of rows of the matrix (if different from the
 * number of faces in the mesh) - used only when storage is column major
 *
 * @ingroup export_buffer
 */
template<FaceMeshConcept MeshType>
void faceNormalsToBuffer(
    const MeshType&   mesh,
    auto*             buffer,
    MatrixStorageType storage = MatrixStorageType::ROW_MAJOR,
    uint              rowNumber = UINT_NULL)
{
    elementNormalsToBuffer<ElemId::FACE>(mesh, buffer, storage, rowNumber);
}

/**
 * @brief Export into a buffer the normals for each triangle computed by
 * triangulating the faces of a Mesh.
 *
 * This function exports the normals of the triangles computed by
 * triangulating the faces of a mesh to a buffer. Normals are stored following
 * the order the faces appear in the mesh. The buffer must be preallocated with
 * the correct size (number of *resulting triangles* times 3).
 *
 * The function requires an already computed index map, which maps each triangle
 * to the face index and vice versa. You can use the @ref
 * vcl::triangulatedFaceIndicesToBuffer function to get the index map. You can
 * use the function @ref vcl::countTriangulatedTriangles to get the number of
 * resulting triangles and allocate the buffer accordingly.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] indexMap: map from triangle index to face index
 * @param[in] storage: storage type of the matrix (row or column major)
 * @param[in] rowNumber: number of rows of the matrix (if different from the
 * number of triangles in the map) - used only when storage is column major
 *
 * @ingroup export_buffer
 */
template<FaceMeshConcept MeshType>
void triangulatedFaceNormalsToBuffer(
    const MeshType&          mesh,
    auto*                    buffer,
    const TriPolyIndexBiMap& indexMap,
    MatrixStorageType        storage   = MatrixStorageType::ROW_MAJOR,
    uint                     rowNumber = UINT_NULL)
{
    requirePerElementComponent<ElemId::FACE, CompId::NORMAL>(mesh);

    if (storage == MatrixStorageType::ROW_MAJOR) {
        for (const auto& f : mesh.faces()) {
            const auto& n     = f.normal();
            uint        first = indexMap.triangleBegin(f.index());
            uint        last  = first + indexMap.triangleNumber(f.index());
            for (uint t = first; t < last; ++t) {
                buffer[t * 3 + 0] = n.x();
                buffer[t * 3 + 1] = n.y();
                buffer[t * 3 + 2] = n.z();
            }
        }
    }
    else {
        const uint FACE_NUM =
            rowNumber == UINT_NULL ? indexMap.triangleNumber() : rowNumber;
        for (const auto& f : mesh.faces()) {
            const auto& n     = f.normal();
            uint        first = indexMap.triangleBegin(f.index());
            uint        last  = first + indexMap.triangleNumber(f.index());
            for (uint t = first; t < last; ++t) {
                buffer[0 * FACE_NUM + t] = n.x();
                buffer[1 * FACE_NUM + t] = n.y();
                buffer[2 * FACE_NUM + t] = n.z();
            }
        }
    }
}

/**
 * @brief Export the edge normals of a mesh to a buffer.
 *
 * This function exports the edge normals of a mesh to a buffer. Normals are
 * stored in the buffer following the order the edges appear in the mesh. The
 * buffer must be preallocated with the correct size (number of edges times 3).
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the edge indices of the mesh. This scenario is possible when
 * the mesh has deleted edges. To be sure to have a direct correspondence,
 * compact the edge container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] storage: storage type of the matrix (row or column major)
 * @param[in] rowNumber: number of rows of the matrix (if different from the
 * number of edges in the mesh) - used only when storage is column major
 *
 * @ingroup export_buffer
 */
template<EdgeMeshConcept MeshType>
void edgeNormalsToBuffer(
    const MeshType&   mesh,
    auto*             buffer,
    MatrixStorageType storage   = MatrixStorageType::ROW_MAJOR,
    uint              rowNumber = UINT_NULL)
{
    elementNormalsToBuffer<ElemId::EDGE>(mesh, buffer, storage, rowNumber);
}

/**
 * @brief Export the element colors identified by `ELEM_ID` of a mesh to a
 * buffer having a value for each color component (RGBA).
 *
 * This function exports the element colors identified by `ELEM_ID` of a mesh
 * to a buffer. Colors are stored in the buffer following the order the elements
 * appear in the mesh. The buffer must be preallocated with the correct size
 * (number of elements times 4).
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the element indices of the mesh. This scenario is possible
 * when the mesh has deleted elements. To be sure to have a direct
 * correspondence, compact the element container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] storage: storage type of the matrix (row or column major)
 * @param[in] representation: representation of the color components (integer or
 * float)
 * @param[in] rowNumber: number of rows of the matrix (if different from the
 * number of elements in the mesh) - used only when storage is column major
 *
 * @ingroup export_buffer
 */
template<uint ELEM_ID, MeshConcept MeshType>
void elementColorsToBuffer(
    const MeshType&       mesh,
    auto*                 buffer,
    MatrixStorageType     storage        = MatrixStorageType::ROW_MAJOR,
    Color::Representation representation = Color::Representation::INT_0_255,
    uint                  rowNumber      = UINT_NULL)
{
    requirePerElementComponent<ELEM_ID, CompId::COLOR>(mesh);

    const bool R_INT = representation == Color::Representation::INT_0_255;

    if (storage == MatrixStorageType::ROW_MAJOR) {
        for (uint i = 0; const auto& c :
                         mesh.template elements<ELEM_ID>() | views::colors) {
            buffer[i * 4 + 0] = R_INT ? c.red() : c.redF();
            buffer[i * 4 + 1] = R_INT ? c.green() : c.greenF();
            buffer[i * 4 + 2] = R_INT ? c.blue() : c.blueF();
            buffer[i * 4 + 3] = R_INT ? c.alpha() : c.alphaF();
            ++i;
        }
    }
    else {
        const uint ELEM_NUM = rowNumber == UINT_NULL ?
                                  mesh.template number<ELEM_ID>() :
                                  rowNumber;
        for (uint i = 0; const auto& c :
                         mesh.template elements<ELEM_ID>() | views::colors) {
            buffer[0 * ELEM_NUM + i] = R_INT ? c.red() : c.redF();
            buffer[1 * ELEM_NUM + i] = R_INT ? c.green() : c.greenF();
            buffer[2 * ELEM_NUM + i] = R_INT ? c.blue() : c.blueF();
            buffer[3 * ELEM_NUM + i] = R_INT ? c.alpha() : c.alphaF();
            ++i;
        }
    }
}

/**
 * @brief Export the element colors identified by `ELEM_ID` of a mesh to a
 * buffer having a value for each color (the color is packed in a single 32 bit
 * value using the provided format).
 *
 * This function exports the element colors identified by `ELEM_ID` of a mesh to
 * a buffer. Colors are stored in the buffer following the order the elements
 * appear in the mesh. The buffer must be preallocated with the correct size
 * (number of elements).
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the element indices of the mesh. This scenario is possible when
 * the mesh has deleted elements. To be sure to have a direct correspondence,
 * compact the element container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] colorFormat: format of the color components
 *
 * @ingroup export_buffer
 */
template<uint ELEM_ID, MeshConcept MeshType>
void elementColorsToBuffer(
    const MeshType& mesh,
    auto*           buffer,
    Color::Format   colorFormat)
{
    requirePerElementComponent<ELEM_ID, CompId::COLOR>(mesh);

    for (uint        i = 0;
         const auto& c : mesh.template elements<ELEM_ID>() | views::colors) {
        switch (colorFormat) {
            using enum Color::Format;
        case ABGR: buffer[i] = c.abgr(); break;
        case ARGB: buffer[i] = c.argb(); break;
        case RGBA: buffer[i] = c.rgba(); break;
        case BGRA: buffer[i] = c.bgra(); break;
        }
        ++i;
    }
}

/**
 * @brief Export the vertex colors of a mesh to a buffer having a value for each
 * color component (RGBA).
 *
 * This function exports the vertex colors of a mesh to a buffer. Colors are
 * stored in the buffer following the order the vertices appear in the mesh. The
 * buffer must be preallocated with the correct size (number of vertices times
 * 4).
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the vertex indices of the mesh. This scenario is possible
 * when the mesh has deleted vertices. To be sure to have a direct
 * correspondence, compact the vertex container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] storage: storage type of the matrix (row or column major)
 * @param[in] representation: representation of the color components (integer or
 * float)
 * @param[in] rowNumber: number of rows of the matrix (if different from the
 * number of vertices in the mesh) - used only when storage is column major
 *
 * @ingroup export_buffer
 */
template<MeshConcept MeshType>
void vertexColorsToBuffer(
    const MeshType&       mesh,
    auto*                 buffer,
    MatrixStorageType     storage        = MatrixStorageType::ROW_MAJOR,
    Color::Representation representation = Color::Representation::INT_0_255,
    uint                  rowNumber      = UINT_NULL)
{
    elementColorsToBuffer<ElemId::VERTEX>(
        mesh, buffer, storage, representation, rowNumber);
}

/**
 * @brief Export the vertex colors of a mesh to a buffer having a value for each
 * color (the color is packed in a single 32 bit value using the provided
 * format).
 *
 * This function exports the vertex colors of a mesh to a buffer. Colors are
 * stored in the buffer following the order the vertices appear in the mesh. The
 * buffer must be preallocated with the correct size (number of vertices).
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the vertex indices of the mesh. This scenario is possible when
 * the mesh has deleted vertices. To be sure to have a direct correspondence,
 * compact the vertex container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] colorFormat: format of the color components
 *
 * @ingroup export_buffer
 */
template<MeshConcept MeshType>
void vertexColorsToBuffer(
    const MeshType& mesh,
    auto*           buffer,
    Color::Format   colorFormat)
{
    elementColorsToBuffer<ElemId::VERTEX>(mesh, buffer, colorFormat);
}

/**
 * @brief Export the face colors of a mesh to a buffer having a value for each
 * color component (RGBA).
 *
 * This function exports the face colors of a mesh to a buffer. Colors are
 * stored in the buffer following the order the faces appear in the mesh. The
 * buffer must be preallocated with the correct size (number of faces times 4).
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the face indices of the mesh. This scenario is possible
 * when the mesh has deleted faces. To be sure to have a direct
 * correspondence, compact the face container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] storage: storage type of the matrix (row or column major)
 * @param[in] representation: representation of the color components (integer or
 * float)
 * @param[in] rowNumber: number of rows of the matrix (if different from the
 * number of faces in the mesh) - used only when storage is column major
 *
 * @ingroup export_buffer
 */
template<FaceMeshConcept MeshType>
void faceColorsToBuffer(
    const MeshType&       mesh,
    auto*                 buffer,
    MatrixStorageType     storage        = MatrixStorageType::ROW_MAJOR,
    Color::Representation representation = Color::Representation::INT_0_255,
    uint                  rowNumber      = UINT_NULL)
{
    elementColorsToBuffer<ElemId::FACE>(
        mesh, buffer, storage, representation, rowNumber);
}

/**
 * @brief Export into a buffer the color values (RGBA) for each triangle
 * computed by triangulating the faces of a Mesh.
 *
 * This function exports the colors of the triangles computed by
 * triangulating the faces of a mesh to a buffer. Colors are stored following
 * the order the faces appear in the mesh. The buffer must be preallocated with
 * the correct size (number of *resulting triangles* times 4).
 *
 * The function requires an already computed index map, which maps each triangle
 * to the face index and vice versa. You can use the @ref
 * vcl::triangulatedFaceIndicesToBuffer function to get the index map. You can
 * use the function @ref vcl::countTriangulatedTriangles to get the number of
 * resulting triangles and allocate the buffer accordingly.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] indexMap: map from triangle index to face index
 * @param[in] storage: storage type of the matrix (row or column major)
 * @param[in] representation: representation of the color components (integer or
 * float)
 * @param[in] rowNumber: number of rows of the matrix (if different from the
 * number of triangles in the map) - used only when storage is column major
 *
 * @ingroup export_buffer
 */
template<FaceMeshConcept MeshType>
void triangulatedFaceColorsToBuffer(
    const MeshType&          mesh,
    auto*                    buffer,
    const TriPolyIndexBiMap& indexMap,
    MatrixStorageType        storage        = MatrixStorageType::ROW_MAJOR,
    Color::Representation    representation = Color::Representation::INT_0_255,
    uint                     rowNumber      = UINT_NULL)
{
    requirePerElementComponent<ElemId::FACE, CompId::COLOR>(mesh);

    const bool R_INT = representation == Color::Representation::INT_0_255;

    if (storage == MatrixStorageType::ROW_MAJOR) {
        for (const auto& f : mesh.faces()) {
            const auto& c     = f.color();
            uint        first = indexMap.triangleBegin(f.index());
            uint        last  = first + indexMap.triangleNumber(f.index());
            for (uint t = first; t < last; ++t) {
                buffer[t * 4 + 0] = R_INT ? c.red() : c.redF();
                buffer[t * 4 + 1] = R_INT ? c.green() : c.greenF();
                buffer[t * 4 + 2] = R_INT ? c.blue() : c.blueF();
                buffer[t * 4 + 3] = R_INT ? c.alpha() : c.alphaF();
            }
        }
    }
    else {
        const uint FACE_NUM =
            rowNumber == UINT_NULL ? indexMap.triangleNumber() : rowNumber;
        for (const auto& f : mesh.faces()) {
            const auto& c     = f.color();
            uint        first = indexMap.triangleBegin(f.index());
            uint        last  = first + indexMap.triangleNumber(f.index());
            for (uint t = first; t < last; ++t) {
                buffer[0 * FACE_NUM + t] = R_INT ? c.red() : c.redF();
                buffer[1 * FACE_NUM + t] = R_INT ? c.green() : c.greenF();
                buffer[2 * FACE_NUM + t] = R_INT ? c.blue() : c.blueF();
                buffer[3 * FACE_NUM + t] = R_INT ? c.alpha() : c.alphaF();
            }
        }
    }
}

/**
 * @brief Export the face colors of a mesh to a buffer having a value for each
 * color (the color is packed in a single 32 bit value using the provided
 * format).
 *
 * This function exports the face colors of a mesh to a buffer. Colors are
 * stored in the buffer following the order the faces appear in the mesh. The
 * buffer must be preallocated with the correct size (number of faces).
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the face indices of the mesh. This scenario is possible when
 * the mesh has deleted faces. To be sure to have a direct correspondence,
 * compact the face container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] colorFormat: format of the color components
 *
 * @ingroup export_buffer
 */
template<FaceMeshConcept MeshType>
void faceColorsToBuffer(
    const MeshType& mesh,
    auto*           buffer,
    Color::Format   colorFormat)
{
    elementColorsToBuffer<ElemId::FACE>(mesh, buffer, colorFormat);
}

/**
 * @brief Export the colors for each triangle computed by triangulating the
 * faces of a Mesh to a buffer having a value for each color (the color is
 * packed in a single 32 bit value using the provided format)..
 *
 * This function exports the colors of the triangles computed by
 * triangulating the faces of a mesh to a buffer. Colors are stored following
 * the order the faces appear in the mesh. The buffer must be preallocated with
 * the correct size (number of *resulting triangles*).
 *
 * The function requires an already computed index map, which maps each triangle
 * to the face index and vice versa. You can use the @ref
 * vcl::triangulatedFaceIndicesToBuffer function to get the index map. You can
 * use the function @ref vcl::countTriangulatedTriangles to get the number of
 * resulting triangles and allocate the buffer accordingly.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] indexMap: map from triangle index to face index
 * @param[in] colorFormat: format of the color components
 *
 * @ingroup export_buffer
 */
template<FaceMeshConcept MeshType>
void triangulatedFaceColorsToBuffer(
    const MeshType&          mesh,
    auto*                    buffer,
    const TriPolyIndexBiMap& indexMap,
    Color::Format            colorFormat)
{
    requirePerElementComponent<ElemId::FACE, CompId::COLOR>(mesh);

    for (const auto& f : mesh.faces()) {
        const auto& c     = f.color();
        uint        first = indexMap.triangleBegin(f.index());
        uint        last  = first + indexMap.triangleNumber(f.index());
        for (uint t = first; t < last; ++t) {
            switch (colorFormat) {
                using enum Color::Format;
            case ABGR: buffer[t] = c.abgr(); break;
            case ARGB: buffer[t] = c.argb(); break;
            case RGBA: buffer[t] = c.rgba(); break;
            case BGRA: buffer[t] = c.bgra(); break;
            }
        }
    }
}

/**
 * @brief Export the edge colors of a mesh to a buffer having a value for each
 * color component (RGBA).
 *
 * This function exports the edge colors of a mesh to a buffer. Colors are
 * stored in the buffer following the order the edges appear in the mesh. The
 * buffer must be preallocated with the correct size (number of edges times 4).
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the face indices of the mesh. This scenario is possible
 * when the mesh has deleted edges. To be sure to have a direct
 * correspondence, compact the edge container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] storage: storage type of the matrix (row or column major)
 * @param[in] representation: representation of the color components (integer or
 * float)
 * @param[in] rowNumber: number of rows of the matrix (if different from the
 * number of edges in the mesh) - used only when storage is column major
 *
 * @ingroup export_buffer
 */
template<EdgeMeshConcept MeshType>
void edgeColorsToBuffer(
    const MeshType&       mesh,
    auto*                 buffer,
    MatrixStorageType     storage        = MatrixStorageType::ROW_MAJOR,
    Color::Representation representation = Color::Representation::INT_0_255,
    uint                  rowNumber      = UINT_NULL)
{
    elementColorsToBuffer<ElemId::EDGE>(
        mesh, buffer, storage, representation, rowNumber);
}

/**
 * @brief Export the edge colors of a mesh to a buffer having a value for each
 * color (the color is packed in a single 32 bit value using the provided
 * format).
 *
 * This function exports the edge colors of a mesh to a buffer. Colors are
 * stored in the buffer following the order the edges appear in the mesh. The
 * buffer must be preallocated with the correct size (number of edges).
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the edge indices of the mesh. This scenario is possible when
 * the mesh has deleted edges. To be sure to have a direct correspondence,
 * compact the edge container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] colorFormat: format of the color components
 *
 * @ingroup export_buffer
 */
template<EdgeMeshConcept MeshType>
void edgeColorsToBuffer(
    const MeshType& mesh,
    auto*           buffer,
    Color::Format   colorFormat)
{
    elementColorsToBuffer<ElemId::EDGE>(mesh, buffer, colorFormat);
}

/**
 * @brief Export the element quality identified by `ELEM_ID` of a mesh to a
 * buffer.
 *
 * This function exports the element quality identified by `ELEM_ID` of a mesh
 * to a buffer. Quality values are stored in the buffer following the order the
 * elements appear in the mesh. The buffer must be preallocated with the correct
 * size (number of elements).
 *
 * @note This function does not guarantee that the rows of the vector
 * correspond to the element indices of the mesh. This scenario is possible when
 * the mesh has deleted elements. To be sure to have a direct correspondence,
 * compact the element container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 *
 * @ingroup export_buffer
 */
template<uint ELEM_ID, MeshConcept MeshType>
void elementQualityToBuffer(const MeshType& mesh, auto* buffer)
{
    requirePerElementComponent<ELEM_ID, CompId::QUALITY>(mesh);

    for (uint        i = 0;
         const auto& q : mesh.template elements<ELEM_ID>() | views::quality) {
        buffer[i] = q;
        ++i;
    }
}

/**
 * @brief Export the vertex quality of a mesh to a buffer.
 *
 * This function exports the vertex quality of a mesh to a buffer. Quality
 * values are stored in the buffer following the order the vertices appear in
 * the mesh. The buffer must be preallocated with the correct size (number of
 * vertices).
 *
 * @note This function does not guarantee that the rows of the vector
 * correspond to the vertex indices of the mesh. This scenario is possible when
 * the mesh has deleted vertices. To be sure to have a direct correspondence,
 * compact the vertex container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 *
 * @ingroup export_buffer
 */
template<MeshConcept MeshType>
void vertexQualityToBuffer(const MeshType& mesh, auto* buffer)
{
    elementQualityToBuffer<ElemId::VERTEX>(mesh, buffer);
}

/**
 * @brief Export the face quality of a mesh to a buffer.
 *
 * This function exports the face quality of a mesh to a buffer. Quality values
 * are stored in the buffer following the order the faces appear in the mesh.
 * The buffer must be preallocated with the correct size (number of faces).
 *
 * @note This function does not guarantee that the rows of the vector
 * correspond to the face indices of the mesh. This scenario is possible when
 * the mesh has deleted faces. To be sure to have a direct correspondence,
 * compact the face container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 *
 * @ingroup export_buffer
 */
template<MeshConcept MeshType>
void faceQualityToBuffer(const MeshType& mesh, auto* buffer)
{
    elementQualityToBuffer<ElemId::FACE>(mesh, buffer);
}

/**
 * @brief Export the edge quality of a mesh to a buffer.
 *
 * This function exports the edge quality of a mesh to a buffer. Quality values
 * are stored in the buffer following the order the edges appear in the mesh.
 * The buffer must be preallocated with the correct size (number of edges).
 *
 * @note This function does not guarantee that the rows of the vector
 * correspond to the edge indices of the mesh. This scenario is possible when
 * the mesh has deleted edges. To be sure to have a direct correspondence,
 * compact the edge container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 *
 * @ingroup export_buffer
 */
template<MeshConcept MeshType>
void edgeQualityToBuffer(const MeshType& mesh, auto* buffer)
{
    elementQualityToBuffer<ElemId::FACE>(mesh, buffer);
}

/**
 * @brief Export the vertex texcoords of a mesh to a buffer.
 *
 * This function exports the vertex texcoords of a mesh to a buffer. Texcoords
 * are stored in the buffer following the order the vertices appear in the mesh.
 * The buffer must be preallocated with the correct size (number of vertices
 * times 2).
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the vertex indices of the mesh. This scenario is possible
 * when the mesh has deleted vertices. To be sure to have a direct
 * correspondence, compact the vertex container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] storage: storage type of the matrix (row or column major)
 * @param[in] rowNumber: number of rows of the matrix (if different from the
 * number of vertices in the mesh) - used only when storage is column major
 *
 * @ingroup export_buffer
 */
template<MeshConcept MeshType>
void vertexTexCoordsToBuffer(
    const MeshType&   mesh,
    auto*             buffer,
    MatrixStorageType storage = MatrixStorageType::ROW_MAJOR,
    uint              rowNumber = UINT_NULL)
{
    requirePerVertexComponent<CompId::TEX_COORD>(mesh);

    if (storage == MatrixStorageType::ROW_MAJOR) {
        for (uint i = 0; const auto& t : mesh.vertices() | views::texCoords) {
            buffer[i * 2 + 0] = t.u();
            buffer[i * 2 + 1] = t.v();
            ++i;
        }
    }
    else {
        const uint VERT_NUM =
            rowNumber == UINT_NULL ? mesh.vertexNumber() : rowNumber;
        for (uint i = 0; const auto& t : mesh.vertices() | views::texCoords) {
            buffer[0 * VERT_NUM + i] = t.u();
            buffer[1 * VERT_NUM + i] = t.v();
            ++i;
        }
    }
}

/**
 * @brief Export the vertex texcoord indices of a mesh to a buffer.
 *
 * This function exports the vertex texcoord indices of a mesh to a buffer.
 * Indices are stored in the buffer following the order the vertices appear in
 * the mesh. The buffer must be preallocated with the correct size (number of
 * vertices).
 *
 * @note This function does not guarantee that the rows of the buffer
 * correspond to the vertex indices of the mesh. This scenario is possible
 * when the mesh has deleted vertices. To be sure to have a direct
 * correspondence, compact the vertex container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 *
 * @ingroup export_buffer
 */
template<MeshConcept MeshType>
void vertexTexCoordIndicesToBuffer(const MeshType& mesh, auto* buffer)
{
    requirePerVertexComponent<CompId::TEX_COORD>(mesh);

    for (uint i = 0; const auto& t : mesh.vertices() | views::texCoords) {
        buffer[i] = t.index();
        ++i;
    }
}

} // namespace vcl

#endif // VCL_ALGORITHMS_MESH_IMPORT_EXPORT_EXPORT_BUFFER_H
